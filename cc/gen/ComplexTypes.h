#pragma once

#include <cstdint>

#include "Values.h"

namespace cc {
	class PointerType : public Type {
	public:
		Type* pointee;

		PointerType(Type* pointee) : Type(PointerTypeID), pointee(pointee) {};

		string dump() override;
	};

	class IntegerType : public Type {
	public:
		uint8_t width;
		bool sign;

		IntegerType(uint8_t width, bool sign) : Type(IntegerTypeID), width(width), sign(sign) {};

		string dump() override;
	};
}
