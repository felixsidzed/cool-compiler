#pragma once

#include <functional>

#include "Values.h"
#include "../vector.h"

namespace cc {
	class FunctionType : public Type {
	private:
		FunctionType(Type* returnType, Type** args, uint32_t nargs, bool vararg = false)
			: Type(FunctionTypeID), returnType(returnType), args(args, nargs), vararg(vararg) {};

	public:
		Type* returnType;
		vector<Type*> args;
		bool vararg;

		static FunctionType* get(Type* returnType, Type** args, uint32_t nargs, bool vararg = false);
		static FunctionType* get(Context& ctx, Type* returnType, Type** args, uint32_t nargs, bool vararg = false);

		string dump() override;
	};

	struct FunctionTypeHash {
		Type* returnType;
		Type** args;
		uint32_t nargs;
		bool vararg;
	};
}

namespace std {
	template<>
	struct hash<cc::FunctionTypeHash> {
		size_t operator()(const cc::FunctionTypeHash& key) const noexcept {
			size_t h = std::hash<cc::Type*>{}(key.returnType);
			
			for (uint32_t i = 0; i < key.nargs; i++)
				h ^= std::hash<cc::Type*>{}(key.args[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);

			h ^= std::hash<uint32_t>{}(key.nargs) + 0x9e3779b9 + (h << 6) + (h >> 2);
			h ^= std::hash<bool>{}(key.vararg) + 0x9e3779b9 + (h << 6) + (h >> 2);

			return h;
		}
	};
}
