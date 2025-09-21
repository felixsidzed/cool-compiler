#pragma once

#include "../Target.h"

namespace cc {
	class x86Target : public Target {
	public:
		x86Target(std::endian endian, bool x64 = (sizeof(void*) == 8)) : Target(endian, x64 ? x64Arch : x86Arch) {};

		std::pair<std::unique_ptr<char[]>, uint32_t> emitObject(Module* module) override;
	};
}
