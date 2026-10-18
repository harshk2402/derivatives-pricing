#pragma once
#include "deriv/numerics/Function.h"

namespace deriv {
namespace fixedincome {

    // Pricing error of a bond paying annual coupons C and face value F at
    // maturity T, under continuous compounding at yield y:
    //
    //     f(y) = sum_{i=1..T} C e^{-y i} + F e^{-y T} - P
    //
    // Its root is the yield to maturity implied by the market price P; solve it
    // with numerics::NonlinearSolver (target 0).
    class BondYield : public numerics::Function
    {
    private:
        double F;   // face value
        double T;   // maturity in years
        double C;   // annual coupon
        double P;   // market price

    public:
        BondYield(double F_, double T_, double C_, double P_) : F(F_), T(T_), C(C_), P(P_) {}
        double Value(double y);
        double Deriv(double y);
    };

}
}
