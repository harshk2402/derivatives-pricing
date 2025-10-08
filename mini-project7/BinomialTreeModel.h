#pragma once
#include <iostream>
#include <cmath>

namespace fre {

class BinomialTreeModel {
private:
    double S0;  
    double U;   
    double D;   
    double R;   

public:
    BinomialTreeModel() : S0(0.0), U(0.0), D(0.0), R(0.0) {}

    int GetInputData();

    int ValidateInputData() const;

    void UpdateBinomialTreeModel(double S0_, double U_, double D_, double R_);

    double RiskNeutProb() const;

    double CalculateAssetPrice(int n, int i) const;
    double GetS0() const { return S0; }
    double GetU() const { return U; }
    double GetD() const { return D; }
    double GetR() const { return R; }
};

} 
