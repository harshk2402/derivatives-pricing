#pragma once
#include "deriv/analytic/BlackScholesCall.h"
#include "deriv/numerics/Function.h"

namespace deriv {
namespace analytic {

    // Black-Scholes call price as a function of volatility, with vega as its
    // derivative. Pass it to numerics::NonlinearSolver with the observed market
    // price as the target to recover the implied volatility:
    //
    //     ImpliedVolatility iv(T, K, S0, r);
    //     numerics::NonlinearSolver solver(marketPrice, 0.01, 2.0, 1e-6, 0.3);
    //     double sigma = solver.SolveByNR(&iv);
    class ImpliedVolatility : public BlackScholesCall, public numerics::Function
    {
    private:
        double S0, r;

    public:
        ImpliedVolatility(double T_, double K_, double S0_, double r_)
            : BlackScholesCall(T_, K_), S0(S0_), r(r_) {}
        double Value(double sigma) { return PriceByBSFormula(S0, sigma, r); }
        double Deriv(double sigma) { return VegaByBSFormula(S0, sigma, r); }
    };

}
}
