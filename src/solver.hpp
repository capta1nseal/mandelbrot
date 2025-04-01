#ifndef _MANDELBROTSOLVER
#define _MANDELBROTSOLVER

#include <mutex>
#include <vector>

#include "complex.hpp"
#include "grid2d.hpp"
#include "workqueue.hpp"

// Wrapper for data and number crunching for the fractal solver.
class Solver {
public:
    Solver();

    void initializeGrid(int width, int height, double viewCenterReal,
                        double viewCenterImag, double viewScale);

    void resizeGrid(int width, int height);

    void resetGrid();

    void toggleJulia();

    void calculationLoop();

    void stop();

    int getMaxIterationCount();

    void getFrameData(int& iterationCount, int& escapeCount,
                      Grid2d<double>& magnitudeGrid, Grid2d<int>& iterationGrid,
                      std::vector<int>& escapeIterationCounterSums);

    void zoomIn(double factor);
    void zoomOut(double factor);

    void zoomOnPixel(int x, int y, double factor);

    void move(double real, double imag);

    void printLocation();

private:
    Grid2d<Complex> m_grid;
    Grid2d<int> m_iterationGrid;

    Grid2d<double> m_magnitudeSquaredGrid;

    std::vector<int> escapeIterationCounter;

    std::atomic_int m_escapeCount;
    std::atomic_int m_iterationCount;
    int m_iterationMaximum;
    double m_escapeRadius;
    int m_width, m_height;
    double aspectRatio;
    Complex m_viewCenter;
    double m_viewScale;

    bool m_isJulia;
    Complex m_juliaCenter;

    bool isRunning;
    WorkQueue workQueue;
    std::mutex calculationMutex;

    Complex mapToComplex(double x, double y);

    // Iterates over one row of the grid, intended for use in multithreading.
    void rowIterator();

    void iterateGrid();
};

#endif
