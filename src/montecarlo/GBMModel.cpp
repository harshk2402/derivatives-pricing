#include "deriv/montecarlo/GBMModel.h"
#include "deriv/numerics/Random.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

namespace deriv {
namespace montecarlo {

    GBMModel::GBMModel(double S0_, double r_, double sigma_) : S0(S0_), r(r_), sigma(sigma_)
    {
        std::srand((unsigned)std::time(NULL));
    }

    void GBMModel::GenerateSamplePath(double T, int m, SamplePath& S) const
    {
        double St = S0;
        for (int k = 0; k < m; k++)
        {
            S[k] = St * std::exp((r - sigma * sigma * 0.5) * (T / m) + sigma * std::sqrt(T / m) * numerics::Gauss());
            St = S[k];
        }
    }

}
}
