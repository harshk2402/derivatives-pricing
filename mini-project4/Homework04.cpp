// 9_Homework04.cpp
//
#include "BinomialTreeModel.h"
#include "Option.h"
#include <iostream>
#include <iomanip>
using namespace std;
using namespace fre;
int main()
{
    int N = 8;
    double U = 1.15125, D = 0.86862, R = 1.00545;
    double S0 = 106.00, K = 100.00;

    BinomialTreeModel BTM(S0, U, D, R);
    Call c(N, K);
    OptionCalculation calc(&c);

    cout << fixed << setprecision(2);

    cout << "European call option price = " << calc.PriceByCRR(BTM) << endl;

    Put p(N, K);
    OptionCalculation pCalc(&p);
    cout << "European put option price = " << pCalc.PriceByCRR(BTM) << endl;

    double K1 = 100, K2 = 110;
    BullSpread bull(N, K1, K2);
    OptionCalculation bullCalc(&bull);
    cout << "European bull spread option price = " << bullCalc.PriceByCRR(BTM) << endl;

    BearSpread bear(N, K1, K2);
    OptionCalculation bearCalc(&bear);
    cout << "European bear spread option price = " << bearCalc.PriceByCRR(BTM) << endl;

    return 0;
}

/*
European call option price = 21.68
European put option price = 11.43
European bull spread option price = 4.72
European bear spread option price = 4.86
*/
