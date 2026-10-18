#include "deriv/binomial/Option.h"

namespace deriv {
namespace binomial {

    Option::~Option() {}

    double Call::Payoff(double z) const
    {
        return (z > K) ? z - K : 0.0;
    }

    double Put::Payoff(double z) const
    {
        return (z < K) ? K - z : 0.0;
    }

}
}
