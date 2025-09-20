#include "Values.h"

#include <memory>
#include <functional>
#include <unordered_map>

#include "Context.h"
#include "FunctionType.h"

namespace cc {
	static void _deleter(Type* ty) { delete ty; }
	static void _deleter(FunctionType* ty) { delete ty; }

	Type* Type::getVoid() { return getVoid(getGlobalContext()); };
	Type* Type::getBool() { return getBool(getGlobalContext()); };
	Type* Type::getFloat() { return getFloat(getGlobalContext()); };
	Type* Type::getDouble() { return getDouble(getGlobalContext()); };
	PointerType* Type::getPointer(Type* pointee) { return getPointer(getGlobalContext(), pointee); };
	Type* Type::getInteger(uint8_t bitwidth, bool sign) { return getInteger(getGlobalContext(), bitwidth, sign); };

	Type* Type::getVoid(Context& ctx) { return &ctx.voidTy; };
	Type* Type::getBool(Context& ctx) { return &ctx.boolTy; };
	Type* Type::getFloat(Context& ctx) { return &ctx.floatTy; };
	Type* Type::getDouble(Context& ctx) { return &ctx.doubleTy; };
	PointerType* Type::getPointer(Context& ctx, Type* pointee) {
		auto it = ctx.ptrTypeToType.find(pointee);
		if (it)
			return it->get();

		auto entry = std::unique_ptr<PointerType, void(*)(Type*)>(new PointerType(pointee), _deleter);
		PointerType* entryPtr = entry.get();
		ctx.ptrTypeToType.insert(pointee, std::move(entry));
		return entryPtr;
	};
	Type* Type::getInteger(Context& ctx, uint8_t bitwidth, bool sign) {
		switch (bitwidth) {
			case 1:
				return &ctx.boolTy;
			case 8:
				return sign ? &ctx.charTy : &ctx.ucharTy;
			case 16:
				return sign ? &ctx.shortTy : &ctx.ushortTy;
			case 32:
				return sign ? &ctx.intTy : &ctx.intTy;
			case 64:
				return sign ? &ctx.longTy : &ctx.ulongTy;
			default:
				return nullptr;
		}
	};

	FunctionType* FunctionType::get(Type* returnType, Type** args, uint32_t nargs, bool vararg) {
		return get(getGlobalContext(), returnType, args, nargs, vararg);
	}
	FunctionType* FunctionType::get(Context& ctx, Type* returnType, Type** args, uint32_t nargs, bool vararg) {
		std::hash<FunctionTypeHash> hasher;
		size_t hash = hasher({ returnType, args, nargs, vararg });
		for (const auto& ftype : ctx.ftypeToType) {
			if (hasher({ ftype.second->returnType, ftype.second->args.data, ftype.second->args.size, ftype.second->vararg }) == hash)
				return ftype.second.get();
		}
		
		auto entry = std::unique_ptr<FunctionType, void(*)(FunctionType*)>(new FunctionType(returnType, args, nargs, vararg), _deleter);
		FunctionType* entryPtr = entry.get();
		ctx.ftypeToType.insert(hash, std::move(entry));
		return entryPtr;
	}
}
