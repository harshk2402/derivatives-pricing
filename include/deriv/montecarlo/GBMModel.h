#pragma once
#include <vector>

namespace deriv {
namespace montecarlo {

    // stock prices S(t_1), ..., S(t_m) at the monitoring dates t_k = k T / m
    typedef std::vector<double> SamplePath;

    // Black-Scholes model: geometric Brownian motion under the risk-neutral
    // measure, dS = r S dt + sigma S dW. Constructing a model seeds std::rand()
    // from the clock.
    class GBMModel
    {
    private:
        double S0, r, sigma;

    public:
        GBMModel() : S0(0.0), r(0.0), sigma(0.0) {}
        GBMModel(double S0_, double r_, double sigma_);

        // fills S (of size m) with an exact simulation of the path on [0, T]
        void GenerateSamplePath(double T, int m, SamplePath& S) const;

        double GetS0() const { return S0; }
        double GetR() const { return r; }
        double GetSigma() const { return sigma; }
        void SetS0(double S0_) { S0 = S0_; }
        void SetR(double r_) { r = r_; }
        void SetSigma(double sigma_) { sigma = sigma_; }
    };

}
}
