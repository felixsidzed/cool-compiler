#pragma once

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

		linked_list<Instruction> instructions;
		
		Block(const char* name, Function* parent) : name(name), parent(parent), terminator(nullptr) {};

		string dump();
	};
}
