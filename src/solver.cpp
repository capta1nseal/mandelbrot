#include "solver.hpp"

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "complex.hpp"
#include "grid2d.hpp"
#include "workqueue.hpp"

Solver::Solver() {
    m_iterationCount = 0;
    m_iterationMaximum = 8192;
    m_escapeRadius = 2.0;
    m_width = 1;
    m_height = 1;
    aspectRatio = static_cast<double>(m_width) / static_cast<double>(m_height);
    m_viewCenter = {-0.5, 0.0};
    m_viewScale = 1.0;

    m_currentFractal = true;
    m_juliaConstant = m_viewCenter;
}

void Solver::initializeGrid(int width, int height, double viewCenterReal,
                            double viewCenterImag, double viewScale) {
    {
        std::lock_guard<std::mutex> lock(calculationMutex);

        m_viewCenter = {viewCenterReal, viewCenterImag};
        m_viewScale = viewScale;
    }

    resizeGrid(width, height);
}

void Solver::resizeGrid(int width, int height) {
    std::lock_guard<std::mutex> lock(calculationMutex);

    m_width = width;
    m_height = height;

    aspectRatio = static_cast<double>(m_width) / static_cast<double>(m_height);

    resetGrid();
}

void Solver::resetGrid() {
    workQueue.abortIteration();

    m_grid.resize(m_width, m_height);
    if (m_currentFractal) {
        m_grid.assign(m_width, m_height, Complex(0.0, 0.0));
    } else {
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                m_grid[x, y] = mapToComplex(x, y);
            }
        }
    }

    m_iterationGrid.resize(m_width, m_height);
    m_iterationGrid.assign(m_width, m_height, 0);

    m_magnitudeSquaredGrid.resize(m_width, m_height);
    m_magnitudeSquaredGrid.assign(m_width, m_height, 0.0);

    m_escapeCount = 0;
    escapeIterationCounter.resize(m_iterationMaximum);
    escapeIterationCounter.assign(m_iterationMaximum, 0);

    m_iterationCount = 0;
}

void Solver::toggleJulia() {
    std::lock_guard<std::mutex> lock(calculationMutex);

    if (m_currentFractal) { // Switch to julia set.
        m_juliaConstant = m_viewCenter;
        std::cout << "Switching to julia set.\n";
    } else { // Switch to mandelbrot set.
        m_viewCenter = m_juliaConstant;
        std::cout << "Switching to mandelbrot set.\n";
    }
    m_currentFractal = !m_currentFractal;

    resetGrid();
}

void Solver::calculationLoop() {
    isRunning = true;
    while (isRunning) {
        iterateGrid();
    }
}

void Solver::stop() { isRunning = false; }

int Solver::getMaxIterationCount() { return m_iterationMaximum; }

void Solver::getFrameData(int& iterationCount, int& escapeCount,
                          Grid2d<double>& magnitudeSquaredGrid,
                          Grid2d<int>& iterationGrid,
                          std::vector<int>& escapeIterationCounterSums) {

    while (m_iterationCount == 0) [[unlikely]] {
    }

    // Loop to keep acquiring mutex until it is acquired after the completion of
    // a non-aborted iteration.
    bool done = false;
    while (!done) {
        std::lock_guard<std::mutex> lock(calculationMutex);

        if (!workQueue.isAborted()) [[likely]] {
            iterationCount = m_iterationCount;

            escapeCount = m_escapeCount;

            magnitudeSquaredGrid = m_magnitudeSquaredGrid;

            iterationGrid = m_iterationGrid;

            escapeIterationCounterSums.resize(m_iterationMaximum);
            escapeIterationCounterSums[0] = escapeIterationCounter[0];
            for (int i = 1; i < m_iterationMaximum; i++) {
                escapeIterationCounterSums[i] =
                    escapeIterationCounterSums[i - 1] +
                    escapeIterationCounter[i];
            }

            done = true;
        }
    }
}

void Solver::zoomIn(double factor) {
    std::lock_guard<std::mutex> lock(calculationMutex);
    m_viewScale *= factor;
    resetGrid();
    printLocation();
}
void Solver::zoomOut(double factor) {
    std::lock_guard<std::mutex> lock(calculationMutex);
    m_viewScale /= factor;
    resetGrid();
    printLocation();
}

void Solver::zoomOnPixel(int x, int y, double factor) {
    std::lock_guard<std::mutex> lock(calculationMutex);
    m_viewCenter = mapToComplex(x, y);
    m_viewScale *= factor;
    resetGrid();
    printLocation();
}

void Solver::move(double real, double imag) {
    std::lock_guard<std::mutex> lock(calculationMutex);
    m_viewCenter += Complex(real / m_viewScale, imag / m_viewScale);
    resetGrid();
    printLocation();
}

void Solver::printLocation() {
    std::cout << std::setprecision(12);
    std::cout << "(" << m_viewCenter.real << ", " << m_viewCenter.imag << ", "
              << m_viewScale << ")\n";
}

Complex Solver::mapToComplex(double x, double y) {
    x += 0.5;
    y += 0.5;
    double realRange = (2.0 * m_escapeRadius) / m_viewScale;
    double imaginaryRange = realRange * (static_cast<double>(m_height) /
                                         static_cast<double>(m_width));
    x *= realRange / m_width;
    y *= imaginaryRange / m_height;

    x += m_viewCenter.real - (m_escapeRadius / m_viewScale);
    y += m_viewCenter.imag - (m_escapeRadius / (m_viewScale * aspectRatio));

    y = 2.0 * m_viewCenter.imag - y;

    return Complex(x, y);
}

void Solver::rowIterator() {
    auto [y, width] = workQueue.getTask();

    while (y != -1) {
        for (int x = 0; x < m_width; x++) {
            if (workQueue.isAborted()) [[unlikely]] {
                break;
            }
            if (m_magnitudeSquaredGrid[x, y] <=
                m_escapeRadius * m_escapeRadius) {
                if (m_currentFractal) { // mandelbrot set.
                    m_grid[x, y].squareAdd(mapToComplex(x, y));
                } else { // julia set.
                    m_grid[x, y].squareAdd(m_juliaConstant);
                }
                m_magnitudeSquaredGrid[x, y] = m_grid[x, y].magnitudeSquared();
                m_iterationGrid[x, y]++;

                if (m_magnitudeSquaredGrid[x, y] >
                    m_escapeRadius * m_escapeRadius) {
                    m_escapeCount++;
                    escapeIterationCounter[m_iterationGrid[x, y] - 1]++;
                }
            }
        }

        std::tie(y, width) = workQueue.getTask();
    }
}

void Solver::iterateGrid() {
    if (m_iterationCount < m_iterationMaximum) {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        std::lock_guard<std::mutex> lock(calculationMutex);

        workQueue.setTaskCount(m_height);
        workQueue.setTaskLength(m_width);

        {
            unsigned int threadCount = std::thread::hardware_concurrency();
            std::vector<std::jthread> threads;

            for (unsigned int i = 0u; i < threadCount; i++) {
                threads.push_back(std::jthread(&Solver::rowIterator, this));
            }
        }

        if (!workQueue.isAborted()) [[likely]] {
            m_iterationCount++;

            if (m_iterationCount >= m_iterationMaximum) {
                std::cout << "max iteration count reached\n";
            }
        }
    }
}
