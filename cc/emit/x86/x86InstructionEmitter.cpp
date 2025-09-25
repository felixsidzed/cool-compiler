#include "x86InstructionEmitter.h"

#include <cassert>

#include "../Target.h"
#include "../../gen/Values.h"
#include "../../gen/Function.h"
#include "../../gen/ComplexTypes.h"

namespace cc {
	uint32_t sizeOf(x86EmissionContext* ctx, Type* type) {
		switch (type->typeId) {
			case FloatTypeID:
				return 32;
			case DoubleTypeID:
				return 64;

			case IntegerTypeID:
				return ((IntegerType*)(type))->width;
			case FunctionTypeID:
			case PointerTypeID:
				return 8 ? ctx->target->arch.is64 : 4;
			default:
				return 0;
		}
	}

	x86Register size2reg(x86GeneralRegister greg, uint32_t bitsize) {
		switch (bitsize) {
			case 8:  return (x86Register)greg;
			case 16: return (x86Register)(greg + 16);
			case 32: return (x86Register)(greg + 32);
			case 64: return (x86Register)(greg + 48);
			default: return REG_AL;
		}
	}

	void x86InstructionEmitter::imov(x86EmissionContext* ctx, x86Register reg, uint64_t val, uint8_t width) {
		if (val == 0) {
			if (width == 8) {
				ctx->buffer.push_back(0x30);
				ctx->buffer.push_back(0xC0 | ((reg & 7) << 3) | (reg & 7));
			} else if (width == 16) {
				ctx->buffer.push_back(0x66);
				ctx->buffer.push_back(0x31);
				ctx->buffer.push_back(0xC0 | ((reg & 7) << 3) | (reg & 7));
			} else if (width == 32) {
				ctx->buffer.push_back(0x31);
				ctx->buffer.push_back(0xC0 | ((reg & 7) << 3) | (reg & 7));
			} else if (width == 64) {
				assert(ctx->target->arch.is64); // TODO: proper 32-bit support

				ctx->buffer.push_back(0x48);
				ctx->buffer.push_back(0x31);
				ctx->buffer.push_back(0xC0 | ((reg & 7) << 3) | (reg & 7));
			}
		} else {
			if (width == 8) {
				ctx->buffer.push_back(0xB0 | (reg & 7));
				ctx->buffer.push_back((uint8_t)val);
			} else if (width == 16) {
				ctx->buffer.push_back(0x66);
				ctx->buffer.push_back(0xB8 | (reg & 7));
				ctx->target->encode(ctx, val, width);
			} else if (width == 32) {
				ctx->buffer.push_back(0xB8 | (reg & 7));
				ctx->target->encode(ctx, val, width);
			} else if (width == 64) {
				assert(ctx->target->arch.is64); // TODO: proper 32-bit support

				ctx->buffer.push_back(0x48);
				ctx->buffer.push_back(0xB8 | (reg & 7));
				ctx->target->encode(ctx, val, width);
			}
		}
	}

	void x86InstructionEmitter::fpmov(x86EmissionContext* ctx, x86Register reg, double val, uint8_t width) {
		if (width == 32) {
			union {
				float f;
				uint32_t n;
			} conv;
			conv.f = (float)val;

			ctx->buffer.push_back(0xB8 | (reg & 7));
			ctx->target->encode(ctx, conv.n, width);
		} else if (width == 64) {
			assert(ctx->target->arch.is64); // TODO: proper 32-bit support

			union {
				double f;
				uint64_t n;
			} conv;
			conv.f = val;

			ctx->buffer.push_back(0x48);
			ctx->buffer.push_back(0xB8 | (reg & 7));
			ctx->target->encode(ctx, conv.n, width);
		}
	}

	void x86InstructionEmitter::ldarg(x86EmissionContext* ctx, x86GeneralRegister greg, Argument* arg) {
		uint8_t bitsize = sizeOf(ctx, arg->type);

		uint8_t stackAlignment = 8 ? ctx->target->arch.is64 : 4;

		switch (bitsize) {
			case 8: {
				if (arg->idx < 2) {
					uint8_t src = (arg->idx == 0) ? 1 : 2;
					ctx->buffer.push_back(0x8A);
					ctx->buffer.push_back(0xC0 | ((greg & 7) << 3) | (src & 7));
				} else {
					ctx->buffer.push_back(0x8A);
					ctx->buffer.push_back(0x84 | ((greg & 7) << 3));
					ctx->buffer.push_back(0x24);
					ctx->buffer.push_back(stackAlignment * (arg->idx - 1));
				}
			} break;

			case 16: {
				ctx->buffer.push_back(0x66);
				if (arg->idx < 2) {
					uint8_t src = (arg->idx == 0) ? 1 : 2;
					ctx->buffer.push_back(0x8B);
					ctx->buffer.push_back(0xC0 | ((greg & 7) << 3) | (src & 7));
				} else {
					ctx->buffer.push_back(0x8B);
					ctx->buffer.push_back(0x84 | ((greg & 7) << 3));
					ctx->buffer.push_back(0x24);
					ctx->buffer.push_back(stackAlignment * (arg->idx - 1));
				}
			} break;

			case 32: {
				if (arg->idx < 2) {
					uint8_t src = (arg->idx == 0) ? 1 : 2;
					ctx->buffer.push_back(0x8B);
					ctx->buffer.push_back(0xC0 | ((greg & 7) << 3) | (src & 7));
				} else {
					ctx->buffer.push_back(0x8B);
					ctx->buffer.push_back(0x84 | ((greg & 7) << 3));
					ctx->buffer.push_back(0x24);
					ctx->buffer.push_back(stackAlignment * (arg->idx - 1));
				}
			} break;

			case 64: {
				uint8_t rex = 0x48;
				if (greg >= GREG_R8) rex |= 0x4;

				if (arg->idx < 4) {
					const static x86Register regs[] = { REG_RCX, REG_RDX, REG_R8, REG_R9 };

					if (arg->idx > 2) rex |= 0x1;
					
					ctx->buffer.push_back(rex);
					ctx->buffer.push_back(0x8B);
					ctx->buffer.push_back(0xC0 | ((greg & 7) << 3) | (regs[arg->idx] & 7));
				} else {
					ctx->buffer.push_back(rex);
					ctx->buffer.push_back(0x8B);
					ctx->buffer.push_back(0x84 | ((greg & 7) << 3));
					ctx->buffer.push_back(0x24);
					ctx->buffer.push_back(stackAlignment * (arg->idx - 3));
				}
			} break;

			default:
				ctx->buffer.push_back(0xCC); // int3
				break;
		}
	}
}
