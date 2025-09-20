#include <string>

#include "../gen/Constant.h"
#include "../gen/ComplexTypes.h"

namespace cc {
	string ConstantInteger::dump() {
		if (((IntegerType*)type)->sign)
			return std::to_string((long long)val).c_str();
		else
			return std::to_string(val).c_str();
	}
}
