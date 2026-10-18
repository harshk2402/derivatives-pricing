#pragma once
#include "deriv/numerics/Function.h"

namespace deriv {
namespace numerics {

    // Solves Fct->Value(x) = Tgt.
    //   bisection:      on the bracket [LEnd, REnd]
    //   Newton-Raphson: from the starting point Guess
    // Both stop once successive iterates are within Acc of each other.
    class NonlinearSolver
    {
    private:
        double Tgt;
        double LEnd;
        double REnd;
        double Acc;
        double Guess;

    public:
        NonlinearSolver() : Tgt(0), LEnd(0), REnd(0), Acc(0), Guess(0) {}
        NonlinearSolver(double Tgt_, double LEnd_, double REnd_, double Acc_, double Guess_)
            : Tgt(Tgt_), LEnd(LEnd_), REnd(REnd_), Acc(Acc_), Guess(Guess_) {}
        ~NonlinearSolver() {}
        void UpdateSolver(double Tgt_, double LEnd_, double REnd_, double Acc_, double Guess_)
        {
            Tgt = Tgt_;
            LEnd = LEnd_;
            REnd = REnd_;
            Acc = Acc_;
            Guess = Guess_;
        }
        double SolveByBisect(Function* Fct);
        double SolveByNR(Function* Fct);
    };

}
}
