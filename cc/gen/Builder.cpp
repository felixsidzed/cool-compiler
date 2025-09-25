#include "Builder.h"

#include "Function.h"

namespace cc {
	void Builder::insertInto(Block* b) {
		block = b;
	}

	void Builder::ret(Value* val) {
		if (!block || val->type != ((FunctionType*)block->parent->type)->returnType)
			return;

		block->terminator = block->instructions.push<RetInstruction>(val, block->parent);
		block = nullptr;
	}
}
