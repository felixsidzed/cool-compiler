#pragma once

#include "../../map.h"
#include "x86Target.h"
#include "../../vector.h"
#include "../BlockEmitter.h"
#include "x86EmissionContext.h"

namespace cc {
	class x86BlockEmitter : public BlockEmitter {
	public:
		uint32_t allocaOffset;
		map<class AllocaInstruction*, uint32_t> allocaMap;

		void emit(EmissionContext* ctx_, Block* block) override;
		void emitValue(EmissionContext* ctx_, Value* val) override;
	protected:
		struct RegAlloc {
			x86GeneralRegister reg;

			bool spilled;
			uint8_t spill;
		};

		map<x86GeneralRegister, bool> allocatedRegs;

		RegAlloc allocReg(x86EmissionContext* ctx);
		void freeReg(const RegAlloc& ra);
	};
}
