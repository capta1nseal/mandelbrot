#ifndef _MANDELBROTCOMPLEX
#define _MANDELBROTCOMPLEX

// Simple complex struct with useful functions for fractals.
struct Complex {
    double real;
    double imag;

    Complex();
    Complex(double initReal, double initImag = 0.0);

    Complex& operator+=(const Complex& rhs);
    friend Complex operator+(Complex lhs, const Complex& rhs) {
        lhs += rhs;
        return lhs;
    }

    Complex& operator-=(const Complex& rhs);
    friend Complex operator-(Complex lhs, const Complex& rhs) {
        lhs -= rhs;
        return lhs;
    }

    Complex& operator*=(const Complex& rhs);
    friend Complex operator*(Complex lhs, const Complex& rhs) {
        lhs *= rhs;
        return lhs;
    }

    Complex& operator/=(const Complex& rhs);
    friend Complex operator/(Complex lhs, const Complex& rhs) {
        lhs /= rhs;
        return rhs;
    }

    void squareAdd(Complex other);

    double magnitude();
    double magnitudeSquared();
};

#endif
