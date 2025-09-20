#pragma once

#include <limits>
#include <type_traits>

#include "Values.h"

namespace cc {
	extern Context& getGlobalContext();

	class ConstantInteger : public Value {
	public:
		uint64_t val;

		ConstantInteger(Type* type, uint64_t val) : Value(type, nullptr), val(val) {};
		
		template<typename T = uint64_t>
		static ConstantInteger* get(T val, bool sign = true) { return get(getGlobalContext(), val, sign); }
		template<typename T = uint64_t>
		static ConstantInteger* get(Context& ctx, T val, bool sign = true) {
			static_assert(
				std::is_same_v<T, bool> || 
				std::is_same_v<T, uint8_t> || std::is_same_v<T, char> ||
				std::is_same_v<T, uint16_t> || std::is_same_v<T, short> ||
				std::is_same_v<T, uint32_t> || std::is_same_v<T, int> ||
				std::is_same_v<T, uint64_t> || std::is_same_v<T, long long>,
				"T must be a bool or (u)int8/16/32/64_t"
			);
			return get(ctx, val, bitwidth<T>(), sign);
		}

		static ConstantInteger* get(uint64_t val, uint8_t width, bool sign = true);
		static ConstantInteger* get(Context& ctx, uint64_t val, uint8_t width, bool sign = true);

		string dump() override;
	private:
		template<typename T>
		static inline constexpr int bitwidth() {
			if constexpr (std::is_same_v<T, bool>)
				return 1;
			else
				return std::numeric_limits<T>::digits + 1;
		}
	};
}
