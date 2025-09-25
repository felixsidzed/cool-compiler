#pragma once

#include "Values.h"

namespace cc {
	enum Linkage : uint8_t {
		PublicLinkage,
		InternalLinkage,
		PrivateLinkage
	};

	class Global : public Value {
	public:
		Linkage linkage;
		Value* initializer;

		Global(Linkage linkage, Value* initializer, Type* type, const char* name) : Value(type, name, GlobalValueKind), initializer(initializer), linkage(linkage) {};

		string dump() override;
	};
}
