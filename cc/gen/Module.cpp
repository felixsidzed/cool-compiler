#include "module.h"

#include <stdexcept>

namespace cc {
	Function* Module::addFunction(const char* name, FunctionType* ftype, DuplicateBehaivor dupBehaviour) {
		const char* name2 = scope.add(name, dupBehaviour == DuplicateBehaivor::Deduplicate);
		if (!name2) {
			switch (dupBehaviour) {
				case DuplicateBehaivor::ReturnExisting:
					return getGlobal<Function>(name);
				case DuplicateBehaivor::ReturnNull:
					return nullptr;
				case DuplicateBehaivor::Throw:
					throw std::runtime_error(std::string("Function already exists: ") + name);
			}
		}

		Function* func = functions.emplace(name2, ftype, this);
		globals.emplace(PublicLinkage, func, ftype, name2);
		return func;
	}

	Global* Module::addGlobal(const char* name, Value* val, Type* type, DuplicateBehaivor dupBehaviour) {
		const char* name2 = scope.add(name, dupBehaviour == DuplicateBehaivor::Deduplicate);
		if (!name2) {
			switch (dupBehaviour) {
				case DuplicateBehaivor::ReturnExisting: {
					for (auto& global : globals) {
						if (!strcmp(global.name, name))
							return &global;
					}
					return nullptr;
				}
				case DuplicateBehaivor::ReturnNull:
					return nullptr;
				case DuplicateBehaivor::Throw:
					throw std::runtime_error(std::string("Global already exists: ") + name);
			}
		}

		return globals.emplace(PublicLinkage, val, type, name2);
	}
}
