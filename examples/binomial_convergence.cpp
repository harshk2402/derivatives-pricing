// Convergence of the binomial price to Black-Scholes.
//
// With U = e^{sigma sqrt h}, D = 1/U, R = e^{r h} and h = T/N, the CRR price
// of a European call converges to the Black-Scholes price as N grows. The
// error oscillates (it depends on where the strike falls between terminal
// nodes) while shrinking roughly like 1/N.

#include "deriv/analytic/BlackScholesCall.h"
#include "deriv/binomial/BinomialPricer.h"
#include "deriv/binomial/BinomialTreeModel.h"
#include "deriv/binomial/Option.h"
#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace deriv;

int main()
{
    double S0 = 106.0, r = 0.058, sigma = 0.46, T = 0.75, K = 100.0;

    analytic::BlackScholesCall bs(T, K);
    double exact = bs.PriceByBSFormula(S0, sigma, r);

    cout << fixed << setprecision(6);
    cout << "European call, S0 = 106, K = 100, r = 5.8%, sigma = 46%, T = 0.75" << endl;
    cout << "Black-Scholes price: " << exact << endl << endl;
    cout << setw(8) << "N" << setw(14) << "CRR price" << setw(14) << "error" << setw(14) << "N * error" << endl;

    for (int N = 8; N <= 4096; N *= 2)
    {
        double h = T / N;
        double U = exp(sigma * sqrt(h));
        binomial::BinomialTreeModel Model(S0, U, 1.0 / U, exp(r * h));
        binomial::Call call(N, K);
        double price = binomial::BinomialPricer(&call).PriceByCRR(Model);
        cout << setw(8) << N << setw(14) << price << setw(14) << price - exact
             << setw(14) << N * (price - exact) << endl;
    }

    return 0;
}
