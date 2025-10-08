#include "BinomialTreeModel.h"
#include "BinLattice.h"
#include "Option.h"
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;
using namespace fre;

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
    cout << "S0 = " << S0 << endl;
    cout << "r  = " << r << endl;
    cout << "sigma = " << sigma << endl;
    cout << "T = " << T << endl;
    cout << "K = " << K << endl;
    cout << "N = " << N << endl;
    cout << endl;
    cout << "U = " << U << endl;
    cout << "D = " << D << endl;
    cout << "R = " << R << endl;
    cout << endl;

    BinomialTreeModel Model;
    Model.UpdateBinomialTreeModel(S0, U, D, R);

    Call call(N, K);
    OptionCalculation calc(&call);

    BinLattice<double> PriceTree;
    BinLattice<bool> StoppingTree;
    double price = calc.PriceBySnell(Model, PriceTree, StoppingTree);

    cout << "American call option price = " << price << endl;

    return 0;
}
