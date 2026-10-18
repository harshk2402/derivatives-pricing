#pragma once
#include "deriv/binomial/Option.h"

namespace deriv {
namespace binomial {

    // Double digital: pays 1 if K1 < S < K2 at expiry, otherwise 0.
    class DoubleDigital : public Option
    {
    private:
        double K1;
        double K2;

    public:
        DoubleDigital(int N_, double K1_, double K2_) : Option(N_), K1(K1_), K2(K2_) {}
        double Payoff(double z) const;
    };

}
}
