Absolutely. Replace your **entire `README.md`** with this final version. This version incorporates the **actual validated results** from your project, including the corrected delta-hedging experiment.

````markdown
# Option Pricing & Risk Engine

A C++17 quantitative finance engine for **European option pricing, implied volatility estimation, Greek calculation, portfolio risk aggregation, and dynamic delta hedging**.

The project implements multiple numerical pricing methods and validates them against analytical Black-Scholes results through convergence experiments and automated tests.

---

## Overview

This project was built to study the numerical methods used in derivatives pricing and risk management.

The engine currently supports:

- Black-Scholes analytical pricing
- Cox-Ross-Rubinstein binomial trees
- Monte Carlo pricing under geometric Brownian motion
- Brent root-finding for implied volatility
- Analytical Greeks
- Finite-difference Greeks
- Portfolio-level Greek aggregation
- Dynamic delta hedging
- Monte Carlo convergence analysis
- Binomial tree convergence analysis
- Automated numerical validation

The emphasis is on **numerical accuracy, convergence, and model validation**, rather than simply implementing pricing formulas.

---

## Model Architecture

```text
                    Option Pricing & Risk Engine
                              │
             ┌────────────────┼────────────────┐
             │                │                │
       Analytical         Numerical         Risk
        Pricing            Methods         Analytics
             │                │                │
      Black-Scholes      Binomial Tree      Greeks
                         Monte Carlo        Portfolio Risk
                              │             Delta Hedging
                              │
                     Numerical Validation
                              │
              ┌───────────────┴───────────────┐
              │                               │
       Convergence Tests                Automated Tests
````

---

# 1. Black-Scholes Pricing

The engine implements the analytical Black-Scholes model for European calls and puts.

For a European call:

$$
C = S N(d_1) - K e^{-rT} N(d_2)
$$

For a European put:

$$
P = K e^{-rT} N(-d_2) - S N(-d_1)
$$

where:

$$
d_1 =
\frac{
\ln(S/K) + (r + \frac{1}{2}\sigma^2)T
}{
\sigma\sqrt{T}
}
$$

and

$$
d_2 = d_1 - \sigma\sqrt{T}
$$

### Inputs

* $S$ — underlying price
* $K$ — strike price
* $r$ — continuously compounded risk-free rate
* $\sigma$ — volatility
* $T$ — time to maturity

For the benchmark configuration:

```text
S     = 100
K     = 100
r     = 5%
σ     = 20%
T     = 1 year
```

the Black-Scholes call price is:

```text
10.450584
```

---

# 2. Binomial Tree

A **Cox-Ross-Rubinstein (CRR)** binomial tree is implemented for European option pricing.

The up and down factors are:

$$
u = e^{\sigma\sqrt{\Delta t}}
$$

$$
d = \frac{1}{u}
$$

The risk-neutral probability is:

$$
p =
\frac{
e^{r\Delta t} - d
}{
u-d
}
$$

Option values are calculated at maturity and recursively discounted backward through the tree.

## Convergence Results

The binomial price is compared against the Black-Scholes benchmark:

```text
Black-Scholes benchmark = 10.450584
```

| Steps | Binomial Price | Absolute Error |
| ----: | -------------: | -------------: |
|     5 |      10.805934 |       0.355350 |
|    10 |      10.253409 |       0.197175 |
|    25 |      10.520966 |       0.070382 |
|    50 |      10.410692 |       0.039892 |
|   100 |      10.430612 |       0.019972 |
|   250 |      10.442589 |       0.007995 |
|   500 |      10.446585 |   **0.003998** |

The pricing error decreases substantially as the number of tree steps increases, with the 500-step tree producing an absolute error of approximately **0.004** relative to Black-Scholes.

Results are also written to:

```text
results/binomial_convergence.csv
```

---

# 3. Monte Carlo Pricing

The Monte Carlo engine simulates the terminal stock price under risk-neutral geometric Brownian motion:

$$
S_T =
S
\exp
\left[
\left(r-\frac{1}{2}\sigma^2\right)T
+
\sigma\sqrt{T}Z
\right]
$$

where:

$$
Z \sim N(0,1)
$$

The option price is estimated from the discounted expected payoff:

$$
V =
e^{-rT}E[\text{Payoff}]
$$

The implementation also calculates the standard error and an approximate 95% confidence interval.

## Convergence Analysis

The pricing engine evaluates Monte Carlo estimates across:

```text
1,000
5,000
10,000
50,000
100,000
500,000
1,000,000
```

simulations.

For each simulation count, the following are recorded:

* Monte Carlo price
* Standard error
* 95% confidence interval
* Absolute error relative to Black-Scholes

### Results

| Simulations |     Price |   Std. Error | Absolute Error |
| ----------: | --------: | -----------: | -------------: |
|       1,000 | 11.419267 |     0.502958 |       0.968683 |
|       5,000 | 10.705404 |     0.211992 |       0.254821 |
|      10,000 | 10.595096 |     0.147732 |       0.144513 |
|      50,000 | 10.504448 |     0.065913 |       0.053864 |
|     100,000 | 10.474081 |     0.046708 |       0.023497 |
|     500,000 | 10.459257 |     0.020838 |       0.008673 |
|   1,000,000 | 10.468200 | **0.014757** |       0.017616 |

Increasing the number of simulations from **1,000 to 1,000,000** reduces the estimated standard error from **0.502958 to 0.014757**, consistent with the expected Monte Carlo convergence behavior:

$$
SE \propto \frac{1}{\sqrt{N}}
$$

The absolute error is not expected to decrease monotonically because Monte Carlo estimates contain statistical noise.

Results are written to:

```text
results/convergence.csv
```

---

# 4. Implied Volatility

The engine calculates implied volatility by solving:

$$
V(\sigma) - V_{\text{market}} = 0
$$

The implementation uses **Brent's root-finding method**, which combines bracketing with interpolation while maintaining numerical robustness.

## Round-Trip Validation

A known volatility is first used to generate an option price:

```text
Known volatility    = 0.200000
Option price        = 10.450584
Recovered volatility = 0.200000
```

The implied-volatility solver successfully recovers the original volatility.

---

# 5. Option Greeks

The engine calculates the primary Black-Scholes Greeks:

### Delta

Measures sensitivity of option value to the underlying price:

$$
\Delta =
\frac{\partial V}{\partial S}
$$

### Gamma

Measures the rate of change of Delta:

$$
\Gamma =
\frac{\partial^2 V}{\partial S^2}
$$

### Vega

Measures sensitivity of option value to volatility:

$$
Vega =
\frac{\partial V}{\partial \sigma}
$$

For the benchmark option:

| Greek | Analytical |
| ----- | ---------: |
| Delta |   0.636831 |
| Gamma |   0.018762 |
| Vega  |  37.524035 |

---

# 6. Analytical vs Finite-Difference Greeks

Two independent approaches are implemented.

### Analytical

Closed-form Black-Scholes expressions are used for:

* Delta
* Gamma
* Vega

### Finite Difference

Central finite-difference approximations are used as an independent numerical validation.

For Delta:

$$
\Delta \approx
\frac{
V(S+h)-V(S-h)
}{
2h
}
$$

For Gamma:

$$
\Gamma \approx
\frac{
V(S+h)-2V(S)+V(S-h)
}{
h^2
}
$$

For Vega:

$$
Vega \approx
\frac{
V(\sigma+h)-V(\sigma-h)
}{
2h
}
$$

### Validation Results

| Greek | Analytical | Finite Difference | Absolute Error |
| ----- | ---------: | ----------------: | -------------: |
| Delta |   0.636831 |          0.636831 |             ~0 |
| Gamma |   0.018762 |          0.018762 |             ~0 |
| Vega  |  37.524035 |         37.524034 |             ~0 |

The finite-difference estimates closely match the analytical Black-Scholes Greeks.

---

# 7. Portfolio Risk

The engine supports portfolios containing multiple option positions.

Each position contains:

```text
Quantity
Underlying price
Strike
Risk-free rate
Volatility
Time to maturity
Option type
```

Portfolio Greeks are calculated by aggregating the Greeks of individual positions:

$$
\Delta_{portfolio}
=
\sum_i q_i\Delta_i
$$

$$
\Gamma_{portfolio}
=
\sum_i q_i\Gamma_i
$$

$$
Vega_{portfolio}
=
\sum_i q_iVega_i
$$

Both long and short positions are supported.

### Example Portfolio

The validation portfolio contains:

```text
+10 ATM calls
 -5 OTM calls
 +8 ATM puts
