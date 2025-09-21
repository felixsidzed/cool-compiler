#include "../gen/Block.h"

#include <string>

namespace cc {
	string Block::dump() {
		std::string result;
		for (auto insn : instructions)
			result += insn->dump();
		return result.c_str();
	}
}
