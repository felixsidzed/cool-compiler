#pragma once

#include "Scope.h"
#include "Values.h"
#include "Instruction.h"
#include "../linked_list.h"

namespace cc {
	class Function;

	class Block {
	public:
		Function* parent;
		const char* name;
		Instruction* terminator;

		Scope scope;

		linked_list<Instruction> instructions;
		
		Block(const char* name, Function* parent) : name(name), parent(parent), terminator(nullptr) {};

		string dump();
	};
}
