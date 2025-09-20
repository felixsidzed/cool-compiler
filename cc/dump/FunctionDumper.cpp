#include "../gen/Function.h"

namespace cc {
	string Function::dump() {
		std::string result = "func ";
		result.append(name);
		result += "()\n"; // TODO: dump arguments

		for (auto& block : blocks) {
			result.append(block.name);
			result += ":\n";

			std::string blockResult = block.dump().data;
			size_t pos = 0;
			while (pos < blockResult.size()) {
				blockResult.insert(pos, "    ");
				pos = blockResult.find('\n', pos);
				if (pos == std::string::npos)
					break;
				pos += 3;
			}
			result += blockResult;
		}

		result += "\nend";
		return result.c_str();
	}
}
