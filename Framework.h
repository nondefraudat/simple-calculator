#ifndef NONDEFRAUDAT_SIMPLE_CALCULATOR_FRAMEWORK_H_
#define NONDEFRAUDAT_SIMPLE_CALCULATOR_FRAMEWORK_H_

#include "deps\string_math.h"

wchar_t* Calculate(const wchar_t* expression) {
	return nd_sm::calculate(expression).c_wstr();
}

#endif