#include "complex.hpp"

#include <cmath>

Complex::Complex() {
    real = 0.0;
    imag = 0.0;
}

Complex::Complex(double initReal, double initImag)
    : real(initReal), imag(initImag) {}

Complex &Complex::operator+=(const Complex &rhs) {
    real += rhs.real;
    imag += rhs.imag;
    return *this;
}

Complex &Complex::operator-=(const Complex &rhs) {
    real -= rhs.real;
    imag -= rhs.imag;
    return *this;
}

Complex &Complex::operator*=(const Complex &rhs) {
    *this = {real * rhs.real - imag * rhs.imag,
             real * rhs.imag + imag * rhs.real};
    return *this;
}

Complex &Complex::operator/=(const Complex &rhs) {
    double denominator = real * real + rhs.real * rhs.real;
    *this = {(real * rhs.real + imag * rhs.imag) / denominator,
             (imag * rhs.real - real * rhs.imag) / denominator};
    return *this;
}

void Complex::squareAdd(Complex other) {
    double realSquared = real * real;
    double imagSquared = imag * imag;
    imag = (real + real) * imag + other.imag;
    real = realSquared - imagSquared + other.real;
}

double Complex::magnitude() { return std::sqrt(real * real + imag * imag); }
double Complex::magnitudeSquared() { return real * real + imag * imag; }
