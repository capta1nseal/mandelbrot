#ifndef _MANDELBROTGRID
#define _MANDELBROTGRID

#include <mutex>
#include <vector>

#include "complex.hpp"
#include "workqueue.hpp"

// Wrapper for data and number crunching for the fractal solver.
class MandelbrotGrid {
public:
    MandelbrotGrid();

    void initializeGrid(int width, int height, double viewCenterReal,
                        double viewCenterImag, double viewScale);

    void resizeGrid(int width, int height);

    void resetGrid();

    void toggleJulia();

    void calculationLoop();

    void stop();

    int getMaxIterationCount();

    void getFrameData(int &iterationCount, int &escapeCount,
                      std::vector<double> &magnitudeGrid,
                      std::vector<int> &iterationGrid,
                      std::vector<int> &escapeIterationCounterSums);

    void zoomIn(double factor);
    void zoomOut(double factor);

    void zoomOnPixel(int x, int y);

    void move(double real, double imag);

    void printLocation();

private:
    std::vector<Complex> m_grid;
    std::vector<int> m_iterationGrid;

    std::vector<double> m_magnitudeSquaredGrid;

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

    bool isRunning;
    WorkQueue workQueue;
    std::mutex calculationMutex;

    Complex mapToComplex(double x, double y);

    void setValueAt(int x, int y, Complex value);

    void incrementIterationGrid(int x, int y);

    // Iterates over one row of the grid, intended for use in multithreading.
    void rowIterator();

    void iterateGrid();
};

#endif
