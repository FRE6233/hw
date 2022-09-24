// fms_distribution.h - Random variable distributions.
#pragma once

namespace fms {

	// interface to distributions
	struct distribution {
		virtual ~distribution() { }

		// probability density function
		double pdf(double x, double s = 0) const
		{
			return _pdf(x, s);
		}
		// cumulative distribution function and derivatives
		double cdf(double x, double s = 0, unsigned nx = 0, unsigned ns = 0) const
		{
			return _cdf(x, s, nx, ns);
		}
		// moment generating function
		double mgf(double s, unsigned n = 0) const
		{
			return _mgf(s, n);
		}
		// cumulant generating function
		double cgf(double s, unsigned n = 0) const
		{
			return _cgf(s, n);
		}
	private:
		virtual double _pdf(double x, double s) const = 0;
		virtual double _cdf(double x, double s, unsigned nx, unsigned ns) const = 0;
		virtual double _mgf(double s, unsigned n) const = 0;
		virtual double _cgf(double s, unsigned n) const = 0;
	};
	
}
