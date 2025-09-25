#pragma once

#include "x86Target.h"
#include "../../vector.h"
#include "../GlobalEmitter.h"

namespace cc {
	class x86GlobalEmitter : public GlobalEmitter {
	public:
		void emit(EmissionContext* ctx_, Global* global) override;
	};
}
