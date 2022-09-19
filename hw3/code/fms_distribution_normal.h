// fms_distribution_normal.h - standard normal distribution
#pragma once
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#include <corecrt_math_defines.h>
#include "fms_distribution.h"

namespace fms {

	struct distribution_normal : public distribution {

		double _pdf(double z, double s = 0) const override
		{
			double z_s = z - s;

			return exp(-z_s * z_s / 2) / sqrt(2 * M_PI);
		}

		// P^s(z) = E[e^{-s^2/2 + s Z} 1(Z <= z)] = P(Z <= z - s)
		double _cdf(double z, double s = 0) const override
		{
			double z_s = z - s;

			return (1 + erf(z_s / sqrt(2))) / 2;
		}

		double _mgf(double s) const override
		{
			return exp(_cgf(s));
		}

		double _cgf(double s) const override
		{
			return s * s / 2;
		}
	};

}
