#include "expression.h"
#include "lexem.h"
#include <list>
#include <map>
#include <cmath>
//
#include <iostream>
//
#include <cstdlib>

namespace string_math {

	Expression::Expression() : lexem_chain_() {

	}

	LexemChain GenerateLexemChain(const char*& source_expression);
	LexemChain DiscloseValidDefinitions(LexemChain lexem_chain);
	//
	LexemChain GenerateReversePolishNotation(LexemChain& lexem_chain);
	//

	Expression::Expression(const char* source_expression) {
		lexem_chain_ = GenerateLexemChain(source_expression);
		lexem_chain_ = DiscloseValidDefinitions(lexem_chain_);
	}

	inline bool IsExist(const wchar_t*& source_expression);

	Expression::Expression(const wchar_t* source_definieion) {
		char* c_string = new char[wcslen(source_definieion)];
		
		const wchar_t* input_it = source_definieion;
		char* output_it = c_string;

		while (IsExist(input_it)) {
			*output_it++ = (*input_it++) % 256;
		}
		*output_it = '\0';
		*this = Expression(c_string);
	}

	LexemChain GenerateReversePolishNotation(LexemChain&);
	LexemChain GenerateSolutionFromRPN(LexemChain&);

	Expression Expression::Solve() {
		LexemChain rpn = GenerateReversePolishNotation(lexem_chain_);
		lexem_chain_ = GenerateSolutionFromRPN(rpn);
		return *this;
	}
	Expression Expression::ExtractSolution() {
		Expression expression = *this;
		expression.Solve();
		return expression;
	}

	char* Expression::ConvertToCString() {
		
		LexemChain::iterator input_it = lexem_chain_.begin();
		std::string output = "";
		
		while (input_it != lexem_chain_.end()) {
			output += input_it++->GetDefinition();
			output += ' ';
		}

		int length = output.length();

		char* result = new char[length + 1];
		int i;
		for (i = 0; i < length; i++) {
			result[i] = output[i];
		}
		result[i] = '\0';

		return result;
	}

	inline bool IsExist(const char*& source_expression);

	wchar_t* Expression::ConvertToWString() {
		const char* c_string = ConvertToCString();
		wchar_t* w_string = new wchar_t[strlen(c_string)];

		const char* input_it = c_string;
		wchar_t* output_it = w_string;

		while (IsExist(input_it)) {
			*output_it++ = (wchar_t)0 + *input_it++;
		}
		*output_it = L'\0';

		return w_string;
	}



	// Hidden Instructions;


	// Operation handlers

	Number add(Number a, Number b) {
		return a + b;
	}
	Number sub(Number a, Number b) {
		return a - b;
	}
	Number mul(Number a, Number b) {
		return a * b;
	}
	Number div(Number a, Number b) {
		return a / b;
	}
	Number pow(Number a, Number b) {
		return std::pow(a, b);
	}

	
	// Math Function handlers

	Number sin(Number a) {
		return std::sin(a);
	}
	Number cos(Number a) {
		return std::cos(a);
	}
	Number tan(Number a) {
		return std::tan(a);
	}
	Number sqrt(Number a) {
		return std::sqrt(a);
	}

	using LexemTemplates = std::map<std::string, Lexem>;

	static const LexemTemplates kDefaultLexemTemplates = {
		// Brackets
		{"(",
			Lexem(OpenBracket("("))
		},
		{")",
			Lexem(CloseBracket(")", "("))
		},

		// Operations
		{"+",
			Lexem(Operation("+", add), Priority::kThirdOrder)
		}, 
		{"-",
			Lexem(Operation("-", sub), Priority::kThirdOrder)
		},
		{"*",
			Lexem(Operation("*", mul), Priority::kSecondOrder)
		},
		{"/",
			Lexem(Operation("/", div), Priority::kSecondOrder)
		},
		{"^",
			Lexem(Operation("^", pow), Priority::kFirstOrder)
		},

		// Math Functions
		{"sin",
			Lexem(MathFunction("sin", sin))
		},
		{"cos",
			Lexem(MathFunction("cos", cos))
		},
		{"tan",
			Lexem(MathFunction("tan", tan))
		},
		{"sqrt",
			Lexem(MathFunction("sqrt", sqrt))
		},
	};


	char* RewriteWithoutSpaces(const char*& source_expression);
	inline bool IsExist(const char*& source_expression);
	Lexem ExtractNextLexem(const char*& source_expression);

	LexemChain GenerateLexemChain(const char*& source_expression) {
		
		LexemChain lexem_chain;
		const char* source_expression_reminder = RewriteWithoutSpaces(source_expression);
		
		while (IsExist(source_expression_reminder)) {
			lexem_chain.push_back(
				ExtractNextLexem(source_expression_reminder)
			);
		}

		return lexem_chain;
	}

	LexemChain DiscloseValidDefinitions(LexemChain lexem_chain) {
		return lexem_chain;
	}

