#pragma once
#include "deriv/montecarlo/PathDepOption.h"

namespace deriv {
namespace montecarlo {

    // Call on the arithmetic average of S(t_1), ..., S(t_m).
    // No closed form; price by Monte Carlo.
    class ArithmeticAsianCall : public PathDepOption
    {
    public:
        ArithmeticAsianCall(double T_, double K_, int m_) : PathDepOption(T_, K_, m_) {}
        double Payoff(const SamplePath& S) const;
    };

    // Call on the geometric average of S(t_1), ..., S(t_m).
    // The geometric average of a GBM is lognormal, so it has a Black-Scholes
    // type closed form, which makes it the natural control variate for
    // ArithmeticAsianCall.
    class GeometricAsianCall : public PathDepOption
    {
    public:
        GeometricAsianCall(double T_, double K_, int m_) : PathDepOption(T_, K_, m_) {}
        double Payoff(const SamplePath& S) const;
        double PriceByBSFormula(const GBMModel& Model);
        double DeltaByBSFormula(const GBMModel& Model);
    };

}
}
