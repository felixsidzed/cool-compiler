#pragma once

#include "Block.h"
#include "Instruction.h"

namespace cc {
	class Module;
	class Constant;

	class Builder {
	public:
		Block* block;

		Builder() : block(nullptr) {};
		
		void insertInto(Block* b);

		void ret(Value* val);
	};
}
