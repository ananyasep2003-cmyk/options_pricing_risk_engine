#include "pricing.hpp"

#include <stdexcept>

namespace OptionPricing {

double impliedVolatility(
    double marketPrice,
    double S,
    double K,
    double r,
    double T,
    OptionType type
) {

    if (marketPrice <= 0.0) {
        throw std::invalid_argument(
            "Market price must be positive"
        );
    }

    auto objective =
        [&](double volatility) {

            return blackScholesPrice(
                S,
                K,
                r,
                volatility,
                T,
                type
            ) - marketPrice;
        };

    return brentRoot(
        objective,
        0.0001,
        5.0,
        1e-8,
        100
    );
}

}