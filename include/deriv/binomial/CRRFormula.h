#pragma once

namespace deriv {
namespace binomial {

    // Closed-form Cox-Ross-Rubinstein price of a European option:
    //
    //     H(0) = R^-N * sum_{i=0..N} C(N,i) q^i (1-q)^(N-i) h(S(N,i))
    //
    // a procedural alternative to BinomialPricer's backward induction.
    // The binomial coefficient is built from factorials held in a long, so N
    // must not exceed 20; use BinomialPricer for longer trees.

    // risk-neutral probability of an up move
    double RiskNeutProb(double U, double D, double R);

    // stock price at time step n after i up moves
    double CalculateAssetPrice(double S0, double U, double D, int n, int i);

    // payoffs h(z) with strike K
    double CallPayoff(double z, double K);
    double PutPayoff(double z, double K);
    double DigitCallPayoff(double z, double K);
    double DigitPutPayoff(double z, double K);

    double PriceByCRRFormula(double S0, double U, double D, double R, unsigned int N, double K,
                             double (*Payoff)(double z, double K) = CallPayoff);

}
}
