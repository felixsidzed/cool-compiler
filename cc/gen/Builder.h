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

		RetInstruction* ret(Value* val);
		AllocaInstruction* alloca_(Type* type, const char* name = "");
		StoreInstruction* store(Value* ptr, Value* val, const char* name = "");
	};
}
