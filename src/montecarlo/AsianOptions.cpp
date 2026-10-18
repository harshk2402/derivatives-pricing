#include "deriv/montecarlo/AsianOptions.h"
#include "deriv/analytic/BlackScholesCall.h"
#include <cmath>

namespace deriv {
namespace montecarlo {

    double ArithmeticAsianCall::Payoff(const SamplePath& S) const
    {
        double Ave = 0.0;
        for (int k = 0; k < m; k++) Ave = (k * Ave + S[k]) / (k + 1.0);
        if (Ave < K) return 0.0;
        return Ave - K;
    }

    double GeometricAsianCall::Payoff(const SamplePath& S) const
    {
        double Prod = 1.0;
        for (int i = 0; i < m; i++)
        {
            Prod = Prod * S[i];
        }
        if (std::pow(Prod, 1.0 / m) < K) return 0.0;
        return std::pow(Prod, 1.0 / m) - K;
    }

    // The geometric average is distributed as the terminal price of a GBM with
    // initial value a and volatility b, so the option prices as a European call.
    double GeometricAsianCall::PriceByBSFormula(const GBMModel& Model)
    {
        double a = std::exp(-Model.GetR() * T) * Model.GetS0() * std::exp((m + 1.0) * T / (2.0 * m) * (Model.GetR() + Model.GetSigma() * Model.GetSigma() * ((2.0 * m + 1.0) / (3.0 * m) - 1.0) / 2.0));
        double b = Model.GetSigma() * std::sqrt((m + 1.0) * (2.0 * m + 1.0) / (6.0 * m * m));
        analytic::BlackScholesCall G(T, K);
        Price = G.PriceByBSFormula(a, b, Model.GetR());
        return Price;
    }

    double GeometricAsianCall::DeltaByBSFormula(const GBMModel& Model)
    {
        double S0 = Model.GetS0();
        double r = Model.GetR();
        double sigma = Model.GetSigma();
        double a = std::exp(-r * T) * S0 * std::exp((m + 1.0) * T / (2.0 * m) *
            (r + sigma * sigma * ((2.0 * m + 1.0) / (3.0 * m) - 1.0) / 2.0));
        double b = sigma * std::sqrt((m + 1.0) * (2.0 * m + 1.0) / (6.0 * m * m));
        double d = (std::log(a / K) + (r + 0.5 * b * b) * T) / (b * std::sqrt(T));
        double N_d = 0.5 * std::erfc(-d / std::sqrt(2.0));
        return N_d * a / S0;
    }

}
}
