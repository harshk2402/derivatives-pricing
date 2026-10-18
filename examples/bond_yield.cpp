// Yield to maturity of a coupon bond: the continuously compounded rate y at
// which discounted coupons and face value equal the market price.

#include "deriv/fixedincome/BondYield.h"
#include "deriv/numerics/NonlinearSolver.h"
#include <iomanip>
#include <iostream>

using namespace std;
using namespace deriv;

int main()
{
    double F = 100.0;   // face value
    double T = 3.0;     // years to maturity
    double C = 1.2;     // annual coupon
    double P = 98.56;   // market price

    double Acc = 0.0001;
    double LEnd = 0.0;
    double REnd = 1.0;
    double Guess = 0.2;

    fixedincome::BondYield bond(F, T, C, P);
    numerics::NonlinearSolver solver(0.0, LEnd, REnd, Acc, Guess);

    double yBisect = solver.SolveByBisect(&bond);
    double yNR = solver.SolveByNR(&bond);

    cout << "3-year bond, face value " << F << ", annual coupon " << C << ", price " << P << endl;
    cout << fixed << setprecision(4);
    cout << "Yield by bisection:      " << yBisect << endl;
    cout << "Yield by Newton-Raphson: " << yNR << endl;
    cout << scientific << setprecision(2);
    cout << "Pricing error at the Newton-Raphson yield: " << bond.Value(yNR) << endl;

    return 0;
}
