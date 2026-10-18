#include "deriv/binomial/CRRFormula.h"
#include <algorithm>
#include <cmath>

namespace deriv {
namespace binomial {

    namespace {
        long int fact(int n)
        {
            if (n == 0 || n == 1) return 1;
            long int factorial = 1;
            for (int i = 2; i <= n; i++)
                factorial *= i;
            return factorial;
        }

        long double BinomialCoefficient(int N, int i)
        {
            return fact(N) / (fact(i) * fact(N - i));
        }
    }

    double RiskNeutProb(double U, double D, double R)
    {
        return (R - D) / (U - D);
    }

    double CalculateAssetPrice(double S0, double U, double D, int n, int i)
    {
        return S0 * std::pow(U, i) * std::pow(D, n - i);
    }

    double CallPayoff(double z, double K)
    {
        return std::max(z - K, 0.0);
    }

    double PutPayoff(double z, double K)
    {
        return std::max(K - z, 0.0);
    }

    double DigitCallPayoff(double z, double K)
    {
        return z > K ? 1.0 : 0.0;
    }

    double DigitPutPayoff(double z, double K)
    {
        return z < K ? 1.0 : 0.0;
    }

    double PriceByCRRFormula(double S0, double U, double D, double R, unsigned int N, double K,
                             double (*Payoff)(double z, double K))
    {
        double q = RiskNeutProb(U, D, R);
        double price = 0.0;

        for (unsigned int i = 0; i <= N; i++) {
            double S_N_i = CalculateAssetPrice(S0, U, D, N, i);
            double payoff = Payoff(S_N_i, K);
            double prob = BinomialCoefficient(N, i) * std::pow(q, i) * std::pow(1 - q, N - i);
            price += prob * payoff;
        }

        // discount back to the present
        return price / std::pow(R, N);
    }

}
}
