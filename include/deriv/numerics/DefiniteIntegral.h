#pragma once

namespace deriv {
namespace numerics {

    // Definite integral of f over [a, b] by composite quadrature rules.
    class DefiniteIntegral
    {
    private:
        double a, b;
        double (*f)(double x);
        // f(a) + f(b) is shared by both rules, so it is computed once
        const double initial_sum;

    public:
        DefiniteIntegral(double a_, double b_, double (*f_)(double x))
            : a(a_), b(b_), f(f_), initial_sum(f(a) + f(b)) {}

        // composite trapezoidal rule with N subintervals
        double ByTrapezoid(int N) const;

        // composite Simpson's rule with N subintervals (N must be even)
        double BySimpson(int N) const;
    };

}
}
