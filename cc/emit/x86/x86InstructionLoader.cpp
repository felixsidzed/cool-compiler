#include "../../gen/Instruction.h"

#include "x86Target.h"
#include "x86BlockEmitter.h"
#include "x86EmissionContext.h"

namespace cc {
	void AllocaInstruction::load(EmissionContext* ctx_) {
		x86EmissionContext* ctx = (x86EmissionContext*)ctx_;

		uint32_t offset = ((x86BlockEmitter*)ctx->blockEmitter)->allocaMap[this];

		if (ctx->target->arch.is64) {
			uint8_t rex = 0x48;
			if (ctx->greg >= GREG_R8) rex |= 0x1;
			ctx->buffer.push_back(rex);

			ctx->buffer.push_back(0x8D);							// lea r64, r/m64
			ctx->buffer.push_back(0x84 | ((ctx->greg & 7) << 3));	// modr/m
			ctx->buffer.push_back(0x24);							// sib
			ctx->target->encode(ctx, offset, 32);
		} else {
			ctx->buffer.push_back(0x8D);							// lea r32, r/m32
			ctx->buffer.push_back(0x84 | ((ctx->greg & 7) << 3));	// modr/m
			ctx->buffer.push_back(0x24);							// sib
			ctx->target->encode(ctx, offset, 32);
		}
	}
}
