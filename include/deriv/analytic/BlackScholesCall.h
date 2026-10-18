#pragma once

namespace deriv {
namespace analytic {

    // Standard normal cumulative distribution function (polynomial approximation).
    double NormalCDF(double x);

    // European call in the Black-Scholes model, closed form.
    // Market data (S0, sigma, r) is passed per call so the same contract can be
    // evaluated across volatilities, e.g. when solving for implied volatility.
    class BlackScholesCall
    {
    private:
        double T, K;
        double d_plus(double S0, double sigma, double r);
        double d_minus(double S0, double sigma, double r);

    public:
        BlackScholesCall(double T_, double K_) : T(T_), K(K_) {}
        double PriceByBSFormula(double S0, double sigma, double r);
        double VegaByBSFormula(double S0, double sigma, double r);
        double DeltaByBSFormula(double S0, double sigma, double r);
    };

}
}
