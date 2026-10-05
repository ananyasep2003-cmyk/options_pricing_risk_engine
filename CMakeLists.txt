cmake_minimum_required(VERSION 3.15)

project(OptionPricingEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# ---------------------------------------------------------
# Main option pricing engine
# ---------------------------------------------------------

add_executable(option_engine

    src/main.cpp

    src/black_schole.cpp
    src/binomial.cpp
    src/monte_carlo.cpp

    src/brent.cpp
    src/implied_volatility.cpp

    src/greeks.cpp
    src/finite_difference.cpp

    src/portfolio.cpp
)

# ---------------------------------------------------------
# Automated tests
# ---------------------------------------------------------

add_executable(test_pricing

    tests/test_pricing.cpp

    src/black_schole.cpp
    src/binomial.cpp
    src/monte_carlo.cpp

    src/brent.cpp
    src/implied_volatility.cpp

    src/greeks.cpp
    src/finite_difference.cpp

    src/portfolio.cpp
)

enable_testing()

add_test(
    NAME PricingTests
    COMMAND test_pricing
)