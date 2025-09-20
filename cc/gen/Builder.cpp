#include "Builder.h"

namespace cc {
	void Builder::insertInto(Block* b) {
		block = b;
	}

	void Builder::ret(Value* val) {
		if (!block) return;
		block->terminator = block->instructions.push<RetInstruction>(val, block->parent);
		block = nullptr;
	}
}
