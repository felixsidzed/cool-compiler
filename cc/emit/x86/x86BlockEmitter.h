#pragma once

#include "x86Target.h"
#include "../../vector.h"
#include "../BlockEmitter.h"

namespace cc {
	class x86BlockEmitter : public BlockEmitter {
	public:
		x86BlockEmitter(x86Target* parent) : BlockEmitter(parent) {};

		x86Target* parent() { return (x86Target*)_parent; }

		void emit(EmissionContext* ctx_, Block* block) override;
		void emitValue(EmissionContext* ctx_, Value* val) override;
	};
}
