#include "module.h"

#include <stdexcept>

namespace cc {
	Function* Module::addFunction(const char* name, FunctionType* ftype, DuplicateBehaivor dupBehaviour) {
		const char* name2 = scope.add(name, dupBehaviour == DuplicateBehaivor::Deduplicate);

		if (!name2) {
			switch (dupBehaviour) {
				case DuplicateBehaivor::ReturnExisting:
					return getGlobal<cc::Function>(name);
				case DuplicateBehaivor::ReturnNull:
					return nullptr;
				case DuplicateBehaivor::Throw:
					throw std::runtime_error(std::string("Function already exists: ") + name);
			}
		}

		Function* func = functions.emplace(name2, ftype, this);
		globals.push(func);
		return func;
	}
}
