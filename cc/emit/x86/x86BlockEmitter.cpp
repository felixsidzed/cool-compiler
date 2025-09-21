#include "x86BlockEmitter.h"

#include <Windows.h>

#include "../../gen/Block.h"
#include "x86EmissionContext.h"
#include "../../gen/Constant.h"
#include "x86InstructionEmitter.h"
#include "../../gen/Instruction.h"
#include "../../gen/ComplexTypes.h"

namespace cc {
	static x86Register width2reg(x86GeneralRegister greg, uint8_t width) {
		switch (width) {
			case 8:  return (x86Register)greg;
			case 16: return (x86Register)(greg + 16);
			case 32: return (x86Register)(greg + 32);
			case 64: return (x86Register)(greg + 48);
			default: return REG_AL;
		}
	}

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
				invalid:
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
						x86InstructionEmitter::integerMov(ctx, width2reg(ctx->greg, width), ((ConstantInteger*)val_)->val, width);
					} break;

					default:
						goto invalid;
				}
			} break;

			default:
			invalid:
				ctx->buffer.push_back(0xCC);
				break;
		}
	}
}
