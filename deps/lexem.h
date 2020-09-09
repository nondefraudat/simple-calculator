#ifndef NONDEFRAUDAT_EXPIREMENTS_STRING_MATH_LEXEM_H_
#define NONDEFRAUDAT_EXPIREMENTS_STRING_MATH_LEXEM_H_

#include <iostream>

namespace string_math {

	enum class LexemType {
		kUndefined,
		kNumber,
		kOpenBracket,
		kCloseBracket,
		kOperation,
		kMathFunction
	};

	enum class Priority {
		kUndefined,
		kLastOrder,
		kThirdOrder,
		kSecondOrder,
		kFirstOrder
	};

	using Number = double;


	struct TextDefintion {
		const char* text_definition;
		explicit TextDefintion(const char* text_definition);
	};

	struct OpenBracket : TextDefintion {
		explicit OpenBracket(const char* text_definition);
	};

	struct CloseBracket : TextDefintion {
		const char* text_definition_of_friendly_bracket;
		explicit CloseBracket(const char* text_definition, const char* text_definition_of_friendly_bracket);
	};

	struct Operation : TextDefintion {
		Number(*operation_hendler)(Number, Number);
		explicit Operation(const char* text_definition, Number(*operation_hendler)(Number, Number));
	};

	struct MathFunction : TextDefintion {
		Number(*math_function_hendler)(Number);
		explicit MathFunction(const char* text_definition, Number(*math_function_hendler)(Number));
	};


	class Lexem {
		LexemType type_;
		Priority priority_;
		union {
			Number number_;
			OpenBracket open_bracket_;
			CloseBracket close_bracket_;
			Operation operation_;
			MathFunction math_function_;
		};

	protected:
		explicit Lexem(LexemType, Priority);

	public:
		explicit Lexem();

		explicit Lexem(Number);
		explicit Lexem(OpenBracket);
		explicit Lexem(CloseBracket);
		explicit Lexem(Operation, Priority);
		explicit Lexem(MathFunction);

		inline bool IsNumber();
		inline bool IsOpenBracket();
		inline bool IsCloseBracket();
		inline bool IsOperation();
		inline bool IsMathFunction();

		LexemType GetType();
		Priority GetPriority();

		const char* GetDefinition();

		Number GetNumber();
		OpenBracket GetOpenBracket();
		CloseBracket GetCloseBracket();
		Operation GetOperation();
		MathFunction GetMathFunction();

		Lexem& operator=(const Lexem&);
	};
}

#endif
