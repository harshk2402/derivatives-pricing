# Methods

The maths behind each pricer, with pointers to the code that implements it.

## Binomial trees — `deriv::binomial`

### Model
`BinomialTreeModel` is the Cox–Ross–Rubinstein model. Over each period the stock moves from $S$ to $SU$ or $SD$, and cash grows by a factor $R$:

$$S(n,i) = S_0\,U^i D^{\,n-i}, \qquad q = \frac{R-D}{U-D}.$$

The model is arbitrage-free iff $D < R < U$, in which case $q \in (0,1)$ is the risk-neutral probability of an up move (`ValidateInputData`). A tree calibrated to Black–Scholes parameters over $N$ steps of length $h = T/N$ uses

$$U = e^{\sigma\sqrt h}, \qquad D = 1/U, \qquad R = e^{rh}.$$

### European options
**Closed-form CRR formula** (`PriceByCRRFormula`): the price is the discounted risk-neutral expectation of the payoff $h$ over the terminal nodes,

$$H(0) = R^{-N}\sum_{i=0}^{N}\binom{N}{i} q^i (1-q)^{N-i}\, h\big(S(N,i)\big).$$

**Backward induction** (`BinomialPricer::PriceByCRR`) gives the same number by stepping back through the tree one period at a time:

$$H(n,i) = \frac{q\,H(n+1,i+1) + (1-q)\,H(n+1,i)}{R}.$$

The induction costs $O(N^2)$ and works for any `Option`. The formula is a single sum, but its factorials limit it to $N \le 20$.

### Replicating strategy
At node $(n,i)$, holding $X$ shares and $Y$ in cash reproduces the option's value in both successor states when

$$X(n,i) = \frac{H(n+1,i+1) - H(n+1,i)}{S(n+1,i+1) - S(n+1,i)}, \qquad Y(n,i) = \frac{H(n+1,i) - X(n,i)\,S(n+1,i)}{R}.$$

$X$ is the hedge ratio (the discrete delta), and $X(0,0)S_0 + Y(0,0) = H(0)$. The three-argument `PriceByCRR` overload fills both lattices.

### American options
`PriceBySnell` computes the Snell envelope, i.e. the larger of exercising now and continuing:

$$H(n,i) = \max\!\Big(h\big(S(n,i)\big),\ \frac{q\,H(n+1,i+1) + (1-q)\,H(n+1,i)}{R}\Big).$$

The stopping lattice records which term wins. With no dividends an American call is never exercised early, so it is worth exactly the European call. An American put is worth more than the European put.

## Closed form — `deriv::analytic`

### Black–Scholes
`BlackScholesCall` implements

$$C = S_0 N(d_+) - K e^{-rT} N(d_-), \qquad d_\pm = \frac{\ln(S_0/K) + (r \pm \tfrac12\sigma^2)T}{\sigma\sqrt T},$$

with $\Delta = N(d_+)$ and vega $= S_0\,\varphi(d_+)\sqrt T$. `NormalCDF` is a five-term polynomial approximation; the `numerical_integration` example compares it with the exact value.

### Generalized Black–Scholes
`GeneralizedBlackScholes` adds a cost-of-carry rate $b$:

$$C = S e^{(b-r)T} N(d_1) - K e^{-rT} N(d_2), \qquad P = K e^{-rT} N(-d_2) - S e^{(b-r)T} N(-d_1),$$

$$d_1 = \frac{\ln(S/K) + (b + \tfrac12\sigma^2)T}{\sigma\sqrt T}, \qquad d_2 = d_1 - \sigma\sqrt T.$$

Different choices of $b$ cover different underlyings:

