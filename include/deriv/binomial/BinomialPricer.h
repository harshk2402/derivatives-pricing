#pragma once
#include "deriv/binomial/BinomialTreeModel.h"
#include "deriv/binomial/BinLattice.h"
#include "deriv/binomial/Option.h"

namespace deriv {
namespace binomial {

    // Prices an Option on a BinomialTreeModel. The option is held by pointer
    // and must outlive the pricer.
    class BinomialPricer
    {
    private:
        Option* pOption;
        BinomialPricer() : pOption(0) {}
        BinomialPricer(const BinomialPricer& pricer) : pOption(pricer.pOption) {}

    public:
        BinomialPricer(Option* pOption_) : pOption(pOption_) {}
        ~BinomialPricer() {}

        // European price by backward induction under the risk-neutral measure
        double PriceByCRR(const BinomialTreeModel& Model);

        // European price by backward induction, also recording the replicating
        // strategy: X(n,i) shares of stock and Y(n,i) in the money market,
        // held over the period from step n to n+1
        double PriceByCRR(const BinomialTreeModel& Model,
                          BinLattice<double>& X,
                          BinLattice<double>& Y);

        // American price as the Snell envelope of the payoff. PriceTree holds
        // the option value at every node; StoppingTree marks the nodes where
        // immediate exercise is optimal
        double PriceBySnell(const BinomialTreeModel& Model,
                            BinLattice<double>& PriceTree,
                            BinLattice<bool>& StoppingTree);
    };

}
}
