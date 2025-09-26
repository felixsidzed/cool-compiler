#include "../gen/Instruction.h"

#include <string>

namespace cc {
	string RetInstruction::dump(bool flag) {
		std::string result = "ret ";
		result += val->type->dump();
		result.append(1, ' ');
		if (val->vk == InstructionValueKind)
			result += ((ValueInstruction*)val)->dump(true);
		else
			result += val->dump();
		return result.c_str();
	}

	string AllocaInstruction::dump(bool flag) {
		std::string result = name;
		if (!flag) {
			result += " = alloca ";
			result += allocated->dump();
		}
		return result.c_str();
	}

	string StoreInstruction::dump(bool flag) {
		std::string result = "store ";

		result += ptr->type->dump();
		result.append(1, ' ');
		result += ptr->dump();

		result.append(1, ',');
		result.append(1, ' ');

		result += val->type->dump();
		result.append(1, ' ');
		if (val->vk == InstructionValueKind)
			result += ((ValueInstruction*)val)->dump(true);
		else
			result += val->dump();

		return result.c_str();
	}
}
