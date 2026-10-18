#include "deriv/binomial/SpreadOption.h"

namespace deriv {
namespace binomial {

    double BullSpread::Payoff(double z) const
    {
        if (K2 <= z) return K2 - K1;
        else if (K1 < z) return z - K1;
        return 0.0;
    }

    double BearSpread::Payoff(double z) const
    {
        if (K2 <= z) return 0.0;
        else if (K1 < z) return K2 - z;
        return K2 - K1;
    }

    double Strangle::Payoff(double z) const
    {
        if (z <= K1) return K1 - z;
        else if (K2 < z) return z - K2;
        return 0.0;
    }

    double Butterfly::Payoff(double z) const
    {
        if (K1 < z && z <= (K1 + K2) / 2) return z - K1;
        else if ((K1 + K2) / 2 < z && z <= K2) return K2 - z;
        return 0.0;
    }

}
}
