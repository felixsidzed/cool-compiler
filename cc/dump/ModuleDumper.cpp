#include "../gen/Module.h"

namespace cc {
	string Module::dump() {
		std::string result = "// ";
		result.append(name);
		result.append(2, '\n');

		for (auto& global : globals) {
			if (global.type->typeId == FunctionTypeID)
				continue;

			result.append(global.dump());
			result.append(2, '\n');
		}

		for (auto& func : functions)
			result.append(func.dump());

		return result.c_str();
	}
}
