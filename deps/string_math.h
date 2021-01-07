#ifndef NONDEFRAUDAT_STRING_MATH_H_
#define NONDEFRAUDAT_STRING_MATH_H_

#define IMPORT __declspec(dllimport)

#include "expression.h"

namespace nd_sm {
	expression IMPORT calculate(expression);
}

#endif