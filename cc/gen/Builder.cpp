#include "Builder.h"

#include <cassert>

#include "Function.h"
#include "ComplexTypes.h"

namespace cc {
	void Builder::insertInto(Block* b) {
		block = b;
	}

	RetInstruction* Builder::ret(Value* val) {
		if (!block || val->type != ((FunctionType*)block->parent->type)->returnType)
			return nullptr;

		RetInstruction* insn = block->instructions.push<RetInstruction>(val, block->parent);
		block->terminator = insn;
		block = nullptr;
		return insn;
	}

	AllocaInstruction* Builder::alloca_(Type* type, const char* name) {
		if (!block || type->typeId == VoidTypeID)
			return nullptr;

		AllocaInstruction* insn = block->instructions.push<AllocaInstruction>(type, block->parent);
		insn->name = block->scope.add(name);
		return insn;
	}

	StoreInstruction* Builder::store(Value* ptr, Value* val, const char* name) {
		assert(ptr->type->typeId == PointerTypeID);
		assert(((PointerType*)ptr->type)->pointee == val->type);

		return block->instructions.push<StoreInstruction>(ptr, val, block->parent);
	}
}
