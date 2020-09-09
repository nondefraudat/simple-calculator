#ifndef NONDEFRAUDAT_EXPIREMENTS_STRING_MATH_EXPRESSION_H_
#define NONDEFRAUDAT_EXPIREMENTS_STRING_MATH_EXPRESSION_H_

#include "lexem.h"
#include <list>
#include <map>
#include <cmath>

namespace string_math {

	using LexemChain = std::list<Lexem>;

	class Expression {
		LexemChain lexem_chain_;

	public:
		Expression();
		Expression(const char* source_defintion);
		Expression(const wchar_t* source_definieion);

		Expression Solve();
		Expression ExtractSolution();

		char* ConvertToCString();
		wchar_t* ConvertToWString();
	};
}

#endif