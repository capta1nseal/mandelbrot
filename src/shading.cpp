#include "shading.hpp"

#include <vector>
#include <utility>
#include <tuple>
#include <cmath>

Shading::Shading()
{
    shadingFunction = &Shading::shadeGreyscale;
}

Shading::Colour Shading::shade(double histogramFactor)
{
    return (this->*shadingFunction)(histogramFactor);
}

void Shading::setShadingFunction(int functionNumber)
{
    switch (functionNumber)
    {
    case 0:
        shadingFunction = &Shading::shadeGreyscale;
        break;
    case 1:
        shadingFunction = &Shading::shadeGreyscaleInverse;
        break;
    case 2:
        shadingFunction = &Shading::shadeHsv;
        break;
    case 3:
        shadingFunction = &Shading::shadeMidnightCherry;
        break;
    default:
        setShadingFunction(0);
        break;
    }
}

Shading::Colour Shading::shadeGreyscale(double histogramFactor)
{
    return shadeGreyscaleInverse(1.0 - histogramFactor);
}

Shading::Colour Shading::shadeGreyscaleInverse(double histogramFactor)
{
    int alpha = histogramFactor * 255;

    return {alpha, alpha, alpha};
}

Shading::Colour Shading::shadeHsv(double histogramFactor)
{
    histogramFactor = 1.0 - histogramFactor;
    return hsvToRgb(histogramFactor * 720 + 300, 0.75 + histogramFactor / 4.0, 1.0);
}

Shading::Colour Shading::shadeMidnightCherry(double histogramFactor)
{
    HsvColour midnight = {250.0, 0.80, 0.20};
    HsvColour cherry   = {315.0, 0.90, 0.80};

    std::vector<std::pair<double, HsvColour>> hsvPath = {
        {0.0,  midnight},
        {0.60, cherry},
        {1.0,  midnight}
    };
    return colourRamp(hsvPath, histogramFactor);
}

Shading::Colour Shading::hsvToRgb(double hue, double saturation, double value)
{
    hue = fmodf64(hue, 360.0) / 60.0;
    long i = static_cast<int>(hue);
    double ff = hue - i;
    double p = value * (1.0 - saturation);
    double q = value * (1.0 - (saturation * ff));
    double t = value * (1.0 - (saturation * (1.0 - ff)));

    int r;
    int g;
    int b;

    switch (i)
    {
    case 0:
        r = 255 * value;
        g = 255 * t;
        b = 255 * p;
        break;
    case 1:
        r = 255 * q;
        g = 255 * value;
        b = 255 * p;
        break;
    case 2:
        r = 255 * p;
        g = 255 * value;
        b = 255 * t;
        break;
    case 3:
        r = 255 * p;
        g = 255 * q;
        b = 255 * value;
        break;
    case 4:
        r = 255 * t;
        g = 255 * p;
        b = 255 * value;
        break;
    case 5:
    default:
        r = 255 * value;
        g = 255 * p;
        b = 255 * q;
        break;
    }

    return {r, g, b};
}

Shading::Colour Shading::hsvToRgb(HsvColour hsvColour)
{
    double hue = get<0>(hsvColour);
    double saturation = get<1>(hsvColour);
    double value = get<2>(hsvColour);
    hue = fmodf64(hue, 360.0) / 60.0;
    long i = static_cast<int>(hue);
    double ff = hue - i;
    double p = value * (1.0 - saturation);
    double q = value * (1.0 - (saturation * ff));
    double t = value * (1.0 - (saturation * (1.0 - ff)));

    int r;
    int g;
    int b;

    switch (i)
    {
    case 0:
        r = 255 * value;
        g = 255 * t;
        b = 255 * p;
        break;
    case 1:
        r = 255 * q;
        g = 255 * value;
        b = 255 * p;
        break;
    case 2:
        r = 255 * p;
        g = 255 * value;
        b = 255 * t;
        break;
    case 3:
        r = 255 * p;
        g = 255 * q;
        b = 255 * value;
        break;
    case 4:
        r = 255 * t;
        g = 255 * p;
        b = 255 * value;
        break;
    case 5:
    default:
        r = 255 * value;
        g = 255 * p;
        b = 255 * q;
        break;
    }

    return {r, g, b};
}

Shading::Colour Shading::colourRamp(std::vector<std::pair<double, HsvColour>> hsvPath, double factor)
{
    // case if factor is less than first colour in path
    if (factor <= hsvPath[0].first)
    {
        return hsvToRgb(hsvPath[0].second);
    }

    // all cases which blend between two colours
    // blending algorithm is lerping h,s,v of both colours
    double scaledFactor;
    for (unsigned int i = 1; i < hsvPath.size(); i++)
    {
        if (factor <= hsvPath[i].first)
        {
            scaledFactor = (factor - hsvPath[i - 1].first) / (hsvPath[i].first - hsvPath[i - 1].first);
            return hsvToRgb(lerp(hsvPath[i - 1].second, hsvPath[i].second, scaledFactor));
        }
    }

    // final case if factor is greater than last colour's factor
    return hsvToRgb(hsvPath[hsvPath.size() - 1].second);
}

double Shading::lerp(double min, double max, double normalizedFactor)
{
    return min + normalizedFactor * (max - min);
}
Shading::HsvColour Shading::lerp(HsvColour minColour, HsvColour maxColour, double normalizedFactor)
{
    return {
        lerp(get<0>(minColour), get<0>(maxColour), normalizedFactor),
        lerp(get<1>(minColour), get<1>(maxColour), normalizedFactor),
        lerp(get<2>(minColour), get<2>(maxColour), normalizedFactor)
    };
}
