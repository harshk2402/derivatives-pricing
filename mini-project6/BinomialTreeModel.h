#pragma once
#include <iostream>
#include <cmath>

namespace fre {

class BinomialTreeModel {
private:
    double S0;  // Initial stock price
    double U;   // Up factor
    double D;   // Down factor
    double R;   // Risk-free interest rate per period

public:
    BinomialTreeModel() : S0(0.0), U(0.0), D(0.0), R(0.0) {}

    // Function to get data from user
    int GetInputData();

    // Function to check for validity and arbitrage
    int ValidateInputData() const;

    // Function to manually set the data
    void UpdateBinomialTreeModel(double S0_, double U_, double D_, double R_);

    // Function to calculate the risk-neutral probability
    double RiskNeutProb() const;

    // Function to calculate asset price at node (n, i)
    double CalculateAssetPrice(int n, int i) const;

    // Getters if needed
    double GetS0() const { return S0; }
    double GetU() const { return U; }
    double GetD() const { return D; }
    double GetR() const { return R; }
};

} // namespace fre
