#include "pricing.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace OptionPricing;

bool approximatelyEqual(
    double a,
    double b,
    double tolerance
) {
    return std::abs(a - b) < tolerance;
}

void testBlackScholes() {

    double price = blackScholesPrice(
        100.0, 100.0, 0.05, 0.20, 1.0,
        OptionType::Call
    );

    double expected = 10.4506;

    if (!approximatelyEqual(price, expected, 0.001)) {
        throw std::runtime_error("Black-Scholes test failed");
    }

    std::cout << "PASS: Black-Scholes pricing\n";
}

void testPutCallParity() {

    double callPrice = blackScholesPrice(
        100.0, 100.0, 0.05, 0.20, 1.0,
        OptionType::Call
    );

    double putPrice = blackScholesPrice(
        100.0, 100.0, 0.05, 0.20, 1.0,
        OptionType::Put
    );

    double lhs = callPrice - putPrice;

    double rhs = 100.0 - 100.0 * std::exp(-0.05);

    if (!approximatelyEqual(lhs, rhs, 1e-6)) {
        throw std::runtime_error("Put-call parity test failed");
    }

    std::cout << "PASS: Put-call parity\n";
}

void testImpliedVolatility() {

    double trueVolatility = 0.20;

    double marketPrice = blackScholesPrice(
        100.0, 100.0, 0.05, trueVolatility, 1.0,
        OptionType::Call
    );

    double recoveredVolatility = impliedVolatility(
        marketPrice,
        100.0,
        100.0,
        0.05,
        1.0,
        OptionType::Call
    );

    if (!approximatelyEqual(
            recoveredVolatility,
            trueVolatility,
            1e-6)) {

        throw std::runtime_error(
            "Implied volatility test failed"
        );
    }

    std::cout << "PASS: Implied volatility\n";
}

void testBinomialConvergence() {

    double bsPrice = blackScholesPrice(
        100.0,
        100.0,
        0.05,
        0.20,
        1.0,
        OptionType::Call
    );

    // IMPORTANT:
    // The variable is called treePrice, NOT binomialPrice.
    double treePrice = OptionPricing::binomialPrice(
        100.0,
        100.0,
        0.05,
        0.20,
        1.0,
        500,
        OptionType::Call
    );

    if (!approximatelyEqual(
            treePrice,
            bsPrice,
            0.05)) {

        throw std::runtime_error(
            "Binomial convergence test failed"
        );
    }

    std::cout << "PASS: Binomial convergence\n";
}

void testGreeks() {

    double analyticalDelta = delta(
        100.0, 100.0, 0.05, 0.20, 1.0,
        OptionType::Call
    );

    double numericalDelta = finiteDifferenceDelta(
        100.0, 100.0, 0.05, 0.20, 1.0,
        OptionType::Call
    );

    double analyticalGamma = gamma(
        100.0, 100.0, 0.05, 0.20, 1.0
    );

    double numericalGamma = finiteDifferenceGamma(
        100.0, 100.0, 0.05, 0.20, 1.0,
        OptionType::Call
    );

    double analyticalVega = vega(
        100.0, 100.0, 0.05, 0.20, 1.0
    );

    double numericalVega = finiteDifferenceVega(
        100.0, 100.0, 0.05, 0.20, 1.0,
        OptionType::Call
    );

    if (!approximatelyEqual(
            analyticalDelta,
            numericalDelta,
            1e-5)) {

        throw std::runtime_error("Delta test failed");
    }

    if (!approximatelyEqual(
            analyticalGamma,
            numericalGamma,
            1e-5)) {

        throw std::runtime_error("Gamma test failed");
    }

    if (!approximatelyEqual(
            analyticalVega,
            numericalVega,
            1e-3)) {

        throw std::runtime_error("Vega test failed");
    }

    std::cout << "PASS: Analytical vs finite-difference Greeks\n";
}

void testPortfolioGreeks() {

    std::vector<OptionPosition> positions = {

        {
            10.0,
            100.0,
            100.0,
            0.05,
            0.20,
            1.0,
            OptionType::Call
        },

        {
            -5.0,
            100.0,
            110.0,
            0.05,
            0.20,
            1.0,
            OptionType::Call
        },

        {
            8.0,
            100.0,
            100.0,
            0.05,
            0.20,
            1.0,
            OptionType::Put
        }
    };

    PortfolioGreeks result =
        calculatePortfolioGreeks(positions);

    if (!std::isfinite(result.delta) ||
        !std::isfinite(result.gamma) ||
        !std::isfinite(result.vega)) {

        throw std::runtime_error(
            "Portfolio Greeks test failed"
        );
    }

    std::cout << "PASS: Portfolio Greeks\n";
}

int main() {

    try {

        testBlackScholes();
        testPutCallParity();
        testImpliedVolatility();
        testBinomialConvergence();
        testGreeks();
        testPortfolioGreeks();

        std::cout << "\nAll tests passed.\n";

    } catch (const std::exception& error) {

        std::cerr
            << "\nTEST FAILURE: "
            << error.what()
            << "\n";

        return 1;
    }

    return 0;
}