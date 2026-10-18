#pragma once
#include "deriv/montecarlo/basket/GBMModel.h"

namespace deriv {
namespace montecarlo {
namespace basket {

    // Option whose payoff depends on the paths of several assets.
    class PathDepOption
    {
    protected:
        double Price;
        double T;
        double K;
        int m;

    public:
        PathDepOption(double T_, double K_, int m_) : Price(0.0), T(T_), K(K_), m(m_) {}
        virtual ~PathDepOption() {}

        virtual double Payoff(const SamplePath& S) const = 0;

        // plain Monte Carlo price over N paths
        double PriceByMC(const GBMModel& Model, long N);

        // delta with respect to each asset's initial price: bump S0[j] by a
        // factor (1 + epsilon) and reprice, reseeding std::rand() so every
        // price uses the same random numbers (common random numbers)
        Vector DeltaByMC(const GBMModel& Model, double epsilon, long N);

        double GetPrice() const { return Price; }
    };

    // Call on the time-average of the basket value sum_j S_j(t_k), k = 1..m.
    class ArithmeticAsianCall : public PathDepOption
    {
    public:
        ArithmeticAsianCall(double T_, double K_, int m_) : PathDepOption(T_, K_, m_) {}
        double Payoff(const SamplePath& S) const;
    };

}
}
}
