#pragma once

#include "Scope.h"
#include "Block.h"
#include "../vector.h"
#include "FunctionType.h"

namespace cc {
	class Module;

	class Function : public Value {
	public:
		Module* parent;
		FunctionType* ftype;

		Scope scope;
		vector<Block> blocks;
		
		Function(const char* name, FunctionType* ftype, Module* parent)
			: Value(ftype, name, GlobalValueKind), ftype(ftype), parent(parent) {};

		Block* appendBlock(const char* name);

		string dump();
	};
}
