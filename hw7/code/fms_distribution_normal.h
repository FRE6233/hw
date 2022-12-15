// fms_distribution_normal.h - standard normal distribution
#pragma once
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#include <corecrt_math_defines.h>
#include "fms_distribution.h"

namespace fms {

	struct distribution_normal : public distribution {

		static inline const double M_SQRT2PI = sqrt(2 * M_PI);

		// Hermite polynomials
		// H_{n+1}(x) = x H_n(x) - n H_{n-1}(x), H_0(x) = 1, H_1(x) = x
		static constexpr double H(unsigned n, double x) noexcept
		{
			if (n == 0) {
				return 1;
			}
			if (n == 1) {
				return x;
			}

			return x * H(n - 1, x) - (n - 1) * H(n - 2, x);
		}

		// P(X <= x) and derivatives
		static double N(double x, unsigned n = 0)
		{
			if (n == 0) {
				return  (1 + erf(x / M_SQRT2)) / 2;
			}

			double phi = exp(-x * x / 2) / M_SQRT2PI;

			if (n == 1) {
				return phi;
			}

			// (d/dx)^n phi(x) = (-1)^n phi(x) H_n(x)
			return phi * H(n - 1, x) * ((n & 1) ? 1 : -1);
		}
		double _pdf(double z, double s = 0) const override
		{
			double z_s = z - s;

			return exp(-z_s * z_s / 2) / sqrt(2 * M_PI);
		}

		// P^s(z) = E[e^{-s^2/2 + s Z} 1(Z <= z)] = P(Z <= z - s)
		double _cdf(double z, double s = 0, unsigned nz = 0, unsigned ns = 0) const override
		{
			return N(z - s, nz + ns) * (ns & 1 ? -1 : 1);
		}

		double _mgf(double s, unsigned n = 0) const override
		{
			return n == 0 ? exp(_cgf(s))
				 : n == 1 ? exp(_cgf(s)) * _cgf(s, 1)
				 : std::numeric_limits<double>::quiet_NaN();
		}

		double _cgf(double s, unsigned n = 0) const override
		{
			return n == 0 ? s * s / 2
				 : n == 1 ? s
				 : n == 2 ? 1
				 : 0;
		}
	};

}