	LexemChain GenerateReversePolishNotation(LexemChain& lexem_chain) {
		LexemChain stack;
		LexemChain rpn;
		LexemChain::iterator it = lexem_chain.begin();
		while (it != lexem_chain.end()) {
			if (it->IsCloseBracket()) {
				while (!stack.back().IsOpenBracket()) {
					rpn.push_back(stack.back());
					stack.pop_back();
					if (stack.empty()) {
						throw "BRACKET_STRUCTURE_ERROR";
					}
				}
				stack.pop_back();
			}
			else if (it->IsNumber()) {
				rpn.push_back(*it);
			}
			else if (
				it->IsOpenBracket() 
				or stack.empty() 
				or it->GetPriority() > stack.back().GetPriority()
			) {
				stack.push_back(*it);
			}
			else {
				while (!stack.empty() and stack.back().GetPriority() >= it->GetPriority()) {
					rpn.push_back(stack.back());
					stack.pop_back();
				}
				stack.push_back(*it);
			}
			it++;
		}
		while (!stack.empty()) {
			rpn.push_back(stack.back());
			stack.pop_back();
		}
		return rpn;
	}

	LexemChain GenerateSolutionFromRPN(LexemChain& lexem_chain) {
		LexemChain::iterator it = lexem_chain.begin();
		LexemChain stack;
		while (it != lexem_chain.end()) {
			if (it->IsNumber()) {
				stack.push_back(*it);
			}
			else {
				if (stack.empty()) {
					throw "NUMBER_NOT_FOUND";
				}
				Number b = stack.back().GetNumber();
				stack.pop_back();
				if (it->IsOperation()) {
					if (stack.empty()) {
						throw "NUMBER_NOT_FOUND";
					}
					Number a = stack.back().GetNumber();
					stack.pop_back();
					stack.push_back(
						Lexem(it->GetOperation().operation_hendler(a, b))
					);
				}
				else {
					stack.push_back(Lexem(it->GetMathFunction().math_function_hendler(b)));
				}
			}
			it++;
		}
		return stack;
	}

	inline bool IsExist(const char*& source_expression);
	inline bool IsSpace(char symbol);

	char* RewriteWithoutSpaces(const char*& source_expression) {

		char* result = new char[strlen(source_expression)];

		const char* input_it = source_expression;
		char* output_it = result;

		while (IsExist(input_it)) {
			if (!IsSpace(*input_it)) {
				*output_it++ = *input_it;
			}
			input_it++;
		}
		*output_it = '\0';

		return result;
	}

	inline bool IsSpace(char symbol) {
		return symbol == ' ' or symbol == '\t';
	}


	inline bool IsExist(const char*& source_expression) {
		return *source_expression != '\0';
	}
	inline bool IsExist(const wchar_t*& source_expression) {
		return *source_expression != L'\0';
	}

	inline bool IsDigit(char symbol);
	Number ExtractNumber(const char*& source_expression);
	Lexem ExtractDefinition(const char*& source_expression);

	Lexem ExtractNextLexem(const char*& source_expression) {
		if (IsExist(source_expression)) {
			if (IsDigit(*source_expression)) {
				return Lexem(ExtractNumber(source_expression));
			}
			else {
				return ExtractDefinition(source_expression);
			}
		}
	}
	
	inline bool IsDigit(char symbol) {
		return symbol >= '0' and symbol <= '9';
	}

	inline bool IsDot(char symbol) {
		return symbol == '.';
	}

	Number ExtractNumber(const char*& source_expression) {
		
		static const int MAX_NUMBER_LENGTH = 20;
		static char temp_number[MAX_NUMBER_LENGTH +1];
		
		const char* input_it = source_expression;
		char* output_it = temp_number;
		
		int dot_count = 0;
		int iteration_count = 0;

		while (IsDigit(*input_it) or IsDot(*input_it)) {
			
			iteration_count++;
			if (iteration_count == MAX_NUMBER_LENGTH) {
				throw "NUMBER_IS_TOO_LONG";
			}

			if (IsDot(*input_it)) {
				if (dot_count > 0) {
					throw "INVALID_NUMBER_FORMAT";
				}
				dot_count++;
			}

			*output_it++ = *input_it++;
		}

		*output_it = '\0';

		source_expression = input_it;

		return atof(temp_number);
	}

	int CountMaxDefinitionLength();

	Lexem ExtractDefinition(const char*& source_expression) {
		std::string temp_definition = "";
		
		const char* input_it = source_expression;

		static const int MAX_DEFINITION_LENGTH = CountMaxDefinitionLength();
		
		while (temp_definition.length() < MAX_DEFINITION_LENGTH) {
			
			temp_definition += *input_it++;
			
			static LexemTemplates::const_iterator temp_it;
			temp_it = kDefaultLexemTemplates.find(temp_definition);

			if (temp_it != kDefaultLexemTemplates.end()) {
				source_expression = input_it;
				return temp_it->second;
			}
		}

		throw "UNKNOWN_LEXEM";
	}

	int CountMaxDefinitionLength() {
		LexemTemplates::const_iterator it = kDefaultLexemTemplates.begin();

		int max_definition_length = 0;

		while (it != kDefaultLexemTemplates.end()) {
			static int temp_value;
			temp_value = it++->first.length();
			if (temp_value > max_definition_length) {
				max_definition_length = temp_value;
			}
		}

		return max_definition_length;
	}

}