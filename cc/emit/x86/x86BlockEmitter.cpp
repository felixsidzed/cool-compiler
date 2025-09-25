#include "x86BlockEmitter.h"

#include <Windows.h>

#include "../../gen/Block.h"
#include "x86EmissionContext.h"
#include "../../gen/Constant.h"
#include "../../gen/Function.h"
#include "x86InstructionEmitter.h"
#include "../../gen/Instruction.h"
#include "../../gen/ComplexTypes.h"

namespace cc {
	void x86BlockEmitter::emit(EmissionContext* ctx_, Block* block) {
		x86EmissionContext* ctx = (x86EmissionContext*)ctx_;

		for (auto insn_ : block->instructions) {
			switch (insn_->op) {
				case OP_RET: {
					auto insn = (RetInstruction*)insn_;
					ctx->greg = GREG_AX;
					emitValue(ctx, insn->val);
					ctx->buffer.push_back(0xC3);
				} break;

				default:
					ctx->buffer.push_back(0xCC);
					break;
			}
		}
	}

	void x86BlockEmitter::emitValue(EmissionContext* ctx_, Value* val_) {
		x86EmissionContext* ctx = (x86EmissionContext*)ctx_;

		switch (val_->vk) {
			case ConstantValueKind: {
				switch (val_->type->typeId) {
					case IntegerTypeID: {
						uint8_t width = ((IntegerType*)val_->type)->width;
						x86InstructionEmitter::imov(ctx, size2reg(ctx->greg, width), ((ConstantInteger*)val_)->val, width);
					} break;

					case FloatTypeID: {
						x86InstructionEmitter::fpmov(ctx, size2reg(ctx->greg, 32), ((ConstantFP*)val_)->val, 32);
					} break;

					case DoubleTypeID: {
						x86InstructionEmitter::fpmov(ctx, size2reg(ctx->greg, 64), ((ConstantFP*)val_)->val, 64);
					} break;

					default:
						goto invalid;
				}
			} break;

			case ArgumentValueKind: {
				x86InstructionEmitter::ldarg(ctx, ctx->greg, (Argument*)val_);
			} break;

			default:
			invalid:
				ctx->buffer.push_back(0xCC);
				break;
		}
	}
}
