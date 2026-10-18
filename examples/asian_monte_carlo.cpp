// Arithmetic Asian call by Monte Carlo, with and without a control variate.
//
// Direct Monte Carlo estimates price, delta and gamma (by rescaling each
// simulated path, so the bumped prices share random numbers). The control
// variate is the geometric Asian call, which has a closed form: simulating the
// difference between the two payoffs and adding back the exact geometric price
// cuts the standard error by roughly two orders of magnitude.

#include "deriv/montecarlo/AsianOptions.h"
#include "deriv/montecarlo/GBMModel.h"
#include <iomanip>
#include <iostream>

using namespace std;
using namespace deriv::montecarlo;

int main()
{
    double S0 = 100.0, r = 0.03, sigma = 0.2;
    GBMModel Model(S0, r, sigma);

    double T = 1.0 / 12.0, K = 100.0;
    int m = 30;             // daily monitoring over one month

    long N = 30000;         // sample paths
    double epsilon = 0.001; // relative bump for the Greeks

    ArithmeticAsianCall Option(T, K, m);
    GeometricAsianCall CVOption(T, K, m);

    cout << "Arithmetic Asian call: S0 = 100, K = 100, r = 3%, sigma = 20%, T = 1/12, m = 30, N = 30000"
         << endl << endl;
    cout << fixed << setprecision(5);

    Option.PriceByMC(Model, N, epsilon);
    cout << "Direct Monte Carlo" << endl
         << "  price = " << Option.GetPrice() << "   std error = " << Option.GetPricingError() << endl
         << "  delta = " << Option.GetDelta() << "   gamma = " << Option.GetGamma() << endl << endl;

    Option.PriceByVarRedMC(Model, N, CVOption, epsilon);
    cout << "Control variate (geometric Asian call)" << endl
         << "  price = " << Option.GetPrice() << "   std error = " << Option.GetPricingError() << endl
         << "  delta = " << Option.GetDelta() << endl << endl;

    cout << "Geometric Asian call, closed form: price = " << CVOption.PriceByBSFormula(Model)
         << "   delta = " << CVOption.DeltaByBSFormula(Model) << endl;

    return 0;
}
