#pragma once
#include "deriv/binomial/Option.h"

namespace deriv {
namespace binomial {

    // Combination payoffs on two strikes K1 < K2.

    // long call at K1, short call at K2
    class BullSpread : public Option
    {
    private:
        double K1;
        double K2;

    public:
        BullSpread(int N_, double K1_, double K2_) : Option(N_), K1(K1_), K2(K2_) {}
        double Payoff(double z) const;
    };

    // long put at K2, short put at K1
    class BearSpread : public Option
    {
    private:
        double K1;
        double K2;

    public:
        BearSpread(int N_, double K1_, double K2_) : Option(N_), K1(K1_), K2(K2_) {}
        double Payoff(double z) const;
    };

    // long put at K1, long call at K2
    class Strangle : public Option
    {
    private:
        double K1;
        double K2;

    public:
        Strangle(int N_, double K1_, double K2_) : Option(N_), K1(K1_), K2(K2_) {}
        double Payoff(double z) const;
    };

    // tent-shaped payoff peaking at (K1 + K2) / 2
    class Butterfly : public Option
    {
    private:
        double K1;
        double K2;

    public:
        Butterfly(int N_, double K1_, double K2_) : Option(N_), K1(K1_), K2(K2_) {}
        double Payoff(double z) const;
    };

}
}
