#include "pricing.hpp"

#include <cmath>
#include <stdexcept>

namespace OptionPricing {

namespace {

double normalPDF(double x) {
    return std::exp(-0.5 * x * x)
           / std::sqrt(2.0 * M_PI);
}

double calculateD1(
    double S,
    double K,
    double r,
    double sigma,
    double T
) {
    return (
        std::log(S / K)
        + (r + 0.5 * sigma * sigma) * T
    ) / (sigma * std::sqrt(T));
}

void validateParameters(
    double S,
    double K,
    double sigma,
    double T
) {
    if (S <= 0.0 ||
        K <= 0.0 ||
        sigma <= 0.0 ||
        T <= 0.0) {

        throw std::invalid_argument(
            "Invalid option parameters"
        );
    }
}

}

// ---------------------------------------------------------
// Delta
// ---------------------------------------------------------

double delta(
    double S,
    double K,
    double r,
    double sigma,
    double T,
    OptionType type
) {

    validateParameters(S, K, sigma, T);

    double d1 =
        calculateD1(S, K, r, sigma, T);

    double cumulativeProbability =
        0.5 * std::erfc(
            -d1 / std::sqrt(2.0)
        );

    if (type == OptionType::Call) {
        return cumulativeProbability;
    }

    return cumulativeProbability - 1.0;
}


// ---------------------------------------------------------
// Gamma
// ---------------------------------------------------------

double gamma(
    double S,
    double K,
    double r,
    double sigma,
    double T
) {

    validateParameters(S, K, sigma, T);

    double d1 =
        calculateD1(S, K, r, sigma, T);

    return normalPDF(d1)
           / (S * sigma * std::sqrt(T));
}


// ---------------------------------------------------------
// Vega
// ---------------------------------------------------------

double vega(
    double S,
    double K,
    double r,
    double sigma,
    double T
) {

    validateParameters(S, K, sigma, T);

    double d1 =
        calculateD1(S, K, r, sigma, T);

    return S
           * normalPDF(d1)
           * std::sqrt(T);
}

}