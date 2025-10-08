#include <iostream>
#include <iomanip>
#include "BinomialTreeModel.h"
#include "Option.h"
#include "DigitOpt.h"
#include "SpreadOption.h"

using namespace std;
using namespace fre;
int main()
{
    int N = 8;
    double U = 1.15125, D = 0.86862, R = 1.00545;
    double S0 = 106.00, K1 = 100.00, K2 = 110.00;

    BinomialTreeModel BTM(S0, U, D, R);

    DigitOpt digitOpt(N, K1, K2);
    OptionCalculation digitCalc(&digitOpt);

    cout << fixed << setprecision(2);

    cout << "European double digit option price = " << digitCalc.PriceByCRR(BTM) << endl;

    StrangleOpt strangleOpt(N, K1, K2);
    OptionCalculation strangleCalc(&strangleOpt);
    cout << "European strangle option price = " << strangleCalc.PriceByCRR(BTM) << endl;

    ButterflyOpt butterflyOpt(N, K1, K2);
    OptionCalculation butterflyCalc(&butterflyOpt);
    cout << "European butterfly option price = " << butterflyCalc.PriceByCRR(BTM) << endl;

    return 0;
}

/*
European double digit option price = 0.26
European strangle option price = 28.39
European butterfly option price = 1.04
*/
