#include "Constant.h"

#include "Context.h"

namespace cc {
	extern Context& getGlobalContext();
	
	ConstantInteger* ConstantInteger::get(uint64_t val, uint8_t width, bool sign) { return get(getGlobalContext(), val, width, sign); }
	ConstantInteger* ConstantInteger::get(Context& ctx, uint64_t val, uint8_t width, bool sign) {
		std::unique_ptr<ConstantInteger>* entry = nullptr;
		if (val == 0 || val == 1) {
			auto map = val == 0 ? &ctx.int0Const : &ctx.int1Const;
			auto& entry = map->operator[](width);
			if (!entry)
				entry.reset(new ConstantInteger(Type::getInteger(ctx, width, sign), val));
			return entry.get();
		} else {
			auto& entry = ctx.intConst[val];
			if (!entry)
				entry.reset(new ConstantInteger(Type::getInteger(ctx, width, sign), val));
			return entry.get();
		}

		return nullptr;
	}

	ConstantFP* ConstantFP::get(double val, uint8_t width) { return get(getGlobalContext(), val, width); }
	ConstantFP* ConstantFP::get(Context& ctx, double val, uint8_t width) {
		auto& entry = ctx.fpConst[val];
		if (!entry) {
			if (width == 32)
				entry.reset(new ConstantFP(Type::getFloat(), val));
			else if (width == 64)
				entry.reset(new ConstantFP(Type::getDouble(), val));
			else
				return nullptr;
		}
		return entry.get();
	}
}
