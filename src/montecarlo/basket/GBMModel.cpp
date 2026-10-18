#include "deriv/montecarlo/basket/GBMModel.h"
#include "deriv/numerics/Random.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

namespace deriv {
namespace montecarlo {
namespace basket {

    using namespace numerics;

    namespace {
        // vector of d independent N(0,1) draws
        Vector Gauss(int d)
        {
            Vector Z(d);
            for (int j = 0; j < d; ++j) {
                Z[j] = numerics::Gauss();
            }
            return Z;
        }
    }

    GBMModel::GBMModel(Vector S0_, double r_, Matrix C_)
        : S0(S0_), C(C_), r(r_)
    {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        int d = static_cast<int>(S0.size());
        sigma.resize(d);
        for (int j = 0; j < d; ++j) {
            sigma[j] = std::sqrt(C[j] ^ C[j]);  // length of row j
        }
    }

    void GBMModel::GenerateSamplePath(double T, int m, SamplePath& S) const
    {
        int d = static_cast<int>(S0.size());
        S.resize(m);
        Vector St = S0;
        double dt = T / m;
        for (int k = 0; k < m; ++k) {
            Vector Z = Gauss(d);
            // drift + diffusion term for each component
            Vector drift(d);
            Vector CZ = C * Z;
            for (int j = 0; j < d; ++j) {
                drift[j] = (r - 0.5 * sigma[j] * sigma[j]) * dt + std::sqrt(dt) * CZ[j];
            }
            // element-wise exponential and product
            Vector factor = numerics::exp(drift);
            St = St * factor;
            S[k] = St;
        }
    }

}
}
}
