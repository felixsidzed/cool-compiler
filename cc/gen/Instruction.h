#pragma once

#include "Values.h"

namespace cc {
	class Function;
	struct EmissionContext;

	enum OpCode : uint8_t {
		OP_RET,
		OP_ALLOCA,
		OP_STORE
	};

	class Instruction {
	public:
		Function* parent;
		OpCode op;

		Instruction(OpCode op, Function* parent) : op(op), parent(parent) {};

		virtual string dump(bool flag = false) = 0;
	};

	class RetInstruction : public Instruction {
	public:
		Value* val;

		RetInstruction(Value* val, Function* parent) : Instruction(OP_RET, parent), val(val) {};

		string dump(bool flag) override;
	};

	class ValueInstruction : public Instruction, public Value {
	public:
		using Instruction::dump;

		ValueInstruction(OpCode op, Function* parent, Type* type) : Value(type, nullptr, InstructionValueKind), Instruction(op, parent) {};

		virtual void load(EmissionContext* ctx) = 0;
	};

	class AllocaInstruction : public ValueInstruction {
	public:
		Type* allocated;

		AllocaInstruction(Type* type, Function* parent) : ValueInstruction(OP_ALLOCA, parent, (Type*)cc::Type::getPointer(type)), allocated(type) {};

		void load(EmissionContext* ctx) override;

		string dump(bool flag) override;
	};

	class StoreInstruction : public Instruction {
	public:
		Value* ptr;
		Value* val;

		StoreInstruction(Value* ptr, Value* val, Function* parent) : Instruction(OP_STORE, parent), ptr(ptr), val(val) {};
		
		string dump(bool flag) override;
	};
}
