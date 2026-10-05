#ifndef PRICING_HPP
#define PRICING_HPP

#include <functional>
#include <vector>

namespace OptionPricing {

enum class OptionType {
    Call,
    Put
};

// ---------------------------------------------------------
// Black-Scholes
// ---------------------------------------------------------

double blackScholesPrice(
    double S,
    double K,
    double r,
    double sigma,
    double T,
    OptionType type
);

// ---------------------------------------------------------
// Binomial Tree
// ---------------------------------------------------------

double binomialPrice(
    double S,
    double K,
    double r,
    double sigma,
    double T,
    int steps,
    OptionType type
);

// ---------------------------------------------------------
// Monte Carlo
// ---------------------------------------------------------

struct MonteCarloResult {
    double price;
    double standardError;
    double confidenceLower;
    double confidenceUpper;
};

MonteCarloResult monteCarloPrice(
    double S,
    double K,
    double r,
    double sigma,
    double T,
    int simulations,
    OptionType type
);

// ---------------------------------------------------------
// Brent Root Finding
// ---------------------------------------------------------

double brentRoot(
    const std::function<double(double)>& function,
    double lower,
    double upper,
    double tolerance = 1e-8,
    int maxIterations = 100
);

// ---------------------------------------------------------
// Implied Volatility
// ---------------------------------------------------------

double impliedVolatility(
    double marketPrice,
    double S,
    double K,
    double r,
    double T,
    OptionType type
);

// ---------------------------------------------------------
// Analytical Greeks
// ---------------------------------------------------------

double delta(
    double S,
    double K,
    double r,
    double sigma,
    double T,
    OptionType type
);

double gamma(
    double S,
    double K,
    double r,
    double sigma,
    double T
);

double vega(
    double S,
    double K,
    double r,
    double sigma,
    double T
);

// ---------------------------------------------------------
// Finite-Difference Greeks
// ---------------------------------------------------------

double finiteDifferenceDelta(
    double S,
    double K,
    double r,
    double sigma,
    double T,
    OptionType type,
    double h = 0.01
);

double finiteDifferenceGamma(
    double S,
    double K,
    double r,
    double sigma,
    double T,
    OptionType type,
    double h = 0.01
);

double finiteDifferenceVega(
    double S,
    double K,
    double r,
    double sigma,
    double T,
    OptionType type,
    double h = 0.0001
);

// ---------------------------------------------------------
// Portfolio Risk
// ---------------------------------------------------------

struct OptionPosition {
    double quantity;
    double S;
    double K;
    double r;
    double sigma;
    double T;
    OptionType type;
};

struct PortfolioGreeks {
    double delta;
    double gamma;
    double vega;
};

PortfolioGreeks calculatePortfolioGreeks(
    const std::vector<OptionPosition>& positions
);

} // namespace OptionPricing

#endif