// European options on a binomial tree, priced two independent ways:
//   - the closed-form CRR formula (a probability-weighted sum over terminal nodes)
//   - backward induction through the tree with a polymorphic payoff
// Both agree to machine precision; the pricer handles any Option subclass.

#include "deriv/binomial/BinomialPricer.h"
#include "deriv/binomial/BinomialTreeModel.h"
#include "deriv/binomial/CRRFormula.h"
#include "deriv/binomial/DigitalOption.h"
#include "deriv/binomial/Option.h"
#include "deriv/binomial/SpreadOption.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;
using namespace deriv::binomial;

namespace {
    void PrintRow(const string& name, const string& strikes, double price)
    {
        cout << "  " << left << setw(16) << name << setw(12) << strikes
             << right << setw(10) << price << endl;
    }
}

int main()
{
    int N = 8;
    double S0 = 106.0, U = 1.15125, D = 0.86862, R = 1.00545;
    double K = 100.0, K1 = 100.0, K2 = 110.0;

    cout << "Binomial model: S0 = " << S0 << ", U = " << U << ", D = " << D
         << ", R = " << R << ", N = " << N << " steps" << endl;

    BinomialTreeModel Model(S0, U, D, R);
    if (Model.ValidateInputData() != 0) return 1;

    cout << fixed << setprecision(4);

    cout << "Closed-form CRR formula" << endl;
    PrintRow("Call", "K=100", PriceByCRRFormula(S0, U, D, R, N, K, CallPayoff));
    PrintRow("Put", "K=100", PriceByCRRFormula(S0, U, D, R, N, K, PutPayoff));
    PrintRow("Digital call", "K=100", PriceByCRRFormula(S0, U, D, R, N, K, DigitCallPayoff));
    PrintRow("Digital put", "K=100", PriceByCRRFormula(S0, U, D, R, N, K, DigitPutPayoff));
    cout << endl;

    Call call(N, K);
    Put put(N, K);
    BullSpread bull(N, K1, K2);
    BearSpread bear(N, K1, K2);
    Strangle strangle(N, K1, K2);
    Butterfly butterfly(N, K1, K2);
    DoubleDigital doubleDigital(N, K1, K2);

    cout << "Backward induction" << endl;
    PrintRow("Call", "K=100", BinomialPricer(&call).PriceByCRR(Model));
    PrintRow("Put", "K=100", BinomialPricer(&put).PriceByCRR(Model));
    PrintRow("Bull spread", "K=100/110", BinomialPricer(&bull).PriceByCRR(Model));
    PrintRow("Bear spread", "K=100/110", BinomialPricer(&bear).PriceByCRR(Model));
    PrintRow("Strangle", "K=100/110", BinomialPricer(&strangle).PriceByCRR(Model));
    PrintRow("Butterfly", "K=100/110", BinomialPricer(&butterfly).PriceByCRR(Model));
    PrintRow("Double digital", "K=100/110", BinomialPricer(&doubleDigital).PriceByCRR(Model));
    cout << endl;

    // put-call parity on the tree: C - P = S0 - K / R^N
    double parity = BinomialPricer(&call).PriceByCRR(Model) - BinomialPricer(&put).PriceByCRR(Model);
    cout << "Put-call parity check: C - P = " << parity
         << ", S0 - K/R^N = " << S0 - K / pow(R, N) << endl;

    return 0;
}
