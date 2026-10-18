#include "deriv/numerics/Random.h"
#include <cmath>
#include <cstdlib>

namespace deriv {
namespace numerics {

    const double pi = 4.0 * std::atan(1.0);

    double Gauss()
    {
        double U1 = (std::rand() + 1.0) / (RAND_MAX + 1.0);
        double U2 = (std::rand() + 1.0) / (RAND_MAX + 1.0);
        return std::sqrt(-2.0 * std::log(U1)) * std::cos(2.0 * pi * U2);
    }

}
}
