// fms_double_exponential.h
#pragma once
#include <cmath>
#include "../../xll/xll/ensure.h"
#include "fms_distribution.h"

namespace fms {

	//!!! implement the double exponential distribution
	// copied from distribution_logistic
	struct distribution_double_exponential : public distribution {
		// copied from https://keithalewis.github.io/math/de.html
		inline static double β = sqrt(2.);

		//!!! stub function
		double _pdf(double x, double s = 0) const override
		{
			return 0;
		}

#pragma warning(push)
#pragma warning(disable: 4100)
		double _cdf(double x, double s = 0, unsigned nx = 0, unsigned ns = 0) const override
		{
			ensure(s < β);

			if (x <= 0) {
				return (1 - s / β) * exp((s + β) * x) / 2;
			}
			else {
				return 1 - (1 + s / β) * exp((s - β) * x) / 2;
			}
		}
#pragma warning(pop)

		// moment generating function E[e^{sX}]
		double _mgf(double s, unsigned n = 0) const override
		{
			return β * β/((β + s)*(β - s));
		}
		double _cgf(double s, unsigned n = 0) const override
		{
			return log(_mgf(s, n));
		}
	};

}