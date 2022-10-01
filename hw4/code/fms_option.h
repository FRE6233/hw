// fms_option.h - Forward option pricing model F = f e^{-kappa(s) + sX},
// where f = E[F], s^2 = Var(log(F)), and X is mean 0, variance 1.
#pragma once
#include <cmath>
#include <algorithm>
#ifdef _DEBUG
#include <random>
#endif
#include <tuple>
#include "fms_distribution.h"
#include "fms_distribution_normal.h"

namespace fms::option {

	inline const double NaN = std::numeric_limits<double>::quiet_NaN();
	inline const double eps = std::numeric_limits<double>::epsilon();


	// F <= k iff X <= (log(k/f) + kappa(s))/s
	inline double moneyness(double f, double s, double k, const distribution& m = distribution_normal{})
	{
		if (k <= 0 || f <= 0 || s <= 0) {
			return NaN;
		}

		return (log(k / f) + m.cgf(s)) / s;
	}

	// E[max(k - F, 0)] = k P(F <= k) - f P^s(F <= k)
	inline double put(double f, double s, double k, const distribution& m = distribution_normal{})
	{
		// edge cases
		if (f == 0) {
			return k;
		}
		if (s == 0) {
			return std::max(k - f, 0.);
		}
		if (k == 0) {
			return 0;
		}

		double x = moneyness(f, s, k, m);
		double N = m.cdf(x);
		double Ns = m.cdf(x, s);

		return k * N - f * Ns;
	}
	// Put call parity c - p = f - k
	inline double call(double f, double s, double k, const distribution& m = distribution_normal{})
	{
		return put(f, s, k, m) + f - k;
	}
#if 0
	// dp/df
	inline double delta(double f, double s, double k, const distribution& m = distribution_normal{})
	{
		return 0;
	}

	// d^2p/df^2
	inline double gamma(double f, double s, double k, const distribution& m = distribution_normal{})
	{
		return 0;
	}
#endif // 0
	// dp/ds = -f d/ds P(x, s)
	inline double vega(double f, double s, double k, const distribution& m = distribution_normal{})
	{
		double x = moneyness(f, s, k, m);

		return -f * m.cdf(x, s, 0, 1);
	}

	// p = put(f, implied(f, p, k, m), k, m)
	inline std::tuple<double,double,unsigned> implied(double f, double p, double k, double s = 0, unsigned n = 0, double tol = 0)
	{
		// max(k - f,0) >= k - f
		// max(k - f,0) <= k
		if (p <= std::max(k - f, 0.) || p >= k) {
			return { NaN, 0, 0 };
		}

		if (s == 0) {
			s = 0.1; // initial vol guess
		}
		if (n == 0) {
			n = 100; // maximum number of iterations
		}
		if (tol == 0) {
			tol = sqrt(eps); // absolute tolerance
		}

		double p_ = put(f, s, k);
		double dp_ = vega(f, s, k); // dv/ds
		double s_ = s - (p_ - p) / dp_; // Newton-Raphson
		s_ = std::clamp(s_, s / 2, 2 * s);

		unsigned n0 = 1;
		while (fabs(s_ - s) > tol && n0 <= n) {
			p_ = put(f, s_, k);
			dp_ = vega(f, s_, k);
			s = s_ - (p_ - p) / dp_;
			s_ = std::clamp(s_, s / 2, 2 * s);
			std::swap(s_, s);
			++n0;
		}

		return { s_, s_ - s, n0 };
	}
#ifdef _DEBUG
	inline bool test_implied()
	{
		std::default_random_engine dre;
		auto rng = std::uniform_real_distribution(-.1, .1);
		{
			double f = 100;
			double s = 0.2;

			unsigned nmax = 0;
			double kmax = 0;
			for (double k = 20; k <= 130; k += 10) {
				double p = put(f, s, k);
				auto [s_, ds, n] = implied(f, p, k, s + rng(dre), 100, sqrt(eps));
				if (isnan(s_) || fabs(ds) > sqrt(eps) || n == 100) {
					return false;
				}
				if (n > nmax) {
					nmax = n;
					kmax = k;
				}
			}
			nmax = nmax;
			if (put(f, s, 10) != 0) {
				return false;
			}
		}

		return true;
	}
#endif // _DEBUG

	// Black-Scholes/Merton 
	// S_t = S exp(r - kappa(sigma sqrt(t)) + sigma sqrt(t) X)
	// F_t = f exp(-kappa(s) + s X)
	// so D = exp(-rt), f = S/D, s = sigma sqrt(t)
	namespace bsm {

		struct put {
			double k;
			double t;
		};
		struct call {
			double k;
			double t;
		};

		// Black to Black-Scholes/Merton parameters
		inline std::tuple<double, double, double> Dfs(double r, double S, double sigma, double t)
		{
			double D = exp(-r * t);

			return { D, S / D, sigma * sqrt(t) };
		}

		// value = D*E[nu(F/D)]
		template<class O>
		inline double value(double r, double S, double sigma, const O& o, const distribution& m = distribution_normal{});
		
		template<>
		inline double value(double r, double S, double sigma, const put& o, const distribution& m)
		{
			auto [D, f, s] = Dfs(r, S, sigma, o.t);

			return D * option::put(f, s, o.k, m);
		}

		template<>
		inline double value(double r, double S, double sigma, const call& o, const distribution& m)
		{
			auto [D, f, s] = Dfs(r, S, sigma, o.t);

			return D * option::call(f, s, o.k, m);
		}

		// delta
		// gamma
		// vega
		// implied

	}
}
