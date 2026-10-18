#include "deriv/analytic/BlackScholesCall.h"
#include <cmath>
using namespace std;

namespace deriv {
namespace analytic {

    double NormalCDF(double x)
    {
        double gamma = 0.2316419;     double a1 = 0.319381530;
        double a2 = -0.356563782;     double a3 = 1.781477937;
        double a4 = -1.821255978;     double a5 = 1.330274429;
        double pi = 4.0 * atan(1.0);  double k = 1.0 / (1.0 + gamma * x);
        if (x >= 0.0)
        {
            return 1.0 - ((((a5 * k + a4) * k + a3) * k + a2) * k + a1) * k * exp(-x * x / 2.0) / sqrt(2.0 * pi);
        }
        else return 1.0 - NormalCDF(-x);
    }

    double BlackScholesCall::d_plus(double S0, double sigma, double r)
    {
        return (log(S0 / K) + (r + 0.5 * pow(sigma, 2.0)) * T) / (sigma * sqrt(T));
    }

    double BlackScholesCall::d_minus(double S0, double sigma, double r)
    {
        return d_plus(S0, sigma, r) - sigma * sqrt(T);
    }

    double BlackScholesCall::PriceByBSFormula(double S0, double sigma, double r)
    {
        return S0 * NormalCDF(d_plus(S0, sigma, r)) - K * exp(-r * T) * NormalCDF(d_minus(S0, sigma, r));
    }

    double BlackScholesCall::VegaByBSFormula(double S0, double sigma, double r)
    {
        double pi = 4.0 * atan(1.0);
        return S0 * exp(-d_plus(S0, sigma, r) * d_plus(S0, sigma, r) / 2) * sqrt(T) / sqrt(2.0 * pi);
    }

    double BlackScholesCall::DeltaByBSFormula(double S0, double sigma, double r)
    {
        return NormalCDF(d_plus(S0, sigma, r));
    }

}
}