| $b$ | Underlying |
|---|---|
| $r$ | non-dividend stock |
| $r - q$ | continuous dividend yield $q$ |
| $0$ | futures (Black's model) |
| $r - r_f$ | foreign currency |

A known cash dividend $D$ paid at time $t$ is handled by pricing on $S - De^{-rt}$. This class uses a coarser three-term polynomial approximation of the normal CDF.

### Implied volatility
`ImpliedVolatility` presents $\sigma \mapsto C_{BS}(\sigma)$ as a `numerics::Function`, with vega as its derivative. Newton–Raphson then iterates

$$\sigma_{k+1} = \sigma_k - \frac{C_{BS}(\sigma_k) - C_{\text{market}}}{\text{vega}(\sigma_k)}.$$

## Fixed income — `deriv::fixedincome`

`BondYield` is the pricing error of an annual-coupon bond under continuous compounding:

$$f(y) = \sum_{i=1}^{T} C e^{-yi} + F e^{-yT} - P, \qquad f'(y) = -\sum_{i=1}^{T} iC e^{-yi} - TF e^{-yT}.$$

The yield to maturity is the root of $f$. The same `NonlinearSolver` that backs out implied volatility finds it.

## Monte Carlo — `deriv::montecarlo`

### Path simulation
`GBMModel` simulates geometric Brownian motion exactly at the dates $t_k = kT/m$:

$$S(t_k) = S(t_{k-1}) \exp\!\Big(\big(r - \tfrac12\sigma^2\big)\Delta t + \sigma\sqrt{\Delta t}\,Z_k\Big), \qquad Z_k \sim N(0,1).$$

The normal draws $Z_k$ come from the Box–Muller transform (`numerics::Gauss`).

### Price and standard error
`PathDepOption::PriceByMC` averages the payoff over $N$ paths:

$$\hat H = \frac1N\sum_{j=1}^N h(\text{path}_j), \qquad \text{price} = e^{-rT}\hat H, \qquad \text{s.e.} = e^{-rT}\,\frac{\sqrt{\widehat{h^2} - \hat H^2}}{\sqrt{N-1}}.$$

### Greeks
GBM paths are linear in $S_0$, so multiplying a simulated path by $(1\pm\epsilon)$ is the same as simulating from $S_0(1\pm\epsilon)$ with the same random numbers. This gives finite differences with very little noise:

$$\Delta \approx e^{-rT}\frac{\hat H^{+\epsilon} - \hat H}{S_0\epsilon}, \qquad \Gamma \approx e^{-rT}\frac{\hat H^{+\epsilon} - 2\hat H + \hat H^{-\epsilon}}{(S_0\epsilon)^2}.$$

### Asian options and the control variate
The arithmetic Asian call pays $\big(\frac1m\sum_k S(t_k) - K\big)^+$ and has no closed form.

The geometric average $G = \big(\prod_k S(t_k)\big)^{1/m}$ is lognormal. Its call is therefore a Black–Scholes call with spot $a$ and volatility $b$:

$$a = e^{-rT} S_0 \exp\!\Big(\tfrac{(m+1)T}{2m}\Big(r + \tfrac12\sigma^2\Big(\tfrac{2m+1}{3m} - 1\Big)\Big)\Big), \qquad b = \sigma\sqrt{\frac{(m+1)(2m+1)}{6m^2}}.$$

The two payoffs are highly correlated, so `PriceByVarRedMC` writes

$$E[h_A] = E[h_A - h_G] + E[h_G].$$

It simulates the difference (`DifferenceOfOptions`) and adds $E[h_G]$ exactly. This cuts the standard error by about 85×.

### Baskets — `deriv::montecarlo::basket`
There are $d$ assets driven by a $d$-dimensional Brownian motion $W$ through the volatility matrix $C$:

$$dS_j = rS_j\,dt + S_j\sum_l C_{jl}\,dW_l, \qquad \sigma_j = \lVert C_j \rVert.$$

The log-returns have covariance $CC^\top\Delta t$. Each step draws $Z \sim N(0, I_d)$ and applies

$$S_j \leftarrow S_j \exp\big((r - \tfrac12\sigma_j^2)\Delta t + \sqrt{\Delta t}\,(CZ)_j\big).$$

The basket Asian call pays $\big(\frac1m\sum_k \sum_j S_j(t_k) - K\big)^+$. Deltas bump one $S_{0,j}$ at a time. They reprice on common random numbers by reseeding the generator, so the difference measures the bump rather than sampling noise.

## Numerics — `deriv::numerics`

| Method | Formula | Error |
|---|---|---|
| Trapezoid | $\frac h2\big[f(a) + 2\sum_{i=1}^{N-1} f(a+ih) + f(b)\big]$ | $O(h^2)$ |
| Simpson ($N$ even) | $\frac h3\big[f(a) + 4\sum_{\text{odd}} f + 2\sum_{\text{even}} f + f(b)\big]$ | $O(h^4)$, exact for cubics |
| Bisection | halve $[L, R]$ keeping the sign change | linear, always converges |
| Newton–Raphson | $x_{k+1} = x_k - (f(x_k) - t)/f'(x_k)$ | quadratic near the root |
