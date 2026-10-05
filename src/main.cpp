#include "pricing.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

using namespace OptionPricing;

int main() {

    // =========================================================
    // Market / Option Parameters
    // =========================================================

    const double S = 100.0;
    const double K = 100.0;
    const double r = 0.05;
    const double sigma = 0.20;
    const double T = 1.0;

    // =========================================================
    // 1. Black-Scholes Benchmark
    // =========================================================

    double bsPrice = blackScholesPrice(
        S,
        K,
        r,
        sigma,
        T,
        OptionType::Call
    );

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Black-Scholes Call: "
              << bsPrice
              << "\n\n";


    // =========================================================
    // 2. Monte Carlo Convergence
    // =========================================================

    std::cout << "Monte Carlo Convergence\n";
    std::cout << "Simulations   Price       Std Error   Absolute Error\n";
    std::cout << "------------------------------------------------\n";

    std::vector<int> simulationCounts = {
        1000,
        5000,
        10000,
        50000,
        100000,
        500000,
        1000000
    };

    std::ofstream convergenceFile(
        "results/convergence.csv"
    );

    convergenceFile
        << "simulations,"
        << "price,"
        << "standard_error,"
        << "confidence_lower,"
        << "confidence_upper,"
        << "absolute_error\n";

    for (int simulations : simulationCounts) {

        MonteCarloResult result =
            monteCarloPrice(
                S,
                K,
                r,
                sigma,
                T,
                simulations,
                OptionType::Call
            );

        double absoluteError =
            std::abs(result.price - bsPrice);

        std::cout
            << std::setw(11)
            << simulations
            << "   "
            << std::setw(9)
            << result.price
            << "    "
            << std::setw(9)
            << result.standardError
            << "       "
            << std::setw(9)
            << absoluteError
            << "\n";

        convergenceFile
            << simulations << ","
            << result.price << ","
            << result.standardError << ","
            << result.confidenceLower << ","
            << result.confidenceUpper << ","
            << absoluteError
            << "\n";
    }

    convergenceFile.close();

    std::cout
        << "\nResults saved to results/convergence.csv\n\n";


    // =========================================================
    // 3. Implied Volatility
    // =========================================================

    std::cout << "Implied Volatility Test\n";
    std::cout << "-----------------------\n";

    double knownVolatility = 0.20;

    double optionPrice = blackScholesPrice(
        S,
        K,
        r,
        knownVolatility,
        T,
        OptionType::Call
    );

    double impliedVol =
        impliedVolatility(
            optionPrice,
            S,
            K,
            r,
            T,
            OptionType::Call
        );

    std::cout
        << "Known volatility:   "
        << knownVolatility
        << "\n";

    std::cout
        << "Option price:       "
        << optionPrice
        << "\n";

    std::cout
        << "Implied volatility: "
        << impliedVol
        << "\n\n";


    // =========================================================
    // 4. Analytical Greeks
    // =========================================================

    std::cout << "Analytical Greeks\n";
    std::cout << "----------------\n";

    double analyticalDelta =
        delta(
            S,
            K,
            r,
            sigma,
            T,
            OptionType::Call
        );

    double analyticalGamma =
        gamma(
            S,
            K,
            r,
            sigma,
            T
        );

    double analyticalVega =
        vega(
            S,
            K,
            r,
            sigma,
            T
        );

    std::cout
        << "Delta: "
        << analyticalDelta
        << "\n";

    std::cout
        << "Gamma: "
        << analyticalGamma
        << "\n";

    std::cout
        << "Vega:  "
        << analyticalVega
        << "\n\n";


    // =========================================================
    // 5. Finite-Difference Greeks
    // =========================================================

    std::cout << "Finite-Difference Greeks\n";
    std::cout << "------------------------\n";

    double numericalDelta =
        finiteDifferenceDelta(
            S,
            K,
            r,
            sigma,
            T,
            OptionType::Call
        );

    double numericalGamma =
        finiteDifferenceGamma(
            S,
            K,
            r,
            sigma,
            T,
            OptionType::Call
        );

    double numericalVega =
        finiteDifferenceVega(
            S,
            K,
            r,
            sigma,
            T,
            OptionType::Call
        );

    std::cout
        << "Delta: "
        << numericalDelta
        << "\n";

    std::cout
        << "Gamma: "
        << numericalGamma
        << "\n";

    std::cout
        << "Vega:  "
        << numericalVega
        << "\n\n";


    // =========================================================
    // 6. Greek Validation
    // =========================================================

    double deltaError =
        std::abs(
            analyticalDelta - numericalDelta
        );

    double gammaError =
        std::abs(
            analyticalGamma - numericalGamma
        );

    double vegaError =
        std::abs(
            analyticalVega - numericalVega
        );

    std::cout << "Greek Validation\n";
    std::cout << "----------------\n";

    std::cout
        << "Delta absolute error: "
        << deltaError
        << "\n";

    std::cout
        << "Gamma absolute error: "
        << gammaError
        << "\n";

    std::cout
        << "Vega absolute error:  "
        << vegaError
        << "\n\n";


    // =========================================================
    // 7. Portfolio Risk
    // =========================================================

    std::vector<OptionPosition> positions = {

        // Long 10 ATM calls
        {
            10.0,
            100.0,
            100.0,
            0.05,
            0.20,
            1.0,
            OptionType::Call
        },

        // Short 5 OTM calls
        {
            -5.0,
            100.0,
            110.0,
            0.05,
            0.20,
            1.0,
            OptionType::Call
        },

        // Long 8 ATM puts
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

    PortfolioGreeks portfolio =
        calculatePortfolioGreeks(
            positions
        );

    std::cout << "Portfolio Risk\n";
    std::cout << "--------------\n";

    std::cout
        << "Positions: "
        << positions.size()
        << "\n";

    std::cout
        << "Portfolio Delta: "
        << portfolio.delta
        << "\n";

    std::cout
        << "Portfolio Gamma: "
        << portfolio.gamma
        << "\n";

    std::cout
        << "Portfolio Vega:  "
        << portfolio.vega
        << "\n\n";


    // =========================================================
    // 8. Dynamic Delta Hedging Simulation
    // =========================================================

    std::cout << "Delta Hedging Simulation\n";
    std::cout << "-----------------------\n";

    const int hedgingSteps = 252;
    const int hedgingPaths = 1000;

    const double dt =
        T / hedgingSteps;

    std::mt19937 generator(42);

    std::normal_distribution<double> normal(
        0.0,
        1.0
    );

    std::vector<double> hedgingPnL;

    hedgingPnL.reserve(
        hedgingPaths
    );

    for (int path = 0;
         path < hedgingPaths;
         ++path) {

        double currentS = S;

        // -----------------------------------------------------
        // Initial option value and delta
        // -----------------------------------------------------

        double currentOptionPrice =
            blackScholesPrice(
                currentS,
                K,
                r,
                sigma,
                T,
                OptionType::Call
            );

        double currentDelta =
            delta(
                currentS,
                K,
                r,
                sigma,
                T,
                OptionType::Call
            );

        // Short delta shares to hedge a long call.
        double stockPosition =
            -currentDelta;

        // Cash required after receiving proceeds
        // from the short stock position and paying
        // for the option.
        double cash =
            currentDelta * currentS
            - currentOptionPrice;


        // -----------------------------------------------------
        // Daily rebalancing
        // -----------------------------------------------------

        for (int step = 1;
             step <= hedgingSteps;
             ++step) {

            // Simulate stock price
            double Z = normal(generator);

            currentS *= std::exp(
                (r - 0.5 * sigma * sigma) * dt
                + sigma * std::sqrt(dt) * Z
            );

            // Accrue cash at risk-free rate
            cash *= std::exp(r * dt);

            // Remaining time
            double remainingTime =
                T - step * dt;

            if (remainingTime < 0.0) {
                remainingTime = 0.0;
            }

            // Recalculate delta
            double newDelta;

            if (remainingTime > 0.0) {

                newDelta =
                    delta(
                        currentS,
                        K,
                        r,
                        sigma,
                        remainingTime,
                        OptionType::Call
                    );

            } else {

                // At maturity, use the limiting
                // Delta of the call.
                if (currentS > K) {
                    newDelta = 1.0;
                } else {
                    newDelta = 0.0;
                }
            }

            double newStockPosition =
                -newDelta;

            // Amount of stock traded
            double stockTrade =
                newStockPosition
                - stockPosition;

            // Buying stock reduces cash.
            // Selling stock increases cash.
            cash -= stockTrade * currentS;

            stockPosition =
                newStockPosition;
        }


        // -----------------------------------------------------
        // Terminal portfolio value
        // -----------------------------------------------------

        double optionPayoff =
            std::max(
                currentS - K,
                0.0
            );

        double stockValue =
            stockPosition * currentS;

        double hedgingPortfolioValue =
            cash + stockValue;

        // P&L of the long option plus the
        // delta-hedging portfolio.
        double pnl =
            optionPayoff
            + hedgingPortfolioValue;

        hedgingPnL.push_back(pnl);
    }


    // ---------------------------------------------------------
    // Hedging statistics
    // ---------------------------------------------------------

    double meanPnL =
        std::accumulate(
            hedgingPnL.begin(),
            hedgingPnL.end(),
            0.0
        ) / hedgingPaths;

    double squaredDeviation = 0.0;

    double meanAbsolutePnL = 0.0;

    for (double pnl : hedgingPnL) {

        squaredDeviation +=
            (pnl - meanPnL)
            * (pnl - meanPnL);

        meanAbsolutePnL +=
            std::abs(pnl);
    }

    double pnlStandardDeviation =
        std::sqrt(
            squaredDeviation
            / (hedgingPaths - 1)
        );

    meanAbsolutePnL /=
        hedgingPaths;


    std::cout
        << "Paths: "
        << hedgingPaths
        << "\n";

    std::cout
        << "Hedging steps: "
        << hedgingSteps
        << "\n";

    std::cout
        << "Mean hedging P&L: "
        << meanPnL
        << "\n";

    std::cout
        << "P&L standard deviation: "
        << pnlStandardDeviation
        << "\n";

    std::cout
        << "Mean absolute P&L: "
        << meanAbsolutePnL
        << "\n\n";


    // =========================================================
    // 9. Binomial Convergence
    // =========================================================

    std::cout << "Binomial Convergence\n";
    std::cout << "--------------------\n";
    std::cout << "Steps     Price       Absolute Error\n";
    std::cout << "-------------------------------------\n";

    std::vector<int> binomialSteps = {
        5,
        10,
        25,
        50,
        100,
        250,
        500
    };

    std::ofstream binomialFile(
        "results/binomial_convergence.csv"
    );

    binomialFile
        << "steps,"
        << "binomial_price,"
        << "black_scholes_price,"
        << "absolute_error\n";

    for (int steps : binomialSteps) {

        double treePrice =
            binomialPrice(
                S,
                K,
                r,
                sigma,
                T,
                steps,
                OptionType::Call
            );

        double absoluteError =
            std::abs(
                treePrice - bsPrice
            );

        std::cout
            << std::setw(5)
            << steps
            << "     "
            << std::setw(9)
            << treePrice
            << "         "
            << std::setw(9)
            << absoluteError
            << "\n";

        binomialFile
            << steps << ","
            << treePrice << ","
            << bsPrice << ","
            << absoluteError
            << "\n";
    }

    binomialFile.close();

    std::cout
        << "\nResults saved to "
        << "results/binomial_convergence.csv\n";

    return 0;
}