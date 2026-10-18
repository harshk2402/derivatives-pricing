// The replicating strategy behind a binomial price.
//
// At every node (n, i) the pricer records the portfolio of X shares of stock
// and Y in the money market that, held to step n+1, reproduces the option's
// value in both the up and the down state. Its cost at t = 0 is the option
// price, so X(0,0) * S0 + Y(0,0) must equal the price exactly.
//
// Usage: replicating_portfolio [output-file]   (defaults to standard output)

#include "deriv/binomial/BinLattice.h"
#include "deriv/binomial/BinomialPricer.h"
#include "deriv/binomial/BinomialTreeModel.h"
#include "deriv/binomial/Option.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;
using namespace deriv::binomial;

namespace {
    void Replicate(ostream& out, const string& name, Option& option, const BinomialTreeModel& Model)
    {
        BinLattice<double> X, Y;
        double price = BinomialPricer(&option).PriceByCRR(Model, X, Y);

        out << fixed << setprecision(3);
        out << name << " price: " << price << endl;
        out << "Replicating cost at t=0: X*S0 + Y = "
            << X.GetNode(0, 0) * Model.GetS0() + Y.GetNode(0, 0) << endl << endl;

        out << "Stock positions X(n,i):" << endl;
        X.Display(out, 10);
        out << "Money market positions Y(n,i):" << endl;
        Y.Display(out, 10);
    }
}

int main(int argc, char* argv[])
{
    int N = 8;
    double K = 100.0;
    BinomialTreeModel Model(106.0, 1.15125, 0.86862, 1.00545);
    if (Model.ValidateInputData() != 0) return 1;

    ofstream file;
    if (argc > 1)
    {
        file.open(argv[1]);
        if (!file)
        {
            cerr << "Cannot open " << argv[1] << " for writing" << endl;
            return 1;
        }
    }
    ostream& out = (argc > 1) ? static_cast<ostream&>(file) : cout;

    Call call(N, K);
    Put put(N, K);
    Replicate(out, "European call", call, Model);
    Replicate(out, "European put", put, Model);

    if (argc > 1) cout << "Results written to " << argv[1] << endl;
    return 0;
}
