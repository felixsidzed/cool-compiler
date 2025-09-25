#include "Function.h"

#include <string>

#include "Context.h"

namespace cc {
	Block* Function::appendBlock(const char* name) {
		return blocks.emplace(scope.add(name), this);
	}

	Value* Function::getArg(uint32_t arg) {
		if (!argsBuilt) {
			args.reserve(((FunctionType*)type)->args.size);
			uint32_t i = 0;
			for (auto& argt : ((FunctionType*)type)->args) {
				new (&args[i]) Argument(argt, scope.add(""), i++);
				args.size++;
			}
			argsBuilt = true;
		}
		return &args[arg];
	}
}
