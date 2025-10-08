#include "Function02.h"
#include <cmath>

namespace fre
{
    double F1::Value(double y) { 
        double sum = 0;

        for (int i = 1; i <= T; i++)
            sum += C * exp(-y * i);
        
        sum += F * exp(-y * T);

        return sum - P;
    }
    double F1::Deriv(double y) {
        double result = 0;

        for (int i = 1; i <= T; i++)
            result += -C * i * exp(-y * i);
        
        result += -F * exp(-y * T);

        return result;
    }
}