#include "deriv/montecarlo/basket/PathDepOption.h"
#include <cmath>
#include <cstdlib>

namespace deriv {
namespace montecarlo {
namespace basket {

    using namespace numerics;

    double PathDepOption::PriceByMC(const GBMModel& Model, long N)
    {
        double H = 0.0;
        SamplePath S(m);

        for (long i = 0; i < N; i++)
        {
            Model.GenerateSamplePath(T, m, S);
            H = (i * H + Payoff(S)) / (i + 1.0);
        }
        Price = std::exp(-Model.GetR() * T) * H;
        return Price;
    }

    Vector PathDepOption::DeltaByMC(const GBMModel& Model, double epsilon, long N)
    {
        int d = static_cast<int>(Model.GetS0().size());
        Vector delta(d);
        // fixed seed so every price below sees the same sample paths
        std::srand(0);
        double basePrice = PriceByMC(Model, N);
        for (int j = 0; j < d; ++j) {
            Vector S0_eps = Model.GetS0();
            S0_eps[j] *= (1.0 + epsilon);
            GBMModel model_eps(S0_eps, Model.GetR(), Model.GetC());
            std::srand(0);
            double price_eps = PriceByMC(model_eps, N);
            delta[j] = (price_eps - basePrice) / (epsilon * Model.GetS0()[j]);
        }
        return delta;
    }

    double ArithmeticAsianCall::Payoff(const SamplePath& S) const
    {
        double Ave = 0.0;
        int d = S[0].size();
        Vector one(d);
        for (int i = 0; i < d; i++) one[i] = 1.0;
        for (int k = 0; k < m; k++)
        {
            Ave = (k * Ave + (one ^ S[k])) / (k + 1.0);
        }
        if (Ave < K) return 0.0;
        return Ave - K;
    }

}
}
}
