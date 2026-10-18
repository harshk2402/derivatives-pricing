#include "deriv/fixedincome/BondYield.h"
#include <cmath>

namespace deriv {
namespace fixedincome {

    double BondYield::Value(double y)
    {
        double sum = 0;

        for (int i = 1; i <= T; i++)
            sum += C * std::exp(-y * i);

        sum += F * std::exp(-y * T);

        return sum - P;
    }

    double BondYield::Deriv(double y)
    {
        double result = 0;

        for (int i = 1; i <= T; i++)
            result += -C * i * std::exp(-y * i);

        result += -F * T * std::exp(-y * T);

        return result;
    }

}
}
