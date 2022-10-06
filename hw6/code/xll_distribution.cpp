// xll_distribution.cpp - random variable distributions
#include "xll_FRE6233.h"
#include "fms_distribution_normal.h"
#include "fms_distribution_logistic.h"

using namespace xll;
using namespace fms;

static distribution_normal normal;
static distribution_logistic logistic;

AddIn xai_distribution_normal(
	Function(XLL_HANDLEX, "xll_distribution_normal", "DISTRIBUTION.NORMAL")
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to the standard normal distribution.")
);
HANDLEX WINAPI xll_distribution_normal()
{
#pragma XLLEXPORT
	return safe_handle<distribution>(&normal);
}

AddIn xai_distribution_logistic(
	Function(XLL_HANDLEX, "xll_distribution_logistic", "DISTRIBUTION.LOGISTIC")
	.Category(CATEGORY)
	.FunctionHelp("Return a handle to the standard logistic distribution.")
);
HANDLEX WINAPI xll_distribution_logistic()
{
#pragma XLLEXPORT
	return safe_handle<distribution>(&logistic);
}

AddIn xai_distribution_pdf(
	Function(XLL_DOUBLE, "xll_distribution_pdf", "DISTRIBUTION.PDF")
	.Arguments({
		Arg(XLL_DOUBLE, "x", "is the value for the probability density function."),
		Arg(XLL_DOUBLE, "s", "is the share measure parameter."),
		Arg(XLL_HANDLEX, "m", "is a handle to the distribution")
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the probability density function of a distribution.")
);
double WINAPI xll_distribution_pdf(double x, double s, HANDLEX m)
{
#pragma XLLEXPORT
	distribution* pm = m ? safe_pointer<distribution>(m) : &normal;

	return pm->pdf(x, s);
}

AddIn xai_distribution_cdf(
	Function(XLL_DOUBLE, "xll_distribution_cdf", "DISTRIBUTION.CDF")
	.Arguments({
		Arg(XLL_DOUBLE, "x", "is the value for the cumulative distribution function."),
		Arg(XLL_DOUBLE, "s", "is the share measure parameter."),
		Arg(XLL_HANDLEX, "m", "is a handle to the distribution")
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the cumulative distribution.")
);
double WINAPI xll_distribution_cdf(double x, double s, HANDLEX m)
{
#pragma XLLEXPORT
	distribution* pm = m ? safe_pointer<distribution>(m) : &normal;

	return pm->cdf(x, s);
}

AddIn xai_distribution_mgf(
	Function(XLL_DOUBLE, "xll_distribution_mgf", "DISTRIBUTION.MGF")
	.Arguments({
		Arg(XLL_DOUBLE, "s", "is the share measure parameter."),
		Arg(XLL_HANDLEX, "m", "is a handle to the distribution")
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the moment generating function.")
);
double WINAPI xll_distribution_mgf(double s, HANDLEX m)
{
#pragma XLLEXPORT
	distribution* pm = m ? safe_pointer<distribution>(m) : &normal;

	return pm->mgf(s);
}

AddIn xai_distribution_cgf(
	Function(XLL_DOUBLE, "xll_distribution_cgf", "DISTRIBUTION.CGF")
	.Arguments({
		Arg(XLL_DOUBLE, "s", "is the share measure parameter."),
		Arg(XLL_HANDLEX, "m", "is a handle to the distribution")
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the cumulant generating function.")
);
double WINAPI xll_distribution_cgf(double s, HANDLEX m)
{
#pragma XLLEXPORT
	distribution* pm = m ? safe_pointer<distribution>(m) : &normal;

	return pm->cgf(s);
}
