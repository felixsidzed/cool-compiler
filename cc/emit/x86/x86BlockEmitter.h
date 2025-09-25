#pragma once

#include "x86Target.h"
#include "../../vector.h"
#include "../BlockEmitter.h"

namespace cc {
	class x86BlockEmitter : public BlockEmitter {
	public:
		void emit(EmissionContext* ctx_, Block* block) override;
		void emitValue(EmissionContext* ctx_, Value* val) override;
	};
}
