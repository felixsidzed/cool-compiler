#pragma once

#include <cstdint>

#include "Scope.h"
#include "Block.h"
#include "Global.h"
#include "../vector.h"
#include "FunctionType.h"

namespace cc {
	class Module;

	class Argument : public Value {
	public:
		uint8_t idx;

		Argument(Type* type, const char* name, uint8_t idx) : Value(type, name, ArgumentValueKind), idx(idx) {};
	};

	class Function : public Global {
	public:
		Module* parent;

		Scope scope;
		vector<Block> blocks;
		vector<Argument> args;
		
		Function(const char* name, FunctionType* ftype, Module* parent) : Global(PublicLinkage, this, ftype, name), parent(parent) {};
		
		Value* getArg(uint32_t arg);
		Block* appendBlock(const char* name);

		string dump();
	private:
		bool argsBuilt : 1 = false;
	};
}
