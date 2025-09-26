#pragma once

#include <vector>

namespace cc {
	class Block;
	class Target;
	class BlockEmitter;

	struct EmissionContext {
		Target* target;
		uint8_t sectionIdx = 0;
		BlockEmitter* blockEmitter;
		std::vector<uint8_t>& buffer;

		EmissionContext(std::vector<uint8_t>& buffer, Target* target) : buffer(buffer), target(target), blockEmitter(nullptr) {};
	};
}