```

Resulting portfolio risk:

```text
Portfolio Delta = 1.214712
Portfolio Gamma = 0.238776
Portfolio Vega  = 477.552384
```

---

# 8. Dynamic Delta Hedging

A discrete-time delta-hedging experiment is used to study the replication of a European call.

A long call is hedged by dynamically maintaining a short position in the underlying according to the Black-Scholes Delta.

The simulation:

1. Generates stock-price paths using geometric Brownian motion.
2. Calculates the option Delta at each rebalancing date.
3. Adjusts the underlying hedge.
4. Accrues the cash account at the risk-free rate.
5. Rebalances the hedge over 252 trading days.
6. Evaluates terminal hedging P&L.

## Hedging Results

Using:

```text
Simulation paths = 1,000
Hedging steps     = 252
```

the simulation produces:

| Metric                 |        Result |
| ---------------------- | ------------: |
| Simulation paths       |         1,000 |
| Hedging steps          |           252 |
| Mean hedging P&L       | **-0.011654** |
| P&L standard deviation |  **0.431601** |
| Mean absolute P&L      |  **0.319800** |

The mean hedging P&L is close to zero, consistent with the theoretical replication framework under the Black-Scholes assumptions.

The remaining dispersion reflects the error introduced by **discrete-time rebalancing** and stochastic stock-price movements.

---

# 9. Numerical Validation

The project includes automated tests covering the core pricing and risk calculations.

### Tests

* Black-Scholes benchmark pricing
* Put-call parity
* Implied-volatility round trip
* Binomial convergence
* Analytical vs finite-difference Delta
* Analytical vs finite-difference Gamma
* Analytical vs finite-difference Vega
* Portfolio Greek aggregation

### Test Output

```text
PASS: Black-Scholes pricing
PASS: Put-call parity
PASS: Implied volatility
PASS: Binomial convergence
PASS: Analytical vs finite-difference Greeks
PASS: Portfolio Greeks

