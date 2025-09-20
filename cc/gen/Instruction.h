#pragma once

#include "Values.h"

namespace cc {
	class Function;

	enum OpCode : uint8_t {
		OP_RET
	};

	class Instruction {
	public:
		Function* parent;
		OpCode op;

		Instruction(OpCode op, Function* parent) : op(op), parent(parent) {};

		virtual string dump() = 0;
	};

	class RetInstruction : public Instruction {
	public:
		Value* val;

		RetInstruction(Value* val, Function* parent) : Instruction(OP_RET, parent), val(val) {};

		string dump() override;
	};
}
