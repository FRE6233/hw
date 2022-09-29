﻿// xll_option.cpp - General option pricing
#include "fms_distribution_normal.h"
#include "fms_option.h"
#include "xll_FRE6233.h"

using namespace xll;
using namespace fms;

static distribution_normal normal;

AddIn xai_option_moneyness(
	Function(XLL_DOUBLE, "xll_option_moneyness", "OPTION.MONEYNESS")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward."),
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
		Arg(XLL_DOUBLE, "f", "is the forward."),
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

//!!! AddIn xai_option_implied
//!!! use auto [s, ds, n] = option::implied to get all outputs, return s

AddIn xai_option_implied(
	Function(XLL_DOUBLE, "xll_option_implied", "OPTION.IMPLIED")
	.Arguments({
		Arg(XLL_DOUBLE, "f", "is the forward."),
		Arg(XLL_DOUBLE, "p", "is the put price."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
		Arg(XLL_DOUBLE, "s", "is the initial implied vol guess."),
		Arg(XLL_WORD, "n", "is the maximum number of iterations."),
		Arg(XLL_DOUBLE, "tol", "is the convergence tolerance.")
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the implied vol given put price.")
);
double WINAPI xll_option_implied(double f, double p, double k, 
	double s, WORD n, double tol)
{
#pragma XLLEXPORT
	auto [vol, err, iter] = option::implied(f, p, k, s, n, tol);

	return vol;
}

