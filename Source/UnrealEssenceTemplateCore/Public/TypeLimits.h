// Copyright UnrealEssence. All Rights Reserved.

#pragma once

#include "CoreTypes.h"

namespace EDateTime // для FDateTime аналог ETimespan у FTimespan для удобства и использования в constexpr функциях (стандартный вариант возвращает саму FDateTime и из-за ее конструктора использовать ее там не выйдет)
{
	/** The maximum number of ticks that can be represented in FTimespan. */
	inline constexpr int64 MaxTicks = 3652059 * 864000000000 - 1; // (MaxDays * TicksPerDay - 1) // (1 tick before midnight)

	/** The minimum number of ticks that can be represented in FTimespan. */
	inline constexpr int64 MinTicks = 0;
}

/* Type traits
*****************************************************************************/

	/**
	 * Helper class to map an all type to its limits (Extended TNumericLimits + Time Type Limits)
	 */
	template <typename InType>
	struct TTypeLimits;

	
	/**
	 * limits for const types
	 */
	template <typename InType>
	struct TTypeLimits<const InType> 
		: public TTypeLimits<InType>
	{ };


	/**
	 * limits for volatile types
	 */
	template <typename InType>
	struct TTypeLimits<volatile InType> 
		: public TTypeLimits<InType>
	{ };


	/**
	 * limits for const volatile types
	 */
	template <typename InType>
	struct TTypeLimits<const volatile InType> 
		: public TTypeLimits<InType>
	{ };


/* Time Types (Works return Ticks for Type)
*****************************************************************************/

	template<>
	struct TTypeLimits<FTimespan>
	{
		typedef int64 InTypeTicks;
		typedef FTimespan InType;
		
		/**
		 * Returns the minimum timespan value (Ticks).
		 *
		 * The minimum timespan value is -10675199 days, 02:48:05.4775808 times
		 * (-9223372036854775808 ticks).
		 * This equals approximately -29,234 years.
		 *
		 * @see Max
		 */
		[[nodiscard]] static constexpr InTypeTicks MinTicks()
		{
			return ETimespan::MinTicks;
		}

		/**
		 * Returns the maximum timespan value (Ticks).
		 *
		 * The maximum timespan value is 10675199 days, 02:48:05.4775807 times
		 * (9223372036854775807 ticks).
		 * This equals approximately 29,234 years.
		 *
		 * @see Min
		 */
		[[nodiscard]] static constexpr InTypeTicks MaxTicks()
		{
			return ETimespan::MaxTicks;
		}

		/**
		 * Returns the minimum timespan value (Ticks).
		 *
		 * The minimum timespan value is -10675199 days, 02:48:05.4775808 times
		 * (-9223372036854775808 ticks).
		 * This equals approximately -29,234 years.
		 *
		 * @see Max
		*/
		[[nodiscard]] static constexpr InTypeTicks LowestTicks()
		{
			return MinTicks();
		}
		
		/**
		 * Returns the minimum timespan value
		 *
		 * The minimum timespan value is -10675199 days, 02:48:05.4775808 times
		 * (-9223372036854775808 ticks).
		 * This equals approximately -29,234 years.
		 *
		 * @see Max
		*/
		[[nodiscard]] static UE_FORCEINLINE_HINT InType Min()
		{
			return FTimespan(ETimespan::MinTicks);
		}

		/**
		 * Returns the maximum timespan value
		 *
		 * The maximum timespan value is 10675199 days, 02:48:05.4775807 times
		 * (9223372036854775807 ticks).
		 * This equals approximately 29,234 years.
		 *
		 * @see Min
		 */
		[[nodiscard]] static UE_FORCEINLINE_HINT InType Max()
		{
			return FTimespan(ETimespan::MaxTicks);
		}

		/**
		 * Returns the minimum timespan value
		 *
		 * The minimum timespan value is -10675199 days, 02:48:05.4775808 times
		 * (-9223372036854775808 ticks).
		 * This equals approximately -29,234 years.
		 *
		 * @see Max
		*/
		[[nodiscard]] static UE_FORCEINLINE_HINT InType Lowest()
		{
			return Min();
		}
	};
	
	template<>
	struct TTypeLimits<FDateTime>
	{
		typedef int64 InTypeTicks;
		typedef FDateTime InType;
		
		/**
		 * Returns the minimum date value (Ticks).
		 *
		 * The minimum date value is January 1, 0001, 00:00:00.0.
		 *
		 * @see Max
		 */
		[[nodiscard]] static constexpr InTypeTicks MinTicks()
		{
			return EDateTime::MinTicks;
		}

		/**
		 * Returns the maximum date value (Ticks).
		 *
		 * The maximum date value is December 31, 9999, 23:59:59.9999999.
		 * This equals approximately 3652058,999999999988425925925 days (3652058 days + 23:59:59.9999999 times)
		 * (1 tick before midnight)
		 * 
		 * @see Min
		 */
		[[nodiscard]] static constexpr InTypeTicks MaxTicks()
		{
			// based FDateTime source (cut construct FDateTime and GetTicks() - used source Ticks values!)
			return EDateTime::MaxTicks;
		}

		/**
		 * Returns the minimum date value (Ticks).
		 *
		 * The minimum date value is January 1, 0001, 00:00:00.0.
		 *
		 * @see Max
		 */
		[[nodiscard]] static constexpr InTypeTicks LowestTicks()
		{
			return MinTicks();
		}
		
		/**
		 * Returns the minimum date value
		 *
		 * The minimum date value is January 1, 0001, 00:00:00.0.
		 *
		 * @see Max
		*/
		[[nodiscard]] static UE_FORCEINLINE_HINT InType Min()
		{
			return FDateTime(EDateTime::MinTicks);
		}

		/**
		 * Returns the maximum date value
		 *
		 * The maximum date value is December 31, 9999, 23:59:59.9999999.
		 * This equals approximately 3652058,999999999988425925925 days (3652058 days + 23:59:59.9999999 times)
		 * (1 tick before midnight)
		 * 
		 * @see Min
		 */
		[[nodiscard]] static UE_FORCEINLINE_HINT InType Max()
		{
			// based FDateTime source
			return FDateTime(EDateTime::MaxTicks);
		}

		/**
		 * Returns the minimum date value
		 *
		 * The minimum date value is January 1, 0001, 00:00:00.0.
		 *
		 * @see Max
		 */
		[[nodiscard]] static UE_FORCEINLINE_HINT InType Lowest()
		{
			return Min();
		}
	};

/* NumericType (use TNumericLimits)
*****************************************************************************/

template<typename InType>
requires (std::is_arithmetic_v<InType>)
struct TTypeLimits<InType>
{
	[[nodiscard]] static constexpr InType Min()
	{
		return TNumericLimits<InType>::Min();
	}

	[[nodiscard]] static constexpr InType Max()
	{
		return TNumericLimits<InType>::Max();
	}

	[[nodiscard]] static constexpr InType Lowest()
	{
		return TNumericLimits<InType>::Lowest();
	}
	
	// перегрузки для поддержки единообразной обработки временных и арифметических типов как int64 тики и значения (возвращая запрашиваемое значение для типа не в исходном типе, а в тиках ), но обычной обработки остальных типов
	
	[[nodiscard]] static constexpr InType MinTicks()
	{
		return Min();
	}
	
	[[nodiscard]] static constexpr InType MaxTicks()
	{
		return Max();
	}

	[[nodiscard]] static constexpr InType LowestTicks()
	{
		return Lowest();
	}
};

