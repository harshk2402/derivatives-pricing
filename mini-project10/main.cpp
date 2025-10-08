#include <iostream>
#include <iomanip>
#include "PathDepOption.h"

using namespace std;
using namespace fre;

int main()
{
    double S0 = 100.0, r = 0.03, sigma = 0.2;
    Model model(S0, r, sigma);

    double T = 1.0 / 12.0, K = 100.0;
    int m = 30;

    asianCall option(T, K, m);

    long N = 30000;
    double epsilon = 0.001;
    option.PriceByMC(model, N, epsilon);

    cout << fixed << setprecision(4);
    cout << "Arithmetic Asian Call by direct Monte Carlo = " << option.GetPrice() << endl
         << "Error = " << option.GetPricingError() << endl
         << "delta = " << option.GetDelta() << endl
         << "gamma = " << option.GetGamma() << endl;

    return 0;
}

/*
Arithmetic Asian Call by direct Monte Carlo = 1.4244
Error = 0.0120
delta = 0.5251
gamma = 0.1110
*/

