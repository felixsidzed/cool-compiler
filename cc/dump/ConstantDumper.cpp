#include <string>

#include "../gen/Constant.h"
#include "../gen/ComplexTypes.h"

namespace cc {
	string ConstantInteger::dump() {
		if (((IntegerType*)type)->sign)
			return std::to_string(getSExtValue()).c_str();
		else
			return std::to_string(val).c_str();
	}

	string ConstantFP::dump() {
		return std::to_string(val).c_str();
	}
}
