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
					ctx->buffer.push_back(0xC3);		// ret
				} break;	

				case OP_ALLOCA: {
					auto insn = (AllocaInstruction*)insn_;
					uint32_t size = (sizeOf(ctx, insn->allocated) + 7) / 8;

					uint32_t align = ctx->target->arch.is64 ? 8 : 4;
					size = (size + (align - 1)) & ~(align - 1);

					allocaMap[insn] = allocaOffset;
					allocaOffset += size;

					if (size == 0)
						break;

					if (ctx->target->arch.is64) {
						ctx->buffer.push_back(0x48);	// rex.w
						ctx->buffer.push_back(0x81);	// sub r/m64, imm32
						ctx->buffer.push_back(0xEC);	// modr/m
						ctx->target->encode(ctx, size, 32);
					} else {
						ctx->buffer.push_back(0x81);	// sub r/m32, imm32
						ctx->buffer.push_back(0xEC);	// modr/m
						ctx->target->encode(ctx, size, 32);
					}
				} break;

				case OP_STORE: {
					auto insn = (StoreInstruction*)insn_;
					
					const RegAlloc& pra = allocReg(ctx);
					const RegAlloc& vra = allocReg(ctx);

					ctx->greg = pra.reg;
					emitValue(ctx, insn->ptr);
					ctx->greg = vra.reg;
					emitValue(ctx, insn->val);

					if (sizeOf(ctx, insn->val->type) >= 64) {
						uint8_t rex = 0x48;
						if (vra.reg >= GREG_R8) rex |= 0x01;
						if (pra.reg >= GREG_R8) rex |= 0x04;

						ctx->buffer.push_back(rex);
					}

					ctx->buffer.push_back(0x89);		// mov r/m, r
					ctx->buffer.push_back(0x00 | ((vra.reg & 7) << 3) | (pra.reg & 7));
					
					freeReg(pra);
					freeReg(vra);
				} break;

				default:
					ctx->buffer.push_back(0xCC);		// int3
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

			case InstructionValueKind: {
				ValueInstruction* val = ((ValueInstruction*)val_);
				val->load(ctx);
			} break;

			default:
			invalid:
				ctx->buffer.push_back(0xCC);		// int3
				break;
		}
	}

	x86BlockEmitter::RegAlloc x86BlockEmitter::allocReg(x86EmissionContext* ctx) {
		x86BlockEmitter::RegAlloc ra;
		ra.spilled = false;
		ra.spill = 0;

		const static x86GeneralRegister regs[] = {
			GREG_AX, GREG_CX, GREG_DX, GREG_BX,
			GREG_SP, GREG_BP, GREG_SI, GREG_DI,
			GREG_R8, GREG_R9, GREG_R10, GREG_R11,
			GREG_R12, GREG_R13, GREG_R14, GREG_R15
		};

		for (uint32_t i = 0; i < _countof(regs); i++) {
			if (!allocatedRegs[regs[i]]) {
				ra.reg = regs[i];
				allocatedRegs[regs[i]] = true;
				return ra;
			}
		}

		ra.spilled = true;
		// TODO

		return ra;
	}

	void x86BlockEmitter::freeReg(const x86BlockEmitter::RegAlloc& ra) {
		if (!ra.spilled)
			allocatedRegs[ra.reg] = false;
		// TODO
	}
}
