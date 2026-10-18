#pragma once

namespace deriv {
namespace analytic {

    // Generalized Black-Scholes model for European calls and puts, with a
    // cost-of-carry parameter b (Haug):
    //     b = r        stock without dividends
    //     b = r - q    stock or index paying a continuous dividend yield q
    //     b = 0        option on a futures contract
    //     b = r - rf   currency option, rf the foreign rate
    //
    // Adapted from D. Duffy, "Introduction to C++ for Financial Engineers"
    // ((C) Datasim Component Technology BV 2003-2006). The option type is kept
    // as a manually managed C string, so the class implements the rule of three.
    class GeneralizedBlackScholes
    {
    private:
        double r;   // interest rate
        double sig; // volatility
        double K;   // strike price
        double T;   // time to expiry
        double U;   // current underlying price (e.g. stock, forward)
        double b;   // cost of carry
        char* optType; // "C" for a call, anything else for a put

        // 'kernel' functions for option calculations
        double CallPrice() const;
        double PutPrice() const;
        double CallDelta() const;
        double PutDelta() const;

        // Gaussian density and distribution functions
        double n(double x) const;
        double N(double x) const;

    public:
        GeneralizedBlackScholes(double r_, double sig_, double K_, double T_, double U_, double b_,
                                const char* optType_ = "C");
        GeneralizedBlackScholes(const GeneralizedBlackScholes& o2);
        virtual ~GeneralizedBlackScholes();

        GeneralizedBlackScholes& operator=(const GeneralizedBlackScholes& option2);

        // option price and sensitivities
        double Price() const;
        double Delta() const;

        const char* GetOptType() const { return optType; }

        // switches the option type (call <-> put)
        void Toggle();
    };

}
}
