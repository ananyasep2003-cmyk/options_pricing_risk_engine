#include "pricing.hpp"

#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <limits>

namespace OptionPricing {

double brentRoot(
    const std::function<double(double)>& function,
    double lower,
    double upper,
    double tolerance,
    int maxIterations
) {

    double a = lower;
    double b = upper;

    double fa = function(a);
    double fb = function(b);

    // Root must be bracketed
    if (fa * fb > 0.0) {
        throw std::invalid_argument(
            "Root is not bracketed"
        );
    }

    // c is the previous best approximation
    double c = b;
    double fc = fb;

    double d = b - a;
    double e = d;

    for (int iteration = 0;
         iteration < maxIterations;
         ++iteration) {

        // Make sure b and c bracket the root
        if ((fb > 0.0 && fc > 0.0) ||
            (fb < 0.0 && fc < 0.0)) {

            c = a;
            fc = fa;

            d = b - a;
            e = d;
        }

        // Make b the best approximation
        if (std::abs(fc) < std::abs(fb)) {

            a = b;
            b = c;
            c = a;

            fa = fb;
            fb = fc;
            fc = fa;
        }

        double tolerance1 =
            2.0 * std::numeric_limits<double>::epsilon()
            * std::abs(b)
            + 0.5 * tolerance;

        double midpoint =
            0.5 * (c - b);

        // Convergence condition
        if (std::abs(midpoint) <= tolerance1 ||
            fb == 0.0) {

            return b;
        }

        if (std::abs(e) >= tolerance1 &&
            std::abs(fa) > std::abs(fb)) {

            // Attempt interpolation

            double s = fb / fa;

            double p;
            double q;

            if (a == c) {

                // Secant method
                p = 2.0 * midpoint * s;
                q = 1.0 - s;

            } else {

                // Inverse quadratic interpolation

                double q1 = fa / fc;
                double r = fb / fc;

                p = s *
                    (2.0 * midpoint * q1 * (q1 - r)
                     - (b - a) * (r - 1.0));

                q = (q1 - 1.0) *
                    (r - 1.0) *
                    (s - 1.0);
            }

            if (p > 0.0) {
                q = -q;
            } else {
                p = -p;
            }

            double minimum1 =
                3.0 * midpoint * q
                - std::abs(tolerance1 * q);

            double minimum2 =
                std::abs(e * q);

            if (2.0 * p <
                std::min(minimum1, minimum2)) {

                e = d;
                d = p / q;

            } else {

                d = midpoint;
                e = midpoint;
            }

        } else {

            d = midpoint;
            e = midpoint;
        }

        a = b;
        fa = fb;

        if (std::abs(d) > tolerance1) {
            b += d;
        } else {
            b += (midpoint > 0.0
                  ? tolerance1
                  : -tolerance1);
        }

        fb = function(b);
    }

    throw std::runtime_error(
        "Brent method failed to converge"
    );
}

}