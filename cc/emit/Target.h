#pragma once

#include <bit>
#include <span>
#include <memory>
#include <cstdint>

#include "EmissionContext.h"

#define CC_MEMORY_BUFFER_CHUNK_SIZE (sizeof(void*) * 2)

namespace cc {
	class Module;

	class Target {
	public:
		enum Arch {
			x86Arch,
			x64Arch,
			UnknownArch
		};

		std::endian endian : 1;
		Arch arch : 3;

		Target(std::endian endian, Arch arch) : endian(endian), arch(arch) {};

		template<typename T>
		void encode(EmissionContext* ctx, T value) const { return encode(ctx, value, sizeof(T) * 8); }
		void encode(EmissionContext* ctx, uint64_t value, uint8_t bitwidth) const {
			uint8_t bytes = bitwidth / 8;
			if (endian == std::endian::little)
				for (uint8_t i = 0; i < bytes; i++)
					ctx->buffer.push_back((uint8_t)((value >> (i * 8)) & 0xFF));
			else
				for (uint8_t i = 0; i < bytes; i++)
					ctx->buffer.push_back((uint8_t)((value >> ((bytes - 1 - i) * 8)) & 0xFF));
		}
		
		virtual std::pair<std::unique_ptr<char[]>, uint32_t> emitObject(Module* module) = 0;
	};
}
