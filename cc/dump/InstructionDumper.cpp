#include "../gen/Instruction.h"

#include <string>

namespace cc {
	string RetInstruction::dump() {
		std::string result = "ret ";
		result += val->type->dump();
		result += " ";
		result += val->dump();
		return result.c_str();
	}
}
