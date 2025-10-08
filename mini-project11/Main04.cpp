#include <iostream>
#include <iomanip>
#include "PathDepOption.h"
#include "GmtrAsianCall.h"

using namespace std;
using namespace fre;

int main()
{
    double S0 = 100.0, r = 0.03, sigma = 0.2;
    MCModel Model(S0, r, sigma);

    double T = 1.0 / 12.0, K = 100.0;
    int m = 30;

    ArthmAsianCall Option(T, K, m);
    GmtrAsianCall CVOption(T, K, m);

    long N = 30000;
    double epsilon = 0.001;

    Option.PriceByVarRedMC(Model, N, CVOption, epsilon);
    cout << fixed << setprecision(4);
    cout << "Arithmetic call price by Control Variate = " << Option.GetPrice() << endl
         << "Error = " << Option.GetPricingError() << endl
         << "Delta with Control Variate = " << Option.GetDelta() << endl << endl;

    Option.PriceByMC(Model, N, epsilon);
    cout << "Price by direct MC = " << Option.GetPrice() << endl
         << "Error = " << Option.GetPricingError() << endl << endl;

    return 0;
}
/*
Arithmetic call price by Control Variate = 1.4261
Error = 0.0001
Delta with Control Variate = 0.5199

Price by direct MC = 1.4106
Error = 0.0119
*/

