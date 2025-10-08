#include "BinomialTreeModel.h"
#include "Option02.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;
using namespace fre;

int main()
{
    double u = 1.15125, d = 0.86862, r = 1.00545;
    double s0 = 106.00;
    double k = 100.00;
    const unsigned int N = 8;

    vector<double> optionPrice = PriceByCRR(s0, u, d, r, N, k);
    
    cout << "Enter S0: " << s0 << endl;
    cout << "Enter U: " << u << endl;
    cout << "Enter D: " << d << endl;
    cout << "Enter R: " << r << endl << endl;

    if(r >= d && r <= u)
        cout << "Input data checked" << endl << "There is no arbitrage" << endl << endl;
    else return 0;

    cout << "Enter call option data: " << endl;
    cout << "Enter steps to expiry N: " << N << endl;
    cout << "Enter strike price K: " << k << endl;
    cout << endl;

    cout << "Digit Call option price = " << fixed << setprecision(2) << optionPrice[0] << endl;
    cout << "Digit Put option price = " << optionPrice[1] << endl;
    cout << "European Call option price = " << optionPrice[2] << endl;
    cout << "European Put option price = " << optionPrice[3] << endl;
    
    return 0;
}

/*
Enter S0: 106
Enter U: 1.15125
Enter D: 0.86862
Enter R: 1.00545

Input data checked
There is no arbitrage

Enter call option data: 
Enter steps to expiry N: 8
Enter strike price K: 100

Digit Call option price = 0.58
Digit Put option price = 0.38
European Call option price = 21.68
European Put option price = 11.43
*/