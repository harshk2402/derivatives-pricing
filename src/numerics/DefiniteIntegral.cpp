#include "deriv/numerics/DefiniteIntegral.h"
#include <iostream>

namespace deriv {
namespace numerics {

    double DefiniteIntegral::ByTrapezoid(int N) const
    {
        double h = (b - a) / N;
        double sum = initial_sum;

        for (int i = 1; i < N; i++)
            sum += 2 * f(a + (i * h));

        return (h / 2) * sum;
    }

    double DefiniteIntegral::BySimpson(int N) const
    {
        double sum = initial_sum;

        if (N % 2 != 0) {
            std::cerr << "N needs to be even for the Simpson method" << std::endl;
            return 0.0;
        }

        double h = (b - a) / N;
        for (int i = 1; i < N; i++)
            sum += (i % 2 == 0 ? 2 : 4) * f(a + i * h);

        return (h / 3) * sum;
    }

}
}
