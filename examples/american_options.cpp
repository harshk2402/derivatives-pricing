// American options by the Snell envelope.
//
// The tree is calibrated to Black-Scholes parameters (U = e^{sigma sqrt h},
// D = 1/U, R = e^{r h}). Without dividends an American call is never exercised
// early, so it is worth exactly the European call; an American put carries an
// early-exercise premium. The stopping tree marks the nodes (1) where
// exercising immediately is optimal.

#include "deriv/binomial/BinLattice.h"
#include "deriv/binomial/BinomialPricer.h"
#include "deriv/binomial/BinomialTreeModel.h"
#include "deriv/binomial/Option.h"
#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace deriv::binomial;

int main()
{
    double S0 = 106.0;
    double r = 0.058;
    double sigma = 0.46;
    double T = 9.0 / 12.0;
    double K = 100.0;
    int N = 8;

    double h = T / N;
    double U = exp(sigma * sqrt(h));
    double D = 1.0 / U;
    double R = exp(r * h);

    cout << setiosflags(ios::fixed) << setprecision(5);
    cout << "S0 = " << S0 << "   r = " << r << "   sigma = " << sigma
         << "   T = " << T << "   K = " << K << "   N = " << N << endl;
    cout << "U = " << U << "   D = " << D << "   R = " << R << endl << endl;

    BinomialTreeModel Model;
    Model.UpdateBinomialTreeModel(S0, U, D, R);

    Call call(N, K);
    Put put(N, K);
    BinomialPricer callPricer(&call);
    BinomialPricer putPricer(&put);

    BinLattice<double> PriceTree;
    BinLattice<bool> StoppingTree;

    double americanCall = callPricer.PriceBySnell(Model, PriceTree, StoppingTree);
    double europeanCall = callPricer.PriceByCRR(Model);
    cout << "American call = " << americanCall << "   European call = " << europeanCall
         << "   (no early exercise)" << endl;

    double americanPut = putPricer.PriceBySnell(Model, PriceTree, StoppingTree);
    double europeanPut = putPricer.PriceByCRR(Model);
    cout << "American put  = " << americanPut << "   European put  = " << europeanPut
         << "   premium = " << americanPut - europeanPut << endl << endl;

    cout << "American put value tree:" << endl;
    PriceTree.Display(cout, 9);
    cout << "American put stopping tree (1 = stop, 0 = continue; the last row is expiry):" << endl;
    StoppingTree.Display(cout, 3);

    return 0;
}
