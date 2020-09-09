#ifndef NONDEFRAUDAT_EXPIREMENTS_STRING_MATH_H_
#define NONDEFRAUDAT_EXPIREMENTS_STRING_MATH_H_

#include "expression.h"

namespace string_math {

	char* ExtractSolution(const char* source_expression);
	wchar_t* ExtractSolution(const wchar_t* source_expression);

}

#endif