#pragma once

#include "Scope.h"
#include "Global.h"
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
		vector<Global> globals;
		
		vector<Function> functions;

		Module(const char* name) : name(name) {};

		template<typename T>
		T* getGlobal(const char* name) {
			static_assert(std::is_base_of<Value, T>::value, "T must inherit from Value");

			for (auto& global : globals) {
				if (!strcmp(global.name, name))
					return (T*)global.initializer;
			}
			
			return nullptr;
		}
		
		Global* addGlobal(const char* name, Value* val, Type* type, DuplicateBehaivor dupBehavior = DuplicateBehaivor::Deduplicate);

		Global* addGlobal(Value* val, DuplicateBehaivor dupBehaivor = DuplicateBehaivor::Deduplicate) { return addGlobal(val->name, val, val->type, dupBehaivor); };
		Global* addGlobal(Value* val, Type* type, DuplicateBehaivor dupBehaivor = DuplicateBehaivor::Deduplicate) { return addGlobal(val->name, val, type, dupBehaivor); };

		Function* addFunction(const char* name, FunctionType* ftype, DuplicateBehaivor dupBehaivor = DuplicateBehaivor::Throw);

		string dump();
	};
}
