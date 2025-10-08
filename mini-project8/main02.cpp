#include <iostream>
#include <iomanip>
#include "Function02.h"
#include "NonlinearSolver02.h"
using namespace std;
using namespace fre;
int main()
{
     double Acc = 0.0001;
     double LEnd = 0.0;
     double REnd = 1.0;
     double Guess = 0.2;
     double Tgt = 0.0;

     NonlinearSolver solver(Tgt, LEnd, REnd, Acc, Guess);
     F1 MyF1;
     cout << fixed << setprecision(4);
     cout << "Yield by bisection method: " << solver.SolveByBisect(&MyF1) << endl;
     cout << "Yield by Newton-Raphson method: " << solver.SolveByNR(&MyF1) << endl;
     return 0;
}
/*
Yield by bisection method: 0.0168
Yield by Newton-Raphson method: 0.0168
*/