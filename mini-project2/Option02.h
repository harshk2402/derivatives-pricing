#pragma once
#include <vector>

using namespace std;

namespace fre {
	double DigitCallPayoff(double z, double K);

    double DigitPutPayoff(double z, double K);

    vector<double> PriceByCRR(double S0, double U, double D, double R, unsigned int N, double K);

    double CallPayoff(double z, double K);

    double PutPayoff(double z, double K);

}