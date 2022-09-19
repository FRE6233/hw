// fms_option.h - Forward option pricing model F = f e^{-kappa(s) + sX}
#pragma once
#include <cmath>
#include "fms_distribution.h"

namespace fms::option {

	// F <= k iff X <= (log(k/f) + kappa(s))/s
	double moneyness(double f, double s, double k, const distribution& m)
	{
		return (log(k / f) + m.cgf(s)) / s;
	}

	// E[max(k - F, 0)] = k P(F <= k) - f P^s(F <= k)
	double put(double f, double s, double k, const distribution& m)
	{
		double x = moneyness(f, s, k, m);
		double N = m.cdf(x);
		double Ns = m.cdf(x, s);

		return k * N - f * Ns;
	}
}
