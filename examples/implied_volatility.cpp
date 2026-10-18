// Implied volatility: the sigma at which the Black-Scholes price matches an
// observed option price. ImpliedVolatility exposes the price as a function of
// sigma (with vega as its derivative), so the generic NonlinearSolver can
// invert it by bisection or by Newton-Raphson.

#include "deriv/analytic/ImpliedVolatility.h"
#include "deriv/numerics/NonlinearSolver.h"
#include <iomanip>
#include <iostream>

using namespace std;
using namespace deriv;

int main()
{
    double S0 = 106.0, r = 0.058, T = 0.75, K = 100.0;

    analytic::ImpliedVolatility iv(T, K, S0, r);

    double Acc = 1e-6, LEnd = 0.01, REnd = 2.0, Guess = 0.3;

    cout << fixed << setprecision(6);
    cout << "European call, S0 = 106, K = 100, r = 5.8%, T = 0.75" << endl << endl;
    cout << setw(14) << "market price" << setw(14) << "bisection" << setw(16) << "Newton-Raphson" << endl;

    double quotes[] = { 12.0, 16.5, 21.5637535, 27.0, 35.0 };
    for (double price : quotes)
    {
        numerics::NonlinearSolver solver(price, LEnd, REnd, Acc, Guess);
        cout << setw(14) << price
             << setw(14) << solver.SolveByBisect(&iv)
             << setw(16) << solver.SolveByNR(&iv) << endl;
    }

    cout << endl << "(a price of 21.5637535 corresponds to sigma = 46%)" << endl;
    return 0;
}
