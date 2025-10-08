#include "PathDepOption.h"
#include <cmath>

namespace fre {
    void Rescale(SamplePath& S, double x)
    {
        int m = S.size();
        for (int j = 0; j < m; j++) S[j] = x * S[j];
    }

    double PathDepOption::PriceByMC(const Model& model, long N, double epsilon)
    {
        double H = 0.0, Hsq = 0.0, HepsUp = 0.0, HepsDown = 0.0;
        SamplePath S(m);

        for (long i = 0; i < N; i++)
        {
            model.GenerateSamplePath(T, m, S);
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

        double discount = exp(-model.GetR() * T);
        Price = discount * H;
        PricingError = discount * sqrt(Hsq - H * H) / sqrt(N - 1.0);
        delta = discount * (HepsUp - H) / (model.GetS0() * epsilon);
        gamma = discount * (HepsUp - 2 * H + HepsDown) / (pow(model.GetS0() * epsilon, 2.0));

        return Price;
    }

    double asianCall::Payoff(const SamplePath& S) const
    {
        double Ave = 0.0;
        for (int k = 0; k < m; k++) Ave = (k * Ave + S[k]) / (k + 1.0);
        if (Ave < K) return 0.0;
        return Ave - K;
    }
}
