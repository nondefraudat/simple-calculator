#include "string_math.h"

namespace string_math {

	inline Expression ExtractSolution(Expression source_expression) {
		return source_expression.ExtractSolution();
	}

	char* ExtractSolution(const char* source_expression) {
		return ExtractSolution(Expression(source_expression)).ConvertToCString();
	}

	wchar_t* ExtractSolution(const wchar_t* source_expression) 	{
		return ExtractSolution(Expression(source_expression)).ConvertToWString();
	}
}