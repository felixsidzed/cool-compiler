#pragma once

#include <memory>

#include "../map.h"
#include "Values.h"
#include "Constant.h"
#include "ComplexTypes.h"

namespace cc {
	class FunctionType;

	class Context {
	public:
		Context()
			: boolTy(1, false),
			charTy(8, true), shortTy(16, true), intTy(32, true), longTy(64, true),
			ucharTy(8, false), ushortTy(16, false), uintTy(32, false), ulongTy(64, false),
			floatTy(FloatTypeID), doubleTy(DoubleTypeID),
			voidTy(VoidTypeID)
		{}

	private:
		friend class Type;
		friend class ConstantFP;
		friend class FunctionType;
		friend class ConstantInteger;

		Type voidTy;
		Type floatTy, doubleTy;

		IntegerType boolTy;
		IntegerType charTy, shortTy, intTy, longTy;
		IntegerType ucharTy, ushortTy, uintTy, ulongTy;

		map<Type*, std::unique_ptr<PointerType, void(*)(Type*)>> ptrTypeToType;
		map<size_t, std::unique_ptr<FunctionType, void(*)(FunctionType*)>> ftypeToType;

		map<uint8_t, std::unique_ptr<ConstantInteger>> int0Const;
		map<uint8_t, std::unique_ptr<ConstantInteger>> int1Const;
		map<uint64_t, std::unique_ptr<ConstantInteger>> intConst;

		map<double, std::unique_ptr<ConstantFP>> fpConst;
	};

	Context& getGlobalContext();
}
