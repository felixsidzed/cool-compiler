#include "Function.h"

#include <string>

#include "Context.h"

namespace cc {
	Block* Function::appendBlock(const char* name) {
		return blocks.emplace(scope.add(name), this);
	}
}
