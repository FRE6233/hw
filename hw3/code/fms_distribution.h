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
		// cumulative distribution function
		double cdf(double x, double s = 0) const
		{
			return _cdf(x, s);
		}
		// moment generating function
		double mgf(double s) const
		{
			return _mgf(s);
		}
		// cumulant generating function
		double cgf(double s) const
		{
			return _cgf(s);
		}
	private:
		virtual double _pdf(double x, double s) const = 0;
		virtual double _cdf(double x, double s) const = 0;
		virtual double _mgf(double s) const = 0;
		virtual double _cgf(double s) const = 0;
	};
	
}
