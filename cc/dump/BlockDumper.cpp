#include "../gen/Block.h"

#include <string>

namespace cc {
	string Block::dump() {
		std::string result;
		for (auto insn : instructions) {
			result += insn->dump();
			result.append(1, '\n');
		}
		return result.c_str();
	}
}
