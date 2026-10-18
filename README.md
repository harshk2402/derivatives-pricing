# deriv

A C++ derivatives pricing library: binomial trees, closed-form Black–Scholes, and Monte Carlo simulation. It also includes the numerical tools those pricers rely on (root finding, quadrature and linear algebra).

The library is small and dependency-free, and is meant to be read. Each pricing method is a short, self-contained class, and the same option can be priced several independent ways so the methods can be checked against one another.

```
$ ./build/examples/price_every_way
European call: S0 = 106, K = 100, r = 5.8%, sigma = 46%, T = 0.75

  Method                                           Price      vs BS
  Black-Scholes formula                         21.56375    0.00000
  Generalized Black-Scholes (b = r)             21.56198   -0.00178   coarser normal CDF
  CRR formula, N = 20                           21.70036    0.13661
  Backward induction, N = 20                    21.70036    0.13661
  Cost of replicating portfolio, N = 20         21.70036    0.13661   X*S0 + Y
  American call (Snell envelope), N = 20        21.70036    0.13661   never exercised early
  Backward induction, N = 2000                  21.56482    0.00106
  Monte Carlo, 1,000,000 paths                  21.53146   -0.03229   +/- 0.034486 (1 s.e.)

  Delta: Black-Scholes 0.67532, binomial hedge X(0,0) 0.67416, Monte Carlo 0.67566
  Volatility implied by the N = 2000 binomial price: 0.46003 (true 0.46)
```

## Features

| Module | What it does |
|---|---|
| **`binomial`** | Cox–Ross–Rubinstein model with an arbitrage check. European options priced by the closed-form CRR formula and by backward induction. The replicating strategy (delta and cash lattices). American options by the Snell envelope, with the optimal stopping set. Payoffs: call, put, digital call/put, bull and bear spreads, strangle, butterfly, double digital. |
| **`analytic`** | Black–Scholes call price, delta and vega. Generalized Black–Scholes with cost of carry, for stocks, dividend-paying stocks, futures and FX. Implied volatility. |
| **`montecarlo`** | Exact GBM path simulation. Path-dependent options with standard errors. Delta and gamma by pathwise rescaling. Arithmetic and geometric Asian calls; the geometric one has a closed form. Control-variate variance reduction. |
| **`montecarlo::basket`** | Correlated multi-asset GBM. Basket Asian call. Per-asset deltas computed on common random numbers. |
| **`fixedincome`** | Bond yield to maturity. |
| **`numerics`** | Bisection and Newton–Raphson on a generic `Function` interface. Trapezoid and Simpson quadrature. Vector and matrix algebra. Box–Muller normal draws. |

The maths behind each method is written up in [docs/methods.md](docs/methods.md).

## Building

You need a C++11 compiler and CMake 3.14 or later.

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The examples are written to `build/examples/`. To use the library from another CMake project:

```cmake
add_subdirectory(deriv)            # or: find_package(deriv) after `cmake --install build`
target_link_libraries(my_app PRIVATE deriv::deriv)
```

## Usage

```cpp
#include "deriv/binomial/BinomialPricer.h"
#include "deriv/montecarlo/AsianOptions.h"

using namespace deriv;

// American put on a 500-step tree calibrated to S0 = 100, r = 5%, sigma = 30%, T = 1
int N = 500;
double h = 1.0 / N, U = std::exp(0.3 * std::sqrt(h));
binomial::BinomialTreeModel model(100.0, U, 1.0 / U, std::exp(0.05 * h));
binomial::Put put(N, 100.0);
binomial::BinLattice<double> prices;
binomial::BinLattice<bool> exercise;
double american = binomial::BinomialPricer(&put).PriceBySnell(model, prices, exercise);

// Arithmetic Asian call with a geometric Asian control variate
montecarlo::GBMModel gbm(100.0, 0.03, 0.2);
montecarlo::ArithmeticAsianCall asian(1.0 / 12, 100.0, 30);
montecarlo::GeometricAsianCall control(1.0 / 12, 100.0, 30);
asian.PriceByVarRedMC(gbm, 30000, control, 0.001);
asian.GetPrice(); asian.GetPricingError(); asian.GetDelta();
```

`#include "deriv/deriv.h"` pulls in everything.

## Examples

