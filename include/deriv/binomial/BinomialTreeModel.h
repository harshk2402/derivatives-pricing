#pragma once

namespace deriv {
namespace binomial {

    // Cox-Ross-Rubinstein binomial tree: each period the stock moves
    // S -> S*U or S -> S*D, and the money market grows by R.
    class BinomialTreeModel
    {
    private:
        double S0;  // initial stock price
        double U;   // up factor
        double D;   // down factor
        double R;   // money-market growth factor per period

    public:
        BinomialTreeModel() : S0(0), U(0), D(0), R(0) {}
        BinomialTreeModel(double S0_, double U_, double D_, double R_) : S0(S0_), U(U_), D(D_), R(R_) {}
        BinomialTreeModel(const BinomialTreeModel& B) : S0(B.S0), U(B.U), D(B.D), R(B.R) {}

        // risk-neutral probability of an up move, q = (R - D) / (U - D)
        double RiskNeutProb() const;

        // stock price at time step n after i up moves
        double CalculateAssetPrice(int n, int i) const;

        void UpdateBinomialTreeModel(double S0_, double U_, double D_, double R_);

        // checks S0 > 0, U > D > 0, R > 0 and absence of arbitrage; 0 on success
        int ValidateInputData() const;

        // reads S0, U, D, R from standard input, then validates them
        int GetInputData();

        double GetS0() const { return S0; }
        double GetU() const { return U; }
        double GetD() const { return D; }
        double GetR() const { return R; }
    };

}
}
