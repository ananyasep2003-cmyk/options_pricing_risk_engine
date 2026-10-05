#include "pricing.hpp"

namespace OptionPricing {

PortfolioGreeks calculatePortfolioGreeks(
    const std::vector<OptionPosition>& positions
) {

    PortfolioGreeks portfolio{
        0.0,
        0.0,
        0.0
    };

    for (const OptionPosition& position : positions) {

        double positionDelta =
            delta(
                position.S,
                position.K,
                position.r,
                position.sigma,
                position.T,
                position.type
            );

        double positionGamma =
            gamma(
                position.S,
                position.K,
                position.r,
                position.sigma,
                position.T
            );

        double positionVega =
            vega(
                position.S,
                position.K,
                position.r,
                position.sigma,
                position.T
            );

        portfolio.delta +=
            position.quantity * positionDelta;

        portfolio.gamma +=
            position.quantity * positionGamma;

        portfolio.vega +=
            position.quantity * positionVega;
    }

    return portfolio;
}

}