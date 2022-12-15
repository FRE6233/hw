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

AddIn xai_bsm_put_value(
	Function(XLL_DOUBLE, "xll_bsm_put_value", "BSM.PUT.VALUE")
	.Arguments({
		Arg(XLL_DOUBLE, "r_", "is the risk-neutral rate."),
		Arg(XLL_DOUBLE, "s0", "is the spot."),
		Arg(XLL_DOUBLE, "sigma", "is the volatility."),
		Arg(XLL_DOUBLE, "k", "is the strike."),
		Arg(XLL_DOUBLE, "t", "is the time in years to expiration."),
		Arg(XLL_HANDLEX, "m", "is a handle to the underlying distribution. Default is normal.")
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the forward put option value.")
);
double WINAPI xll_bsm_put_value(double r, double s0, double sigma, double k, double t, HANDLEX m)
{
#pragma XLLEXPORT
	distribution* pm = m ? safe_pointer<distribution>(m) : &normal;

	return option::bsm::value(r, s0, sigma, option::bsm::put{k, t}, *pm);
}

//!!! implement BSM.CALL.VALUE

AddIn xai_bsm_put_delta(
	Function(XLL_FPX, "xll_bsm_put_delta", "BSM.PUT.DELTA")
	.Arguments({
		Arg(XLL_DOUBLE, "r_", "is the risk-neutral rate."),
		Arg(XLL_FPX, "s0", "is the spot."),
		Arg(XLL_FPX, "sigma", "is the volatility."),
		Arg(XLL_FPX, "k", "is the strike."),
		Arg(XLL_FPX, "t", "is the time in years to expiration."),
		Arg(XLL_HANDLEX, "m", "is a handle to the underlying distribution. Default is normal.")
		})
	.Category(CATEGORY)
	.FunctionHelp("Return the forward put option value.")
);
_FPX* WINAPI xll_bsm_put_delta(double r, _FPX* ps0, _FPX* psigma, _FPX* pk, _FPX* pt, HANDLEX m)
{
#pragma XLLEXPORT
	static FPX result;

	try {
		result[0] = option::NaN;
		distribution* pm = m ? safe_pointer<distribution>(m) : &normal;

		int ns0 = size(*ps0);
		int nsigma = size(*psigma);
		int nk = size(*pk);
		int nt = size(*pt);
		int n = std::max({ ns0, nsigma, nk, nt });
		result.resize(n, 1);
		for (int i = 0; i < n; ++i) {
			result[i] = option::bsm::delta(r, 
				ps0->array[i%ns0], 
				psigma->array[i%nsigma], 
				option::bsm::put{pk->array[i%nk], pt->array[i % nt]},
				*pm);
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return result.get();
}

//!!! implement BSM.CALL.DELTA based on BSM.PUT.DELTA

