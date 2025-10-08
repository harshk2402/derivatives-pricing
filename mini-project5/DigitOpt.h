#pragma once
#include "Option.h"
namespace fre {
    class DigitOpt : public EurOption
    {
    private:
        double K1; 
        double K2; 
    public:
        DigitOpt(int N_, double K1_, double K2_) :
            EurOption(N_), K1(K1_), K2(K2_) {};
        double Payoff(double z) const;
    };
}
