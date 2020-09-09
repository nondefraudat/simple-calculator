#include "lexem.h"
#include <iostream>

namespace string_math {

	Lexem::Lexem(LexemType type, Priority priority) {
		type_ = type;
		priority_ = priority;
	}

	Lexem::Lexem() : Lexem(LexemType::kUndefined, Priority::kUndefined) {
	
	}
	Lexem::Lexem(Number number) : Lexem(LexemType::kNumber, Priority::kUndefined) {
		number_ = number;
	}
	Lexem::Lexem(OpenBracket open_bracket) : Lexem(LexemType::kOpenBracket, Priority::kLastOrder) {
		open_bracket_ = open_bracket;
	}
	Lexem::Lexem(CloseBracket close_bracket) : Lexem(LexemType::kCloseBracket, Priority::kThirdOrder) {
		close_bracket_ = close_bracket;
	}
	Lexem::Lexem(Operation operation, Priority priority) : Lexem(LexemType::kOperation, priority) {
		operation_ = operation;
	}
	Lexem::Lexem(MathFunction math_function) : Lexem(LexemType::kMathFunction, Priority::kFirstOrder) {
		math_function_ = math_function;
	}


	inline bool Lexem::IsNumber() {
		return type_ == LexemType::kNumber;
	}
	inline bool Lexem::IsOpenBracket() {
		return type_ == LexemType::kOpenBracket;
	}
	inline bool Lexem::IsCloseBracket() {
		return type_ == LexemType::kCloseBracket;
	}
	inline bool Lexem::IsOperation() {
		return type_ == LexemType::kOperation;
	}
	inline 	bool Lexem::IsMathFunction() {
		return type_ == LexemType::kMathFunction;
	}

	LexemType Lexem::GetType() {
		return type_;
	}
	Priority Lexem::GetPriority() {
		return priority_;
	}

	const char* Lexem::GetDefinition() {
		if (IsNumber()) {
			static char* buffer = new char[21];
			sprintf_s(buffer, 21, "%f", number_);
			return buffer;
		}
		else {
			switch (type_) {
			case string_math::LexemType::kUndefined:
				return "#?";
			case string_math::LexemType::kOpenBracket:
				return open_bracket_.text_definition;
			case string_math::LexemType::kCloseBracket:
				return close_bracket_.text_definition;
			case string_math::LexemType::kOperation:
				return operation_.text_definition;
			case string_math::LexemType::kMathFunction:
				return math_function_.text_definition;
			}
		}
	}

	Number Lexem::GetNumber() {
		if (IsNumber()) {
			return number_;
		}
		throw "NOT_A_NUMBER";
	}
	OpenBracket Lexem::GetOpenBracket() {
		if (IsOpenBracket()) {
			return open_bracket_;
		}
		throw "NOT_AN_OPEN_BRACKET";
	}
	CloseBracket Lexem::GetCloseBracket() {
		if (IsCloseBracket()) {
			return close_bracket_;
		}
		throw "NOT_A_CLOSE_BRACKET";
	}
	Operation Lexem::GetOperation() {
		if (IsOperation()) {
			return operation_;
		}
		throw "NOT_AN_OPERATION";
	}
	MathFunction Lexem::GetMathFunction() {
		if (IsMathFunction()) {
			return math_function_;
		}
		throw "NOT_A_MATH_FUNCTION";
	}

	Lexem& Lexem::operator=(const Lexem& lexem) {
		type_ = lexem.type_;
		priority_ = lexem.priority_;
		number_ = lexem.number_;
		open_bracket_ = lexem.open_bracket_;
		close_bracket_ = lexem.close_bracket_;
		operation_ = lexem.operation_;
		math_function_ = lexem.math_function_;
		
		return *this;
	}

	TextDefintion::TextDefintion(const char* text_definition) {
		this->text_definition = text_definition;
	}
	OpenBracket::OpenBracket(const char* text_definition) 
		: TextDefintion(text_definition) {

	}
	CloseBracket::CloseBracket(const char* text_definition, const char* text_definition_of_friendly_bracket) 
		: TextDefintion(text_definition) {
		this->text_definition_of_friendly_bracket = text_definition_of_friendly_bracket;
	}
	Operation::Operation(const char* text_definition, Number(*operation_hendler)(Number, Number)) 
		: TextDefintion(text_definition) {
		this->operation_hendler = operation_hendler;
	}
	MathFunction::MathFunction(const char* text_definition, Number(*math_function_hendler)(Number)) 
		: TextDefintion(text_definition) {
		this->math_function_hendler = math_function_hendler;
	}
}