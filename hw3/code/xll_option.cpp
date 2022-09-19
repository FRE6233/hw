// xll_option.cpp - General option pricing
#include "fms_distribution_normal.h"
#include "fms_option.h"
#include "xll_FRE6233.h"

using namespace xll;
using namespace fms;

static distribution_normal normal;

AddIn xai_option_moneyness(
	Function(XLL_DOUBLE, "xll_option_moneyness", "OPTION.MONEYNESS")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forwar4d."),
		Arg(XLL_DOUBLE, "s", "is the vol."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
		Arg(XLL_HANDLEX, "m", "is a handle to the underlying distribution. Default is normal.")
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the moneyness.")
);
double WINAPI xll_option_moneyness(double f, double s, double k, HANDLEX m)
{
#pragma XLLEXPORT
	distribution* pm = m ? safe_pointer<distribution>(m) : &normal;

	return option::moneyness(f, s, k, *pm);
}

AddIn xai_option_put(
	Function(XLL_DOUBLE, "xll_option_put", "OPTION.PUT")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forwar4d."),
		Arg(XLL_DOUBLE, "s", "is the vol."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
		Arg(XLL_HANDLEX, "m", "is a handle to the underlying distribution. Default is normal.")
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the forward put option value.")
);
double WINAPI xll_option_put(double f, double s, double k, HANDLEX m)
{
#pragma XLLEXPORT
	distribution* pm = m ? safe_pointer<distribution>(m) : &normal;

	return option::put(f, s, k, *pm);
}
