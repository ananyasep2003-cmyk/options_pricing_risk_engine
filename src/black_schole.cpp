#include "pricing.hpp"

#include <cmath>
#include <stdexcept>
#include <algorithm>

namespace OptionPricing {

namespace {

double normalCDF(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}

}

double blackScholesPrice(
    double S,
    double K,
    double r,
    double sigma,
    double T,
    OptionType type
) {

    if (S <= 0.0 || K <= 0.0 || sigma <= 0.0 || T <= 0.0) {
        throw std::invalid_argument("Invalid option parameters");
    }

    const double sqrtT = std::sqrt(T);

    const double d1 =
        (std::log(S / K) +
         (r + 0.5 * sigma * sigma) * T)
        / (sigma * sqrtT);

    const double d2 = d1 - sigma * sqrtT;

    const double discountFactor = std::exp(-r * T);

    if (type == OptionType::Call) {

        return S * normalCDF(d1)
             - K * discountFactor * normalCDF(d2);

    } else {

        return K * discountFactor * normalCDF(-d2)
             - S * normalCDF(-d1);
    }
}

}