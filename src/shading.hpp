#ifndef _MANDELBROTSHADING
#define _MANDELBROTSHADING

#include <tuple>
#include <vector>

// Class for handling shading, in this case meaning:
// Converts a scalar into RGB.
// Does a bit of light functional programming so the shading function can easily
// be switched between some options.
class Shading {
public:
    using Colour = std::tuple<int, int, int>;
    using HsvColour = std::tuple<double, double, double>;
    using ShadingFunction = Colour (Shading::*)(double, double) const;

    Shading();

    Colour shade(double colourFactor, double timeCounter) const;

    void setShadingFunction(int functionNumber);

private:
    ShadingFunction shadingFunction;

    Colour shadeGreyscale(double histogramFactor, double timeCounter) const;

    Colour shadeGreyscaleInverse(double histogramFactor,
                                 double timeCounter) const;

    Colour shadeHsv(double histogramFactor, double timeCounter) const;

    std::vector<std::pair<double, HsvColour>> midnightCherryPath;
    Colour shadeMidnightCherry(double histogramFactor,
                               double timeCounter) const;

    Colour hsvToRgb(HsvColour hsvColour) const;

    Colour colourRamp(const std::vector<std::pair<double, HsvColour>>& hsvPath,
                      double factor) const;

    double lerp(double min, double max, double normalizedFactor) const;
    HsvColour lerp(HsvColour minColour, HsvColour maxColour,
                   double normalizedFactor) const;
};

#endif
