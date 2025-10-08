#include "Option02.h"
#include "BinomialTreeModel.h"
#include <iostream>
#include <vector>
#include <cmath>

namespace fre{
    long int fact(int n) {
        if (n == 0 || n == 1) return 1;
        long int factorial = 1;
        for (int i = 2; i <= n; i++)
            factorial *= i;
        return factorial;
    }

    long double BinomialCoefficient(int N, int i) {
        return fact(N) / (fact(i) * fact(N - i));
    }

    std::vector<double> PriceByCRR(double S0, double U, double D, double R, unsigned int N, double K)
    {
        double q = RiskNeutProb(U, D, R);
        double price = 0.0;
        std::vector<double> prices;

        for (unsigned int i = 0; i <= N; i++) {
            double S_N_i = CalculateAssetPrice(S0, U, D, N, i);
            double payoff = DigitCallPayoff(S_N_i, K);
            double prob = BinomialCoefficient(N, i) * pow(q, i) * pow(1 - q, N - i);
            price += prob * payoff;
        }

        prices.push_back(price / pow(R, N));
        price = 0.0;

        for (unsigned int i = 0; i <= N; i++) {
            double S_N_i = CalculateAssetPrice(S0, U, D, N, i);
            double payoff = DigitPutPayoff(S_N_i, K);
            double prob = BinomialCoefficient(N, i) * pow(q, i) * pow(1 - q, N - i);
            price += prob * payoff;
        }

        prices.push_back(price / pow(R, N));
        price = 0.0;

        for (unsigned int i = 0; i <= N; i++) {
            double S_N_i = CalculateAssetPrice(S0, U, D, N, i);
            double payoff = CallPayoff(S_N_i, K);
            double prob = BinomialCoefficient(N, i) * pow(q, i) * pow(1 - q, N - i);
            price += prob * payoff;
        }

        prices.push_back(price / pow(R, N));
        price = 0.0;

        for (unsigned int i = 0; i <= N; i++) {
            double S_N_i = CalculateAssetPrice(S0, U, D, N, i);
            double payoff = PutPayoff(S_N_i, K);
            double prob = BinomialCoefficient(N, i) * pow(q, i) * pow(1 - q, N - i);
            price += prob * payoff;
        }

        prices.push_back(price / pow(R, N));
        price = 0.0;

        // Discount back to present
        return prices;
    }
    double DigitCallPayoff(double z, double K){
        return z > K ? 1.0 : 0.0;
    }

    double DigitPutPayoff(double z, double K){
        return z < K ? 1.0 : 0.0;
    }

    double CallPayoff(double z, double K) { 
        return std::max(z-K, 0.0);
    }

    double PutPayoff(double z, double K) { 
        return std::max(K-z, 0.0);
    }
}