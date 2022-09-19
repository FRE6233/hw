// xll_FRE6233.cpp - tests
#include "fms_hypergeometric.h"
#include "fms_distribution_logistic.h"
#include "xll_FRE6233.h"

using namespace xll;

#ifdef _DEBUG

int FRE6233_test()
{
	try {
		ensure(fms::test_2F1());
		fms::distribution_logistic logistic;
		ensure(logistic.test_cdf());
		ensure(logistic.test_cgf());
		// ...
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what(), true);
	}

	return true;
}
Auto<Open> xao_test(FRE6233_test);

#endif // _DEBUG