| Program | Shows |
|---|---|
| [`price_every_way`](examples/price_every_way.cpp) | One European call priced with every method, with the Greeks compared |
| [`european_binomial`](examples/european_binomial.cpp) | Vanilla, digital and combination payoffs, using the CRR formula and backward induction, plus a put-call parity check |
| [`replicating_portfolio`](examples/replicating_portfolio.cpp) | Stock and cash positions at every node; their cost at t=0 equals the price |
| [`american_options`](examples/american_options.cpp) | Snell envelope: the American call equals the European call; the American put's early-exercise premium and stopping tree |
| [`binomial_convergence`](examples/binomial_convergence.cpp) | CRR approaching Black–Scholes as N goes from 8 to 4096, with the oscillating error |
| [`black_scholes`](examples/black_scholes.cpp) | Generalized Black–Scholes for stocks, cash dividends and futures; put-call parity |
| [`implied_volatility`](examples/implied_volatility.cpp) | Inverting Black–Scholes by bisection and by Newton–Raphson |
| [`bond_yield`](examples/bond_yield.cpp) | Yield to maturity of a coupon bond |
| [`asian_monte_carlo`](examples/asian_monte_carlo.cpp) | Direct Monte Carlo with delta and gamma, then the control variate |
| [`basket_option`](examples/basket_option.cpp) | Three correlated assets: basket Asian price and a delta per asset |
| [`numerical_integration`](examples/numerical_integration.cpp) | Trapezoid vs Simpson; checking the normal CDF approximation |

## Design

```
include/deriv/              src/                    one namespace per module
├── binomial/               ├── binomial/           deriv::binomial
├── analytic/               ├── analytic/           deriv::analytic       → numerics
├── fixedincome/            ├── fixedincome/        deriv::fixedincome    → numerics
├── montecarlo/             ├── montecarlo/         deriv::montecarlo     → numerics, analytic
│   └── basket/             │   └── basket/         deriv::montecarlo::basket → numerics
├── numerics/               └── numerics/           deriv::numerics
└── deriv.h
```

- **Models and contracts are separate.** A model (`BinomialTreeModel`, `GBMModel`) describes how the underlying moves. A contract (`Option`, `PathDepOption`) only knows its payoff, through a virtual `Payoff`. Adding a new payoff means writing one subclass with one method; every pricer then handles it.
- **One solver, many problems.** `NonlinearSolver` works on any `numerics::Function`. The same code finds a bond's yield and an option's implied volatility.
- **Monte Carlo results stay on the option.** `PriceByMC` and `PriceByVarRedMC` store the price, standard error and Greeks on the object, and the getters read them back. This lets a control variate combine one simulation's results with another option's closed form.
- **The methods check one another.** The CRR formula and backward induction agree to machine precision. The replicating portfolio costs exactly the option price. Monte Carlo and the closed forms agree within the standard error. The regression tests rely on all of these.

## Testing

`tests/deriv_tests.cpp` runs 97 checks:

- **Deterministic pricers** are compared with reference values to a relative tolerance of 1e-11. This covers the binomial prices and lattices, Snell, both Black–Scholes classes, the bond yield and the quadrature.
- **Monte Carlo** is checked statistically against known prices, to within a few standard errors.
- **Seeded Monte Carlo** runs on macOS reproduce reference values exactly. These depend on the platform's `std::rand()` sequence, so they are skipped elsewhere.
- **Every example** runs as a smoke test under `ctest`.

## Limitations

- Random numbers come from `std::rand()` and Box–Muller. This keeps things simple and reproducible, but it is not a production-quality generator. The global seed also makes the Monte Carlo code unsafe to run on several threads. Each Monte Carlo model seeds from the clock when it is constructed; call `std::srand(seed)` afterwards for reproducible runs.
- `PriceByCRRFormula` builds binomial coefficients from factorials stored in a `long`, so it supports at most N = 20 steps. `BinomialPricer` has no such limit.
- `ValidateInputData` and `BinLattice::Display` write to `std::cout`.

## References

- Daniel Duffy, *Introduction to C++ for Financial Engineers*. `GeneralizedBlackScholes` is adapted from the book's `EuropeanOption` class ((C) Datasim Component Technology BV 2003-2006).
- John Hull, *Options, Futures, and Other Derivatives*. Source of the put and cash-dividend examples in `black_scholes`.
