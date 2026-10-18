// Adapted from D. Duffy, "Introduction to C++ for Financial Engineers"
// (C) Datasim Component Technology BV 2003

#include "deriv/analytic/GeneralizedBlackScholes.h"
#include <cmath>
#include <cstring>

namespace deriv {
namespace analytic {

    namespace {
        char* CopyString(const char* s)
        {
            char* copy = new char[std::strlen(s) + 1];
            std::strcpy(copy, s);
            return copy;
        }
    }

    //////////// Gaussian functions /////////////////////////////////

    double GeneralizedBlackScholes::n(double x) const
    {
        double A = 1.0 / std::sqrt(2.0 * 3.1415);
        return A * std::exp(-x * x * 0.5);
    }

    double GeneralizedBlackScholes::N(double x) const
    { // the approximation to the cumulative normal distribution

        double a1 = 0.4361836;
        double a2 = -0.1201676;
        double a3 = 0.9372980;

        double k = 1.0 / (1.0 + (0.33267 * x));

        if (x >= 0.0)
        {
            return 1.0 - n(x) * (a1 * k + (a2 * k * k) + (a3 * k * k * k));
        }
        else
        {
            return 1.0 - N(-x);
        }
    }

    // Kernel functions (Haug)
    double GeneralizedBlackScholes::CallPrice() const
    {
        double tmp = sig * std::sqrt(T);

        double d1 = (std::log(U / K) + (b + (sig * sig) * 0.5) * T) / tmp;
        double d2 = d1 - tmp;

        return (U * std::exp((b - r) * T) * N(d1)) - (K * std::exp(-r * T) * N(d2));
    }

    double GeneralizedBlackScholes::PutPrice() const
    {
        double tmp = sig * std::sqrt(T);

        double d1 = (std::log(U / K) + (b + (sig * sig) * 0.5) * T) / tmp;
        double d2 = d1 - tmp;

        return (K * std::exp(-r * T) * N(-d2)) - (U * std::exp((b - r) * T) * N(-d1));
    }

    double GeneralizedBlackScholes::CallDelta() const
    {
        double tmp = sig * std::sqrt(T);

        double d1 = (std::log(U / K) + (b + (sig * sig) * 0.5) * T) / tmp;

        return std::exp((b - r) * T) * N(d1);
    }

    double GeneralizedBlackScholes::PutDelta() const
    {
        double tmp = sig * std::sqrt(T);

        double d1 = (std::log(U / K) + (b + (sig * sig) * 0.5) * T) / tmp;

        return std::exp((b - r) * T) * (N(d1) - 1.0);
    }

    GeneralizedBlackScholes::GeneralizedBlackScholes(double r_, double sig_, double K_, double T_, double U_,
                                                     double b_, const char* optType_)
        : r(r_), sig(sig_), K(K_), T(T_), U(U_), b(b_), optType(CopyString(optType_))
    {
    }

    GeneralizedBlackScholes::GeneralizedBlackScholes(const GeneralizedBlackScholes& o2)
        : r(o2.r), sig(o2.sig), K(o2.K), T(o2.T), U(o2.U), b(o2.b), optType(CopyString(o2.optType))
    {
    }

    GeneralizedBlackScholes::~GeneralizedBlackScholes()
    {
        delete[] optType;
    }

    GeneralizedBlackScholes& GeneralizedBlackScholes::operator=(const GeneralizedBlackScholes& option2)
    {
        if (this == &option2)
            return *this;

        // allocate before releasing, so a failed allocation leaves *this intact
        char* newType = CopyString(option2.optType);
        delete[] optType;
        optType = newType;

        r = option2.r;
        sig = option2.sig;
        K = option2.K;
        T = option2.T;
        U = option2.U;
        b = option2.b;

        return *this;
    }

    // Functions that calculate option price and sensitivities
    double GeneralizedBlackScholes::Price() const
    {
        if (std::strcmp(optType, "C") == 0)
            return CallPrice();
        else
            return PutPrice();
    }

    double GeneralizedBlackScholes::Delta() const
    {
        if (std::strcmp(optType, "C") == 0)
            return CallDelta();
        else
            return PutDelta();
    }

    // Modifier functions
    void GeneralizedBlackScholes::Toggle()
    { // change option type (C/P, P/C)

        char* newType = CopyString(std::strcmp(optType, "C") == 0 ? "P" : "C");
        delete[] optType;
        optType = newType;
    }

}
}
