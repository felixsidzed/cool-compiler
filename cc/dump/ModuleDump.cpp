#include "../gen/Module.h"

namespace cc {
	string Module::dump() {
		std::string result = "// ";
		result.append(name);
		result += "\n\n";

		for (auto& func : functions)
			result.append(func.dump());

		return result.c_str();
	}
}
