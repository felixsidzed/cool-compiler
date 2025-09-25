#pragma once

#include <vector>

#include "Target.h"
#include "../gen/Values.h"
#include "EmissionContext.h"

namespace cc {
	class Block;

	class BlockEmitter {
	public:
		virtual void emit(EmissionContext* ctx, Block* block) = 0;
		virtual void emitValue(EmissionContext* ctx, Value* val) = 0;
	};
}
