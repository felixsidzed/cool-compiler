#pragma once

#include <cstdint>

#include "../EmissionContext.h"

struct _IMAGE_SYMBOL;
typedef _IMAGE_SYMBOL IMAGE_SYMBOL;

struct _IMAGE_BASE_RELOCATION;
typedef _IMAGE_BASE_RELOCATION IMAGE_BASE_RELOCATION;

namespace cc {
	enum x86Register : uint8_t {
		REG_AL, REG_CL, REG_DL, REG_BL,
		REG_AH, REG_CH, REG_DH, REG_BH,
		REG_R8B, REG_R9B, REG_R10B, REG_R11B,
		REG_R12B, REG_R13B, REG_R14B, REG_R15B,

		REG_AX, REG_CX, REG_DX, REG_BX,
		REG_SP, REG_BP, REG_SI, REG_DI,
		REG_R8W, REG_R9W, REG_R10W, REG_R11W,
		REG_R12W, REG_R13W, REG_R14W, REG_R15W,

		REG_EAX, REG_ECX, REG_EDX, REG_EBX,
		REG_ESP, REG_EBP, REG_ESI, REG_EDI,
		REG_R8D, REG_R9D, REG_R10D, REG_R11D,
		REG_R12D, REG_R13D, REG_R14D, REG_R15D,

		REG_RAX, REG_RCX, REG_RDX, REG_RBX,
		REG_RSP, REG_RBP, REG_RSI, REG_RDI,
		REG_R8, REG_R9, REG_R10, REG_R11,
		REG_R12, REG_R13, REG_R14, REG_R15,

		REG_ES, REG_CS, REG_SS, REG_DS, REG_FS, REG_GS,

		REG_CR0, REG_CR2, REG_CR3, REG_CR4, REG_CR8,

		REG_DR0, REG_DR1, REG_DR2, REG_DR3, REG_DR6, REG_DR7
	};

	enum x86GeneralRegister : uint8_t {
		GREG_AX, GREG_CX, GREG_DX, GREG_BX,
		GREG_SP, GREG_BP, GREG_SI, GREG_DI,
		GREG_R8, GREG_R9, GREG_R10, GREG_R11,
		GREG_R12, GREG_R13, GREG_R14, GREG_R15
	};

	struct x86EmissionContext : public EmissionContext {
		x86Register reg = REG_AL;
		x86GeneralRegister greg = GREG_AX;

		std::vector<IMAGE_SYMBOL>& symbols;
		std::vector<IMAGE_BASE_RELOCATION>& relocs;

		x86EmissionContext(std::vector<uint8_t>& buffer, Target* target, std::vector<IMAGE_SYMBOL>& symbols, std::vector<IMAGE_BASE_RELOCATION>& relocs)
			: EmissionContext(buffer, target), symbols(symbols), relocs(relocs) {}
	};
}
