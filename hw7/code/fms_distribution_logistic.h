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

#pragma warning(push)
#pragma warning(disable: 4100)
		// F(x) = 1/(1 + e^{-x/sigma}), s = 0
		// F(x, s) = F(x)^{1 + sigma s}/(1 + sigma s) 
		//           2F1(1 + sigma s, sigma s, 2 + sigma s, F(x))/mgf(s), -1/sigma < s < 1/sigma
		double _cdf(double x, double s = 0, unsigned nx = 0, unsigned ns = 0) const override
		{
			if (s == 0) {
				return 1 / (1 + exp(-x / sigma));
			}

			double Fx = _cdf(x);
			double A = pow(Fx, 1 + sigma * s) / (1 + sigma * s);
			double B = _2F1(1 + sigma * s, sigma * s, 2 + sigma * s, Fx) / _mgf(s);

			return A * B;
		}
#pragma warning(pop)

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

		// moment generating function E[e^{sX}] = pi sigma s/sin(pi sigma s)
		double _mgf(double s, unsigned n = 0) const override
		{
			// 1 + sigma s > 0 and 1 - sigma s > 0
			if (fabs(s * sigma) >= 1) {
				return std::numeric_limits<double>::quiet_NaN();
			}

			double pss = M_PI * sigma * s;

			if (s == 0) {
				return n == 0 ? 1
					: std::numeric_limits<double>::quiet_NaN(); //!!! fix
			}

			double spss = sin(pss);
			double cpss = cos(pss);

			return n == 0 ? pss / spss 
				: n == 1 ? M_PI * sigma * (spss - pss * cpss)/(spss * spss)
				: std::numeric_limits<double>::quiet_NaN();
		}
		double _cgf(double s, unsigned n = 0) const override
		{
			return n == 0 ? log(_mgf(s))
				: n == 1 ? _mgf(s, 1) / _mgf(s)
				: std::numeric_limits<double>::quiet_NaN();
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