All tests passed.
```

---

# 10. Project Structure

```text
Black_Scholes Project/
│
├── include/
│   └── pricing.hpp
│
├── src/
│   ├── main.cpp
│   ├── black_schole.cpp
│   ├── binomial.cpp
│   ├── monte_carlo.cpp
│   ├── brent.cpp
│   ├── implied_volatility.cpp
│   ├── greeks.cpp
│   ├── finite_difference.cpp
│   └── portfolio.cpp
│
├── tests/
│   └── test_pricing.cpp
│
├── results/
│   ├── convergence.csv
│   └── binomial_convergence.csv
│
├── CMakeLists.txt
└── README.md
```

---

# 11. Building the Project

The project requires a compiler supporting **C++17**.

On macOS, the project can be compiled directly using `g++`:

```bash
g++ -std=c++17 -Iinclude \
src/main.cpp \
src/black_schole.cpp \
src/binomial.cpp \
src/monte_carlo.cpp \
src/brent.cpp \
src/implied_volatility.cpp \
src/greeks.cpp \
src/finite_difference.cpp \
src/portfolio.cpp \
-o option_engine
```

Run the pricing engine:

```bash
./option_engine
```

---

# 12. Running the Tests

Compile the test suite:

```bash
g++ -std=c++17 -Iinclude \
tests/test_pricing.cpp \
src/black_schole.cpp \
src/binomial.cpp \
src/monte_carlo.cpp \
src/brent.cpp \
src/implied_volatility.cpp \
src/greeks.cpp \
src/finite_difference.cpp \
src/portfolio.cpp \
-o test_pricing
```

Run:

```bash
./test_pricing
```

Expected result:

```text
All tests passed.
```

---

# 13. Numerical Experiments

The project produces CSV files containing numerical experiment results.

### Monte Carlo

```text
results/convergence.csv
```

Contains:

```text
Simulation count
Monte Carlo price
Standard error
95% confidence interval
Absolute error
```

### Binomial Tree

```text
results/binomial_convergence.csv
```

Contains:

```text
Number of tree steps
Binomial price
Black-Scholes price
Absolute error
```

These outputs can be analyzed using Python, pandas, or spreadsheet software.

---

# 14. Technologies

### Programming

* C++17

### Quantitative Finance

* Black-Scholes
* Binomial option pricing
* Monte Carlo simulation
* Implied volatility
* Option Greeks
* Portfolio risk
* Delta hedging

### Numerical Methods

* Brent root finding
* Finite-difference differentiation
* Monte Carlo statistical estimation
* Numerical convergence analysis

### Data Analysis

* CSV experiment output
* Statistical error estimation
* Convergence analysis

---

# 15. Key Quantitative Concepts

The project provides hands-on implementation of:

* Risk-neutral pricing
* Geometric Brownian motion
* Discounted expected payoff
* Risk-neutral probabilities
* Root-finding
* Numerical differentiation
* Monte Carlo estimation
* Statistical confidence intervals
* Numerical convergence
* Sensitivity analysis
* Portfolio risk aggregation
* Dynamic hedging

---

# 16. Future Extensions

Potential extensions include:

* Variance-reduced Monte Carlo
* Antithetic variates
* American option pricing
* Additional Greeks
* Volatility surface construction
* Local volatility models
* Interest-rate models
* More sophisticated hedging strategies
* Transaction-cost-aware hedging
* Calibration to market option prices

---

# Author

**Ananya Sivakumar**

---

# Disclaimer

This project is for educational and research purposes and is not intended to provide investment or trading advice.

```

### One small note

I would **not add more features now**. The project has enough depth:

**3 pricing methods → numerical root finding → Greeks → finite differences → portfolio risk → hedging → convergence studies → automated validation.**

That's substantially more impressive for a QR application than adding another 3–4 option types without depth.

Next, I'd do the **:contentReference[oaicite:0]{index=0}**: remove compiled binaries, make sure the CSV results are included, check `git status`, and then we'll write the final **quant-research resume bullet**.
```
