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

	// dp/df = d/df E[(k - F)^+] = -E[1(F <= k) dF/df] = -P^s(F <= k)
	inline double put_delta(double f, double s, double k, const distribution& m = distribution_normal{})
	{
		double x = moneyness(f, s, k, m);

		return -m.cdf(x, s);
	}
	// dp/df = d/df E[(F - k)^+] = E[1(F >= k) dF/df] = P^s(F >= k) = 1 - P^s(F <= k)
	inline double call_delta(double f, double s, double k, const distribution& m = distribution_normal{})
	{
		double x = moneyness(f, s, k, m);

		return 1 - m.cdf(x, s);
	}

#ifdef _DEBUG
	inline bool test_put_delta()
	{
		{
			// f(x + dx) - f(x - dx) = f(x) + f'(x) dx + (1/2) f''(x) dx^2 + O(dx^3)
			//                       -(f(x) - f'(x) dx + (1/2) f''(x) dx^2 + O(dx^3)
			//                       = 2f'(x) dx + O(dx^3)
			// f'(x) = (f(x + dx) - f(x - dx))/2dx + O(dx^2)
			double f = 100;
			double s = 0.1;
			double k = 100;
			for (auto df : { 0.1, 0.01, 0.001, 0.0001 }) {
				double dp = (put(f + df, s, k) - put(f - df, s, k)) / (2 * df);
				dp = dp;
				//!!! if dp - put_delta is not close to df^2 return false
			}
		}

		return true;
	}
#endif // _DEBUG

#if 0
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
	// S_t = s0 exp(r - kappa(sigma sqrt(t)) + sigma sqrt(t) X)
	// F_t = f exp(-kappa(s) + s X)
	// so D = exp(-rt), f = s0/D, s = sigma sqrt(t)
	namespace bsm {

		// Black to Black-Scholes/Merton parameters
		inline std::tuple<double, double, double> Dfs(double r, double s0, double sigma, double t)
		{
			if (s0 < 0 || sigma < 0 || t < 0) {
				// NaN indicates which condition failed
				return { s0 < 0 ? NaN : s0, sigma < 0 ? NaN : sigma, t < 0 ? NaN : t };
			}

			double D = exp(-r * t);

			return { D, s0 / D, sigma * sqrt(t) };
		}

		// option types with strike and expiration
		struct put {
			double k;
			double t;
		};
		struct call {
			double k;
			double t;
		};

		// value = D E[nu(F/D)]
		template<class O>
		inline double value(double r, double s0, double sigma, const O& o, const distribution& m = distribution_normal{});
		
		template<>
		inline double value(double r, double s0, double sigma, const put& o, const distribution& m)
		{
			auto [D, f, s] = Dfs(r, s0, sigma, o.t);

			return D * option::put(f, s, o.k, m);
		}

		template<>
		inline double value(double r, double s0, double sigma, const call& o, const distribution& m)
		{
			auto [D, f, s] = Dfs(r, s0, sigma, o.t);

			return D * option::call(f, s, o.k, m);
		}

		// delta = dp/ds_0 = d/ds_0 D E[nu(F/D)] = D E[nu'(F/D) dF/df dF/ds0] = E^s[nu'(F/D]  
		template<class O>
		inline double delta(double r, double s0, double sigma, const O& o, const distribution& m = distribution_normal{});

		template<>
		inline double delta(double r, double s0, double sigma, const put& o, const distribution& m)
		{
			auto [D, f, s] = Dfs(r, s0, sigma, o.t);

			return option::put_delta(f, s, o.k, m);
		}

		template<>
		inline double delta(double r, double s0, double sigma, const call& o, const distribution& m)
		{
			auto [D, f, s] = Dfs(r, s0, sigma, o.t);

			return option::call_delta(f, s, o.k, m);
		}


		// gamma
		// vega
		// implied

	}
}
