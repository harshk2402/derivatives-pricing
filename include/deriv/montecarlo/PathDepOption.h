#pragma once
#include "deriv/montecarlo/GBMModel.h"

namespace deriv {
namespace montecarlo {

    // Option whose payoff depends on the path S(t_1), ..., S(t_m), t_k = k T / m.
    // Results of the last pricing run are stored on the object and read back
    // through the getters.
    class PathDepOption
    {
    protected:
        double Price;
        double PricingError;
        double delta;
        double gamma;
        double K;
        double T;
        int m;

    public:
        PathDepOption(double T_, double K_, int m_)
            : Price(0.0), PricingError(0.0), delta(0.0), gamma(0.0), K(K_), T(T_), m(m_) {}
        virtual ~PathDepOption() {}

        virtual double Payoff(const SamplePath& S) const = 0;

        // Plain Monte Carlo over N paths. Sets the price, its standard error, and
        // delta and gamma by finite differences with every path rescaled by
        // (1 + epsilon) and (1 - epsilon), i.e. on the same random numbers.
        double PriceByMC(const GBMModel& Model, long N, double epsilon);

        // Control-variate Monte Carlo: simulates (this - CVOption) and adds back
        // CVOption's closed-form price and delta. Sets price, error and delta;
        // gamma keeps whatever PriceByMC last produced.
        double PriceByVarRedMC(const GBMModel& Model, long N, PathDepOption& CVOption, double epsilon);

        // closed-form price and delta, for options that have one
        virtual double PriceByBSFormula(const GBMModel& /*Model*/) { return 0.0; }
        virtual double DeltaByBSFormula(const GBMModel& /*Model*/) { return 0.0; }

        double GetT() const { return T; }
        double GetPrice() const { return Price; }
        double GetPricingError() const { return PricingError; }
        double GetDelta() const { return delta; }
        double GetGamma() const { return gamma; }
    };

    // Payoff of *Ptr1 minus payoff of *Ptr2; used as the control-variate estimator.
    class DifferenceOfOptions : public PathDepOption
    {
    private:
        PathDepOption* Ptr1;
        PathDepOption* Ptr2;

    public:
        DifferenceOfOptions(double T_, double K_, int m_, PathDepOption* Ptr1_, PathDepOption* Ptr2_)
            : PathDepOption(T_, K_, m_), Ptr1(Ptr1_), Ptr2(Ptr2_) {}
        double Payoff(const SamplePath& S) const
        {
            return Ptr1->Payoff(S) - Ptr2->Payoff(S);
        }
    };

}
}
