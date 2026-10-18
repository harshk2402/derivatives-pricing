#pragma once

namespace deriv {
namespace numerics {

    // A real function together with its derivative, as consumed by NonlinearSolver.
    class Function
    {
    public:
        virtual double Value(double x) = 0;
        virtual double Deriv(double x) = 0;
        virtual ~Function() {}
    };

}
}
