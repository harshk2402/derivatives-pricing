#include "MCModel02.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

namespace fre {

    const double pi = 4.0 * std::atan(1.0);

    // Standard normal generator via Box–Muller
    double Gauss()
    {
        double U1 = (std::rand() + 1.0) / (RAND_MAX + 1.0);
        double U2 = (std::rand() + 1.0) / (RAND_MAX + 1.0);
        return std::sqrt(-2.0 * std::log(U1)) * std::cos(2.0 * pi * U2);
    }

    // Generate a vector of d independent N(0,1)
    Vector Gauss(int d)
    {
        Vector Z(d);
        for (int j = 0; j < d; ++j) {
            Z[j] = Gauss();
        }
        return Z;
    }

    // Constructor: initialize parameters and seed RNG
    MCModel::MCModel(Vector S0_, double r_, Matrix C_)
        : S0(S0_), r(r_), C(C_)
    {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        int d = static_cast<int>(S0.size());
        sigma.resize(d);
        for (int j = 0; j < d; ++j) {
            sigma[j] = std::sqrt(C[j] ^ C[j]);  // length of row j
        }
    }

    // Generate one sample path of length m (m time steps) at times spaced T/m
    void MCModel::GenerateSamplePath(double T, int m, SamplePath& S) const
    {
        int d = static_cast<int>(S0.size());
        S.resize(m);
        Vector St = S0;
        double dt = T / m;
        for (int k = 0; k < m; ++k) {
            Vector Z = Gauss(d);
            // compute drift + diffusion term for each component
            Vector drift(d);
            Vector CZ = C * Z;
            for (int j = 0; j < d; ++j) {
                drift[j] = (r - 0.5 * sigma[j] * sigma[j]) * dt + std::sqrt(dt) * CZ[j];
            }
            // elementwise exponential and multiply
            Vector factor = fre::exp(drift);
            St = St * factor;  // elementwise multiply
            S[k] = St;
        }
    }

} // namespace fre
