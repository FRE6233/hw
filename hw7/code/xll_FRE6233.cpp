// xll_FRE6233.cpp - tests
#include "fms_hypergeometric.h"
#include "fms_distribution_logistic.h"
#include "fms_option.h"
#include "xll_FRE6233.h"

using namespace xll;
using namespace fms;

#ifdef _DEBUG

int FRE6233_test()
{
	try {
		ensure(test_2F1());
		distribution_logistic logistic;
		ensure(logistic.test_cdf());
		ensure(logistic.test_cgf());
		ensure(option::test_implied());
		ensure(option::test_put_delta());
		// ...
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what(), true);
	}

	return true;
}
Auto<Open> xao_test(FRE6233_test);

#endif // _DEBUG