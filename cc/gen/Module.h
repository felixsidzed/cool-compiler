#pragma once

#include "Scope.h"
#include "Function.h"

namespace cc {
	enum class DuplicateBehaivor : uint8_t {
		ReturnExisting,
		ReturnNull,
		Throw,
		Deduplicate
	};

	class Module {
	public:
		string name;

		Scope scope;
		vector<Value*> globals;
		
		vector<Function> functions;

		Module(const char* name) : name(name) {};

		template<typename T>
		T* getGlobal(const char* name) {
			static_assert(std::is_base_of<Value, T>::value, "T must inherit from Value");

			for (Value* global : globals) {
				if (!strcmp(global->name, name))
					return (T*)global;
			}
			
			return nullptr;
		}

		Function* addFunction(const char* name, FunctionType* ftype, DuplicateBehaivor dupBehaviour = DuplicateBehaivor::Throw);

		string dump();
	};
}
