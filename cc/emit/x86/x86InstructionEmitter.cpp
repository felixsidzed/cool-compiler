#include "x86InstructionEmitter.h"

#include "../Target.h"

namespace cc {
	void x86InstructionEmitter::integerMov(x86EmissionContext* ctx, x86Register reg, uint64_t val, uint8_t width) {
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
				ctx->buffer.push_back(0x48);
				ctx->buffer.push_back(0xB8 | (reg & 7));
				ctx->target->encode(ctx, val, width);
			}
		}
	}
}

