// fms_hypergeometric.h
#pragma once
#include <cmath>
#include <limits>

namespace fms {

	// 2F1(a, b; c; x) = sum_0^infty (a)_n (b)_n/(c)_n x^n/n!
	// (q)_n = q(q + 1)...(q + n - 1), (q)_0 = 1
	template<class X = double>
	inline X _2F1(X a, X b, X c, X x)
	{
		if (b == c) { // (1 - x)^a = 1 + a x + a(a + 1) x^2/2 + ...
			return pow(1 - x, a);
		}

		X eps = sqrt(std::numeric_limits<X>::epsilon());
		X ab_cn = 1; // (a)_n (b)_n/(c)_n
		X x_n = 1;   // x^n/n!
		X s = 1;     // n = 0

		X ds = 1;
		for (int n = 1; fabs(ds) > eps || fabs(x_n) > eps; ++n, ++a, ++b, ++c) {
			ab_cn *= a * b / c;
			x_n *= x / n;
			ds = ab_cn * x_n;
			s += ds;
		}

		return s;
	}

#ifdef _DEBUG
	template<class X = double>
	inline bool test_2F1()
	{
		X eps = sqrt(std::numeric_limits<X>::epsilon());
		{
			// _2F1(1,1,2,-x) = log(1 + x)/x;
			X one = 1;
			X two = 2;
			double xs[] = { -.1, .1, .9 };

			for (X x : xs) {
				X F = _2F1(one, one, two, -x);
				X F_ = log(1 + x) / x;
				X dF = F - F_;
				if (fabs(dF) > eps) {
					return false;
				}
			}

		}

		return true;
	}
#endif // _DEBUG
}
