#pragma once

#include <limits>
#include <type_traits>

#include "Values.h"
#include "ComplexTypes.h"

namespace cc {
	extern Context& getGlobalContext();

	class ConstantInteger : public Value {
	public:
		uint64_t val;

		ConstantInteger(Type* type, uint64_t val) : Value(type, nullptr, ConstantValueKind), val(val) {};
		
		template<typename T = uint64_t>
		static ConstantInteger* get(T val) { return get(getGlobalContext(), val); }
		template<typename T = uint64_t>
		static ConstantInteger* get(Context& ctx, T val) {
			static_assert(std::is_integral<T>::value, "T must be integral");
			return get(ctx, val, bitwidth<T>(), std::numeric_limits<T>::is_signed);
		}

		static ConstantInteger* get(uint64_t val, uint8_t width, bool sign = true);
		static ConstantInteger* get(Context& ctx, uint64_t val, uint8_t width, bool sign = true);

		string dump() override;

		long long getSExtValue() {
			uint8_t width = ((IntegerType*)type)->width;
			return (long long)(val << (64 - width)) >> (64 - width);
		}
	private:
		template<typename T>
		static inline constexpr int bitwidth() {
			if constexpr (std::is_same_v<T, bool>)
				return 1;
			else
				return std::numeric_limits<T>::digits + (std::numeric_limits<T>::is_signed);
		}
	};

	class ConstantFP : public Value {
	public:
		double val;

		ConstantFP(Type* type, double val) : Value(type, nullptr, ConstantValueKind), val(val) {};

		static ConstantFP* get(double val, uint8_t width);
		static ConstantFP* get(Context& ctx, double val, uint8_t width);

		string dump() override;
	};
}
