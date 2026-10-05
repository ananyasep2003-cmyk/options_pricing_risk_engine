#include "pricing.hpp"

#include <cmath>
#include <stdexcept>
#include <vector>
#include <algorithm>

namespace OptionPricing {

double binomialPrice(
    double S,
    double K,
    double r,
    double sigma,
    double T,
    int steps,
    OptionType type
) {

    if (S <= 0.0 || K <= 0.0 || sigma <= 0.0 ||
        T <= 0.0 || steps <= 0) {
        throw std::invalid_argument("Invalid option parameters");
    }

    // Length of each time step
    double dt = T / steps;

    // Up and down movement factors
    double u = std::exp(sigma * std::sqrt(dt));
    double d = 1.0 / u;

    // Risk-neutral probability
    double p = (std::exp(r * dt) - d) / (u - d);

    // Discount factor
    double discount = std::exp(-r * dt);

    // Store option values at maturity
    std::vector<double> optionValues(steps + 1);

    // Calculate stock prices and option payoff at maturity
    for (int i = 0; i <= steps; ++i) {

        double stockPrice =
            S * std::pow(u, i) * std::pow(d, steps - i);

        if (type == OptionType::Call) {
            optionValues[i] =
                std::max(stockPrice - K, 0.0);
        } else {
            optionValues[i] =
                std::max(K - stockPrice, 0.0);
        }
    }

    // Work backwards through the tree
    for (int step = steps - 1; step >= 0; --step) {

        for (int i = 0; i <= step; ++i) {

            optionValues[i] =
                discount *
                (p * optionValues[i + 1] +
                 (1.0 - p) * optionValues[i]);
        }
    }

    return optionValues[0];
}

}