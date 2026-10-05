#include "pricing.hpp"

#include <cmath>
#include <random>
#include <stdexcept>
#include <algorithm>

namespace OptionPricing {

MonteCarloResult monteCarloPrice(
    double S,
    double K,
    double r,
    double sigma,
    double T,
    int simulations,
    OptionType type
) {

    if (S <= 0.0 || K <= 0.0 || sigma <= 0.0 ||
        T <= 0.0 || simulations <= 0) {
        throw std::invalid_argument("Invalid option parameters");
    }

    // Random number generator
    std::mt19937 generator(42);

    // Standard normal distribution
    std::normal_distribution<double> normal(0.0, 1.0);

    double payoffSum = 0.0;
    double payoffSquaredSum = 0.0;

    for (int i = 0; i < simulations; ++i) {

        // Generate standard normal random variable
        double Z = normal(generator);

        // Simulate terminal stock price
        double ST =
            S * std::exp(
                (r - 0.5 * sigma * sigma) * T
                + sigma * std::sqrt(T) * Z
            );

        // Calculate option payoff
        double payoff;

        if (type == OptionType::Call) {
            payoff = std::max(ST - K, 0.0);
        } else {
            payoff = std::max(K - ST, 0.0);
        }

        payoffSum += payoff;
        payoffSquaredSum += payoff * payoff;
    }

    // Mean payoff
    double meanPayoff =
        payoffSum / simulations;

    // Sample variance
    double variance =
        (payoffSquaredSum / simulations)
        - (meanPayoff * meanPayoff);

    // Standard deviation of payoff
    double standardDeviation =
        std::sqrt(variance);

    // Standard error of the mean
    double standardError =
        standardDeviation / std::sqrt(simulations);

    // Discount factor
    double discountFactor =
        std::exp(-r * T);

    // Monte Carlo option price
    double price =
        discountFactor * meanPayoff;

    // Discount the standard error as well
    double discountedStandardError =
        discountFactor * standardError;

    // 95% confidence interval
    double confidenceLower =
        price - 1.96 * discountedStandardError;

    double confidenceUpper =
        price + 1.96 * discountedStandardError;

    return {
        price,
        discountedStandardError,
        confidenceLower,
        confidenceUpper
    };
}

}