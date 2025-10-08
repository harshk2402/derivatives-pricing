#include "SpreadOption.h"
namespace fre {
	double StrangleOpt::Payoff(double z) const
	{
		if (z <= K1) return K1 - z;
		else if (K2 < z) return z - K2;
		return 0.0;
	}

	double ButterflyOpt::Payoff(double z) const
	{
		if (K1 < z && z <= (K1 + K2) / 2) return z - K1;
		else if ((K1 + K2) / 2 < z && z <= K2) return K2 - z;
		return 0.0;
	}

}
