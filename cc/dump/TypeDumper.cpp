#include "../gen/Values.h"
#include "../gen/ComplexTypes.h"
#include "../gen/FunctionType.h"

namespace cc {
	string Type::dump() {
		switch (typeId) {
			case VoidTypeID:
				return "void";
			case BooleanTypeID:
				return "bool";
			case FloatTypeID:
				return "float";
			case DoubleTypeID:
				return "double";
			default:
				return "unknown";
		}
	}

	string IntegerType::dump() {
		string result;
		if (!sign)
			result += "unsigned ";
		switch (width) {
			case 1:
				return "bool";
			case 8:
				return "char";
			case 16:
				return "short";
			case 32:
				return "int";
			case 64:
				return "long";
			default:
				return "unknown";
		}
	}

	string PointerType::dump() {
		if (pointee)
			return pointee->dump() + "*";
		return "unknown*";
	}

	string FunctionType::dump() {
		string result = "func(";

		for (uint32_t i = 0; i < args.size; ++i) {
			if (i > 0) result += ", ";
			result += args[i]->dump();
		}

		if (vararg) {
			if (!args.empty())
				result += ", ";
			result += "...";
		}

		if (returnType) {
			result += ") -> ";
			result += returnType->dump();
		} else
			result += ") -> void";

		return result;
	}
}
