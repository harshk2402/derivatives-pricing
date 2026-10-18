#pragma once

namespace deriv {
namespace binomial {

    // Base class for options priced on a binomial tree. N is the number of
    // time steps to expiry; derived classes supply the payoff h(S_N).
    class Option
    {
    private:
        Option() : N(0) {}
        Option(const Option& option) : N(option.N) {}

    protected:
        int N;

    public:
        Option(int N_) : N(N_) {}
        int GetN() const { return N; }
        virtual double Payoff(double z) const = 0;
        virtual ~Option() = 0;
    };

    // max(S - K, 0)
    class Call : public Option
    {
    private:
        double K;

    public:
        Call(int N_, double K_) : Option(N_), K(K_) {}
        ~Call() {}
        double Payoff(double z) const;
    };

    // max(K - S, 0)
    class Put : public Option
    {
    private:
        double K;

    public:
        Put(int N_, double K_) : Option(N_), K(K_) {}
        ~Put() {}
        double Payoff(double z) const;
    };

}
}
