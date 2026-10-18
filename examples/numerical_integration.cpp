// Composite trapezoid and Simpson rules.
//
// 1. A polynomial with a known integral: Simpson is exact for cubics, the
//    trapezoid rule converges at O(h^2).
// 2. The standard normal CDF, Phi(x) = 1/2 + integral of the density from 0
//    to x, as an independent check on the polynomial approximation that the
//    Black-Scholes formula uses.

#include "deriv/analytic/BlackScholesCall.h"
#include "deriv/numerics/DefiniteIntegral.h"
#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace deriv;

namespace {
    double Cubic(double x) { return x * x * x - x * x + 1; }

    double NormalDensity(double x)
    {
        const double pi = 4.0 * atan(1.0);
        return exp(-x * x / 2.0) / sqrt(2.0 * pi);
    }
}

int main()
{
    cout << fixed << setprecision(10);

    cout << "Integral of x^3 - x^2 + 1 over [1, 2]  (exact: 29/12 = " << 29.0 / 12.0 << ")" << endl;
    numerics::DefiniteIntegral cubic(1.0, 2.0, Cubic);
    cout << setw(8) << "N" << setw(18) << "trapezoid" << setw(18) << "Simpson" << endl;
    for (int N = 4; N <= 1024; N *= 4)
        cout << setw(8) << N << setw(18) << cubic.ByTrapezoid(N) << setw(18) << cubic.BySimpson(N) << endl;
    cout << endl;

    cout << "Standard normal CDF" << endl;
    cout << setw(8) << "x" << setw(18) << "Simpson (N=200)" << setw(18) << "NormalCDF" << setw(18) << "erfc (exact)" << endl;
    double xs[] = { -3.0, -2.0, -1.0, -0.5, 0.5, 1.0, 2.0, 3.0 };
    for (double x : xs)
    {
        numerics::DefiniteIntegral phi(0.0, x, NormalDensity);
        cout << setw(8) << setprecision(2) << x << setprecision(10)
             << setw(18) << 0.5 + phi.BySimpson(200)
             << setw(18) << analytic::NormalCDF(x)
             << setw(18) << 0.5 * erfc(-x / sqrt(2.0)) << endl;
    }

    return 0;
}
