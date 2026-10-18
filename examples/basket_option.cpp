// Asian call on a basket of three correlated stocks, by Monte Carlo.
//
// Each stock follows dS_j = r S_j dt + S_j sum_l C[j][l] dW_l. The payoff is
// max(A - K, 0), where A is the basket value S_1 + S_2 + S_3 averaged over the
// m monitoring dates. Deltas with respect to each stock are finite differences
// computed on common random numbers.

#include "deriv/montecarlo/basket/GBMModel.h"
#include "deriv/montecarlo/basket/PathDepOption.h"
#include <iostream>

using namespace std;
using namespace deriv::montecarlo::basket;

int main()
{
    int d = 3;
    Vector S0(d);
    S0[0] = 40.0;
    S0[1] = 60.0;
    S0[2] = 100.0;
    double r = 0.03;

    Matrix C(d);
    for (int i = 0; i < d; i++) C[i].resize(d);
    C[0][0] =  0.1;  C[0][1] = -0.1;  C[0][2] = 0.0;
    C[1][0] = -0.1;  C[1][1] =  0.2;  C[1][2] = 0.0;
    C[2][0] =  0.0;  C[2][1] =  0.0;  C[2][2] = 0.3;
    GBMModel Model(S0, r, C);

    double T = 1.0 / 12.0, K = 200.0;
    int m = 30;
    ArithmeticAsianCall Option(T, K, m);

    cout << "Basket Asian call on 3 stocks: S0 = (40, 60, 100), K = 200, r = 3%, T = 1/12, m = 30" << endl;
    Vector sigma = Model.GetSigma();
    cout << "Volatilities: " << sigma[0] << ", " << sigma[1] << ", " << sigma[2] << endl << endl;

    long N = 30000;
    cout << "Price (N = " << N << ") = " << Option.PriceByMC(Model, N) << endl;

    Vector deltas = Option.DeltaByMC(Model, 0.001, 100000);
    for (int j = 0; j < (int)deltas.size(); j++)
        cout << "  delta[" << j << "] = " << deltas[j] << endl;

    return 0;
}
