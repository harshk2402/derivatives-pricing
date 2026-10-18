// One European call, priced with every method in the library.
//
//   S0 = 106, K = 100, r = 5.8%, sigma = 46%, T = 0.75
//
// The binomial methods use a tree calibrated to the same parameters
// (U = e^{sigma sqrt h}, D = 1/U, R = e^{r h}); they agree with each other
// exactly at a given N and approach Black-Scholes as N grows. Monte Carlo
// agrees within its standard error.

#include "deriv/analytic/BlackScholesCall.h"
#include "deriv/analytic/GeneralizedBlackScholes.h"
#include "deriv/analytic/ImpliedVolatility.h"
#include "deriv/binomial/BinLattice.h"
#include "deriv/binomial/BinomialPricer.h"
#include "deriv/binomial/BinomialTreeModel.h"
#include "deriv/binomial/CRRFormula.h"
#include "deriv/binomial/Option.h"
#include "deriv/montecarlo/GBMModel.h"
#include "deriv/montecarlo/PathDepOption.h"
#include "deriv/numerics/NonlinearSolver.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;
using namespace deriv;

namespace {
    // A European call is the simplest path-dependent option: its payoff only
    // looks at the last point of the path.
    class EuropeanCall : public montecarlo::PathDepOption
    {
    public:
        EuropeanCall(double T_, double K_, int m_) : PathDepOption(T_, K_, m_) {}
        double Payoff(const montecarlo::SamplePath& S) const
        {
            return S[m - 1] > K ? S[m - 1] - K : 0.0;
        }
    };

    binomial::BinomialTreeModel CalibratedTree(double S0, double r, double sigma, double T, int N)
    {
        double h = T / N;
        double U = exp(sigma * sqrt(h));
        return binomial::BinomialTreeModel(S0, U, 1.0 / U, exp(r * h));
    }

    double reference = 0.0;

    void Row(const string& method, double price, const string& note = "")
    {
        cout << "  " << left << setw(44) << method << right << setw(10) << price
             << setw(11) << price - reference << "   " << note << endl;
    }
}

int main()
{
    double S0 = 106.0, K = 100.0, r = 0.058, sigma = 0.46, T = 0.75;

    cout << fixed << setprecision(5);
    cout << "European call: S0 = 106, K = 100, r = 5.8%, sigma = 46%, T = 0.75" << endl << endl;
    cout << "  " << left << setw(44) << "Method" << right << setw(10) << "Price" << setw(11) << "vs BS" << endl;

    // closed form
    analytic::BlackScholesCall bs(T, K);
    reference = bs.PriceByBSFormula(S0, sigma, r);
    Row("Black-Scholes formula", reference);

    analytic::GeneralizedBlackScholes gbs(r, sigma, K, T, S0, r);
    Row("Generalized Black-Scholes (b = r)", gbs.Price(), "coarser normal CDF");

    // binomial tree, N = 20 (the largest N the factorial-based formula supports)
    int N = 20;
    binomial::BinomialTreeModel tree = CalibratedTree(S0, r, sigma, T, N);
    binomial::Call call(N, K);
    binomial::BinomialPricer pricer(&call);

    Row("CRR formula, N = 20",
        binomial::PriceByCRRFormula(tree.GetS0(), tree.GetU(), tree.GetD(), tree.GetR(), N, K));
    Row("Backward induction, N = 20", pricer.PriceByCRR(tree));

    binomial::BinLattice<double> X, Y;
    pricer.PriceByCRR(tree, X, Y);
    Row("Cost of replicating portfolio, N = 20", X.GetNode(0, 0) * S0 + Y.GetNode(0, 0), "X*S0 + Y");

    binomial::BinLattice<double> PriceTree;
    binomial::BinLattice<bool> StoppingTree;
    Row("American call (Snell envelope), N = 20", pricer.PriceBySnell(tree, PriceTree, StoppingTree),
        "never exercised early");

    int Nfine = 2000;
    binomial::BinomialTreeModel fineTree = CalibratedTree(S0, r, sigma, T, Nfine);
    binomial::Call fineCall(Nfine, K);
    double finePrice = binomial::BinomialPricer(&fineCall).PriceByCRR(fineTree);
    Row("Backward induction, N = 2000", finePrice);

    // Monte Carlo
    montecarlo::GBMModel gbm(S0, r, sigma);
    EuropeanCall mcCall(T, K, 1);
    mcCall.PriceByMC(gbm, 1000000, 0.001);
    Row("Monte Carlo, 1,000,000 paths", mcCall.GetPrice(),
        "+/- " + to_string(mcCall.GetPricingError()) + " (1 s.e.)");
    cout << endl;

    // Greeks side by side
    cout << "  Delta: Black-Scholes " << bs.DeltaByBSFormula(S0, sigma, r)
         << ", binomial hedge X(0,0) " << X.GetNode(0, 0)
         << ", Monte Carlo " << mcCall.GetDelta() << endl;

    // invert the fine binomial price back to a volatility
    analytic::ImpliedVolatility iv(T, K, S0, r);
    numerics::NonlinearSolver solver(finePrice, 0.01, 2.0, 1e-8, 0.3);
    cout << "  Volatility implied by the N = 2000 binomial price: " << solver.SolveByNR(&iv)
         << " (true 0.46)" << endl;

    return 0;
}
