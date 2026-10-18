#pragma once
#include "deriv/numerics/Matrix.h"
#include <vector>

namespace deriv {
namespace montecarlo {
namespace basket {

    using numerics::Vector;
    using numerics::Matrix;

    // S[k][j] is the price of asset j at the monitoring date t_{k+1} = (k+1) T / m
    typedef std::vector<Vector> SamplePath;

    // d correlated assets following geometric Brownian motion under the
    // risk-neutral measure:
    //
    //     dS_j = r S_j dt + S_j sum_l C[j][l] dW_l
    //
    // with W a d-dimensional standard Brownian motion. Row j of C gives asset
    // j's exposure to each driver, so sigma_j = |C[j]|. Constructing a model
    // seeds std::rand() from the clock.
    class GBMModel
    {
    private:
        Vector S0, sigma;
        Matrix C;
        double r;

    public:
        GBMModel(Vector S0_, double r_, Matrix C_);
        void GenerateSamplePath(double T, int m, SamplePath& S) const;
        Vector GetS0() const { return S0; }
        Vector GetSigma() const { return sigma; }
        Matrix GetC() const { return C; }
        double GetR() const { return r; }
        void SetS0(const Vector& S0_) { S0 = S0_; }
        void SetSigma(const Vector& sigma_) { sigma = sigma_; }
        void SetC(const Matrix& C_) { C = C_; }
        void SetR(double r_) { r = r_; }
    };

}
}
}
