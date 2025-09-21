#pragma once

#include "x86EmissionContext.h"

namespace cc::x86InstructionEmitter {
	void integerMov(x86EmissionContext* ctx, x86Register reg, uint64_t val, uint8_t width);
}
