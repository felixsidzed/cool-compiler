#pragma once

#include "x86EmissionContext.h"

namespace cc {
	class Type;
	class Argument;

	uint32_t sizeOf(x86EmissionContext* ctx, Type* ty);
	x86Register size2reg(x86GeneralRegister greg, uint32_t bitsize);

	namespace x86InstructionEmitter {
		void imov(x86EmissionContext* ctx, x86Register reg, uint64_t val, uint8_t width);
		void fpmov(x86EmissionContext* ctx, x86Register reg, double val, uint8_t width);

		void ldarg(x86EmissionContext* ctx, x86GeneralRegister greg, Argument* arg);
	}
}
