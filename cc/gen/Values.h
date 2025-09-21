#pragma once

#include <cstdint>

#include "../string.h"

namespace cc {
	enum TypeID : uint8_t {
		// primitive types (sorted by size)
		VoidTypeID,
		BooleanTypeID,
		FloatTypeID,
		DoubleTypeID,

		// complex types
		IntegerTypeID,
		PointerTypeID,
		FunctionTypeID
	};

	enum ValueKind : uint8_t {
		ConstantValueKind,
		GlobalValueKind
	};

	class Context;
	class PointerType;

	class Type {
	public:
		TypeID typeId;

		Type(TypeID typeId) : typeId(typeId) {};

		static Type* getVoid();
		static Type* getBool();
		static Type* getFloat();
		static Type* getDouble();
		static PointerType* getPointer(Type* pointee);
		static Type* getInteger(uint8_t bidwith, bool sign = true);

		static Type* getVoid(Context& ctx);
		static Type* getBool(Context& ctx);
		static Type* getFloat(Context& ctx);
		static Type* getDouble(Context& ctx);
		static PointerType* getPointer(Context& ctx, Type* pointee);
		static Type* getInteger(Context& ctx, uint8_t bidwith, bool sign = true);

		virtual string dump();
	};

	class Value {
	public:
		Type* type;
		const char* name;
		ValueKind vk;

		Value(Type* type, const char* name, ValueKind vk) : type(type), name(name), vk(vk) {};

		virtual string dump() { return name; };
	};
}
