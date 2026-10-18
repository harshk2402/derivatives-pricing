// Closed-form European option prices.
//
// GeneralizedBlackScholes covers stocks, dividend-paying stocks, futures and
// currencies through the cost-of-carry b. The cases below are textbook
// examples from Hull, "Options, Futures, and Other Derivatives". A known cash
// dividend D paid at time t is handled by pricing on S0 - D e^{-r t}.

#include "deriv/analytic/BlackScholesCall.h"
#include "deriv/analytic/GeneralizedBlackScholes.h"
#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace deriv::analytic;

namespace {
    void Print(const char* description, const GeneralizedBlackScholes& option)
    {
        cout << "  " << left << setw(48) << description << right
             << (option.GetOptType()[0] == 'C' ? "call" : "put ")
             << "  price " << setw(9) << option.Price()
             << "  delta " << setw(9) << option.Delta() << endl;
    }
}

int main()
{
    cout << fixed << setprecision(5);

    // 3-month call on a stock: S = 60, K = 65, r = 8%, sigma = 30%
    double r = 0.08, sig = 0.30, K = 65.0, T = 0.25, S = 60.0;
    GeneralizedBlackScholes stockCall(r, sig, K, T, S, r);

    // 3-month put: S = 50, K = 50, r = 10%, sigma = 30%
    r = 0.10; K = 50.0; S = 50.0;
    GeneralizedBlackScholes stockPut(r, sig, K, T, S, r, "P");

    // the same put with a $1.50 dividend expected in two months
    double D = 1.5, tD = 2.0 / 12.0;
    GeneralizedBlackScholes dividendPut(r, sig, K, T, S - D * exp(-r * tD), r, "P");

    cout << "Generalized Black-Scholes" << endl;
    Print("Stock, S=60 K=65 r=8% vol=30% T=0.25", stockCall);
    Print("Stock, S=50 K=50 r=10% vol=30% T=0.25", stockPut);
    Print("Same, $1.50 dividend in 2 months", dividendPut);
    cout << endl;

    // switching call <-> put on a copy leaves the original untouched
    GeneralizedBlackScholes stockCall2(stockPut);
    stockCall2.Toggle();
    cout << "Put-call parity, S=50 K=50: C - P = " << stockCall2.Price() - stockPut.Price()
         << ", S - K e^{-rT} = " << S - K * exp(-r * T) << endl << endl;

    // b = 0 prices an option on a futures contract (Black's model)
    GeneralizedBlackScholes futuresCall(0.10, 0.25, 50.0, 0.5, 50.0, 0.0);
    Print("Futures, F=50 K=50 r=10% vol=25% T=0.5", futuresCall);
    cout << endl;

    // the plain Black-Scholes call uses a more accurate normal CDF approximation
    BlackScholesCall bsCall(0.25, 65.0);
    cout << "BlackScholesCall, S=60 K=65 r=8% vol=30% T=0.25:"
         << "  price " << bsCall.PriceByBSFormula(60.0, 0.30, 0.08)
         << "  delta " << bsCall.DeltaByBSFormula(60.0, 0.30, 0.08)
         << "  vega " << bsCall.VegaByBSFormula(60.0, 0.30, 0.08) << endl;

    return 0;
}
