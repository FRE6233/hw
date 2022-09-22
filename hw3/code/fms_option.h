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

	// dp/df
	double delta(double f, double s, double k, const distribution& m)
	{
		return 0;
	}

	// d^2p/df^2
	double gamma(double f, double s, double k, const distribution& m)
	{
		return 0;
	}

	// dp/ds
	double vega(double f, double s, double k, const distribution& m)
	{
		return 0;
	}

	// p = put(f, implied(f, p, k, m), k, m)
	double implied(double f, double p, double k, const distribution& m)
	{
		return 0;
	}
}
