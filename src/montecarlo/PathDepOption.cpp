#include "deriv/montecarlo/PathDepOption.h"
#include <cmath>

namespace deriv {
namespace montecarlo {

    namespace {
        void Rescale(SamplePath& S, double x)
        {
            int m = S.size();
            for (int j = 0; j < m; j++) S[j] = x * S[j];
        }
    }

    double PathDepOption::PriceByMC(const GBMModel& Model, long N, double epsilon)
    {
        double H = 0.0, Hsq = 0.0, HepsUp = 0.0, HepsDown = 0.0;
        SamplePath S(m);

        for (long i = 0; i < N; i++)
        {
            Model.GenerateSamplePath(T, m, S);
            double payoff0 = Payoff(S);
            H = (i * H + payoff0) / (i + 1.0);
            Hsq = (i * Hsq + payoff0 * payoff0) / (i + 1.0);

            SamplePath S_up = S;
            Rescale(S_up, 1.0 + epsilon);
            HepsUp = (i * HepsUp + Payoff(S_up)) / (i + 1.0);

            SamplePath S_down = S;
            Rescale(S_down, 1.0 - epsilon);
            HepsDown = (i * HepsDown + Payoff(S_down)) / (i + 1.0);
        }

        double discount = std::exp(-Model.GetR() * T);
        Price = discount * H;
        PricingError = discount * std::sqrt(Hsq - H * H) / std::sqrt(N - 1.0);
        delta = discount * (HepsUp - H) / (Model.GetS0() * epsilon);
        gamma = discount * (HepsUp - 2 * H + HepsDown) / (std::pow(Model.GetS0() * epsilon, 2.0));

        return Price;
    }

    double PathDepOption::PriceByVarRedMC(const GBMModel& Model, long N, PathDepOption& CVOption, double epsilon)
    {
        DifferenceOfOptions VarRedOpt(T, K, m, this, &CVOption);

        Price = VarRedOpt.PriceByMC(Model, N, epsilon) + CVOption.PriceByBSFormula(Model);

        PricingError = VarRedOpt.PricingError;
        delta = VarRedOpt.GetDelta() + CVOption.DeltaByBSFormula(Model);

        return Price;
    }

}
}
