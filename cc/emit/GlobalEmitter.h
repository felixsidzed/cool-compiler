#pragma once

#include "x86Target.h"
#include "../vector.h"
#include "../gen/Global.h"

namespace cc {
	class GlobalEmitter {
	public:
		virtual void emit(EmissionContext* ctx_, Global* global) = 0;
	};
}
