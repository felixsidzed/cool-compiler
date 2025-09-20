#include "Constant.h"

#include "Context.h"

namespace cc {
	extern Context& getGlobalContext();
	
	ConstantInteger* ConstantInteger::get(uint64_t val, uint8_t width, bool sign) { return get(getGlobalContext(), val, width, sign); }

	ConstantInteger* ConstantInteger::get(Context& ctx, uint64_t val, uint8_t width, bool sign) {
		std::unique_ptr<ConstantInteger>* entry = nullptr;
		if (val == 0 || val == 1) {
			auto map = val == 0 ? &ctx.int0Const : &ctx.int1Const;
			auto it = map->find(width);
			if (it)
				return it->get();

			auto entry = std::make_unique<ConstantInteger>(Type::getInteger(ctx, width, sign), val);
			ConstantInteger* entryPtr = entry.get();
			map->insert(width, std::move(entry));
			return entryPtr;
		} else {
			auto it = ctx.intConst.find(val);
			if (it)
				return it->get();

			auto entry = std::make_unique<ConstantInteger>(Type::getInteger(ctx, width, sign), val);
			ConstantInteger* entryPtr = entry.get();
			ctx.intConst.insert(val, std::move(entry));
			return entryPtr;
		}

		return nullptr;
	}
}
