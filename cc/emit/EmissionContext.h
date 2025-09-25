#pragma once

#include <vector>

namespace cc {
	class Block;
	class Target;

	struct EmissionContext {
		Target* target;
		uint8_t sectionIdx;
		std::vector<uint8_t>& buffer;

		EmissionContext(std::vector<uint8_t>& buffer, Target* target) : buffer(buffer), target(target) {};
	};
}
