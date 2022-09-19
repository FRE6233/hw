// fms_distribution_logistic.h - Logistic distribution.
#pragma once
#define _USE_MATH_DEFINES // for M_PI
#include <cmath>
#include <corecrt_math_defines.h>
#include "fms_hypergeometric.h"
#include "fms_distribution.h"

namespace fms {

	struct distribution_logistic : public distribution {
		inline static double sigma = sqrt(3.) / M_PI;

		// f(x,s) = e^{-kappa(s) + sx} e^{-x/sigma}/sigma(1 + e^{-x/sigma})^2
		double _pdf(double x, double s = 0) const override
		{
			double exs = exp(-x / sigma);
			double exsp1 = exs + 1;

			return (exp(s * x) / mgf(s)) * exs / (sigma * exsp1 * exsp1);
		}

		// F(x) = 1/(1 + e^{-x/sigma}), s = 0
		// F(x, s) = F(x)^{1 + sigma s}/(1 + sigma s) 2F1(1 + sigma s, sigma s, 2 + sigma s, F(x))/mgf(s), -1/sigma < s < 1/sigma
		double _cdf(double x, double s = 0) const override
		{
			if (s == 0) {
				return 1 / (1 + exp(-x / sigma));
			}

			return 0;
		}

#ifdef _DEBUG
		bool test_cdf() const
		{
			double eps = sqrt(std::numeric_limits<double>::epsilon());

			{
				// P^s(X <= x) = int_0^{F(x)} (q/(1 - q))^{sigma s} dq/mgf(s)
				double x = 0;
				double s = 0.1;
				double dq = 0.0001;
				double Fx = cdf(x);
				double P = 0;
				for (double q = 0; q <= Fx; q += dq) {
					P += pow(q / (1 - q), sigma * s) * dq;
				}
				double Pxs = P / _mgf(s);
				double Pxs_ = _cdf(x, s);
				double dP = Pxs - Pxs_;
				if (abs(dP) * dq > eps) {
					return false;
				}
			}

			return true;
		}
#endif // _DEBUG

		// moment generating function E[e^{sX}]
		double _mgf(double s) const override
		{
			// 1 + sigma s > 0 and 1 - sigma s > 0
			if (fabs(s * sigma) >= 1) {
				return std::numeric_limits<double>::quiet_NaN();
			}

			double pss = M_PI * sigma * s;

			return s == 0 ? 1 : pss / sin(pss);
		}
		double _cgf(double s) const override
		{
			return log(mgf(s));
		}
#ifdef _DEBUG
		bool test_cgf() const
		{
			{
				// E[X] = kappa'(0) = 0 and Var(X) = kappa''(0) = 1.
				double ds = 1e-6;
				double k = _cgf(0);
				double k_ = _cgf(ds);
				double _k = _cgf(-ds);

				double dk = (k_ - _k) / (2 * ds);
				if (fabs(dk - 0) > sqrt(ds)) {
					return false;
				}

				double ddk = (k_ - 2 * k + _k) / (ds * ds);
				if (fabs(ddk - 1) > sqrt(ds)) {
					return false;
				}
			}

			return true;
		}
#endif // _DEBUG
	};

}