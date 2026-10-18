#pragma once

namespace deriv {
namespace numerics {

    // Standard normal draw by the Box-Muller transform on std::rand().
    // Seeding is left to the caller (the Monte Carlo models seed from time()).
    double Gauss();

}
}
