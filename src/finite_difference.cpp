#include "pricing.hpp"

#include <stdexcept>

namespace OptionPricing {

namespace {

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
// Finite-Difference Delta
// ---------------------------------------------------------

double finiteDifferenceDelta(
    double S,
    double K,
    double r,
    double sigma,
    double T,
    OptionType type,
    double h
) {

    validateParameters(S, K, sigma, T);

    if (h <= 0.0 || h >= S) {
        throw std::invalid_argument(
            "Invalid finite-difference step size"
        );
    }

    double priceUp =
        blackScholesPrice(
            S + h,
            K,
            r,
            sigma,
            T,
            type
        );

    double priceDown =
        blackScholesPrice(
            S - h,
            K,
            r,
            sigma,
            T,
            type
        );

    return (priceUp - priceDown) / (2.0 * h);
}


// ---------------------------------------------------------
// Finite-Difference Gamma
// ---------------------------------------------------------

double finiteDifferenceGamma(
    double S,
    double K,
    double r,
    double sigma,
    double T,
    OptionType type,
    double h
) {

    validateParameters(S, K, sigma, T);

    if (h <= 0.0 || h >= S) {
        throw std::invalid_argument(
            "Invalid finite-difference step size"
        );
    }

    double priceUp =
        blackScholesPrice(
            S + h,
            K,
            r,
            sigma,
            T,
            type
        );

    double priceCurrent =
        blackScholesPrice(
            S,
            K,
            r,
            sigma,
            T,
            type
        );

    double priceDown =
        blackScholesPrice(
            S - h,
            K,
            r,
            sigma,
            T,
            type
        );

    return (
        priceUp
        - 2.0 * priceCurrent
        + priceDown
    ) / (h * h);
}


// ---------------------------------------------------------
// Finite-Difference Vega
// ---------------------------------------------------------

double finiteDifferenceVega(
    double S,
    double K,
    double r,
    double sigma,
    double T,
    OptionType type,
    double h
) {

    validateParameters(S, K, sigma, T);

    if (h <= 0.0 || h >= sigma) {
        throw std::invalid_argument(
            "Invalid finite-difference volatility step"
        );
    }

    double priceUp =
        blackScholesPrice(
            S,
            K,
            r,
            sigma + h,
            T,
            type
        );

    double priceDown =
        blackScholesPrice(
            S,
            K,
            r,
            sigma - h,
            T,
            type
        );

    return (priceUp - priceDown) / (2.0 * h);
}

}