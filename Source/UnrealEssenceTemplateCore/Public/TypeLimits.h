// Copyright UnrealEssence. All Rights Reserved.

#pragma once

#include "CoreTypes.h"

/**
 * @file TypeLimits.h
 * @brief Расширенная версия TNumericLimits с поддержкой временных типов (FTimespan, FDateTime).
 *
 * @details Предоставляет единый интерфейс TTypeLimits<T> для получения Min/Max/Lowest значений:
 *          - Для арифметических типов — делегирует к стандартному TNumericLimits
 *          - Для FTimespan / FDateTime — возвращает корректные границы в тиках и в исходном типе
 *          - Для const/volatile — автоматически наследуется от базовой специализации
 *
 * @note Позволяет работать с числовыми и временными типами бесшовно через один интерфейс.
 * @version 1.0
 */

/**
 * @namespace EDateTime
 * @brief Аналог ETimespan для FDateTime — предоставляет константы тиков для constexpr-контекста.
 *
 * @details Стандартный FDateTime не имеет аналога ETimespan, и его конструктор нельзя использовать
 *          в constexpr. Данный namespace решает эту проблему, предоставляя сырые значения тиков.
 *          
 * @note Стандартный вариант возвращает саму FDateTime и из-за ее конструктора использовать ее в constexpr не выйдет что ограничивает проверки только рантаймом (мы обходим это ограничение!!!)
*/
namespace EDateTime
{
	/** @brief Максимальное количество тиков FDateTime (December 31, 9999, 23:59:59.9999999). */
	inline constexpr int64 MaxTicks = 3652059 * 864000000000 - 1; // (MaxDays * TicksPerDay - 1) // (1 tick before midnight)

	/** @brief Минимальное количество тиков FDateTime (January 1, 0001, 00:00:00.0). */
	inline constexpr int64 MinTicks = 0;
}

// ============================================================================
// Базовый шаблон TTypeLimits
// ============================================================================

	/**
	 * @struct TTypeLimits
	 * @brief Универсальный интерфейс получения границ значений для любого типа.
	 * @details Расширяет TNumericLimits поддержкой временных типов.
	 * @tparam InType Тип, для которого запрашиваются границы
	*/
	template <typename InType>
	struct TTypeLimits;

	
	/** @brief Специализация для const-типов — наследует базовую. */
	template <typename InType>
	struct TTypeLimits<const InType> 
		: public TTypeLimits<InType>
	{ };


	/** @brief Специализация для volatile-типов — наследует базовую. */
	template <typename InType>
	struct TTypeLimits<volatile InType> 
		: public TTypeLimits<InType>
	{ };


	/** @brief Специализация для const volatile-типов — наследует базовую. */
	template <typename InType>
	struct TTypeLimits<const volatile InType> 
		: public TTypeLimits<InType>
	{ };


// ============================================================================
// Специализация для FTimespan
// ============================================================================

	/**
	 * @brief Специализация TTypeLimits для FTimespan.
	 * @details Предоставляет границы как в тиках (int64), так и в виде FTimespan.
	 */
	template<>
	struct TTypeLimits<FTimespan>
	{
		typedef int64 InTypeTicks;
		typedef FTimespan InType;
		
		/**
		 * @brief Минимальное значение в тиках (~ -29,234 года).
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
		 * @brief Максимальное значение в тиках (~ +29,234 года).
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
		 * @brief Наименьшее значение в тиках (алиас MinTicks для FTimespan).
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
		 * @brief Минимальное значение в виде FTimespan.
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
		 * @brief Максимальное значение в виде FTimespan.
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
		 * @brief Наименьшее значение в виде FTimespan (алиас Min).
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
	
// ============================================================================
// Специализация для FDateTime
// ============================================================================

	/**
	 * @brief Специализация TTypeLimits для FDateTime.
	 * @details Предоставляет границы как в тиках (int64), так и в виде FDateTime.
	 */
	template<>
	struct TTypeLimits<FDateTime>
	{
		typedef int64 InTypeTicks;
		typedef FDateTime InType;
		
		/**
		 * @brief Минимальное значение в тиках (January 1, 0001, 00:00:00.0).
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
		 * @brief Максимальное значение в тиках (December 31, 9999, 23:59:59.9999999).
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
		 * @brief Наименьшее значение в тиках (алиас MinTicks).
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
		 * @brief Минимальное значение в виде FDateTime.
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
		 * @brief Максимальное значение в виде FDateTime.
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
		 * @brief Наименьшее значение в виде FDateTime (алиас Min).
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

// ============================================================================
// Специализация для арифметических типов (через TNumericLimits)
// ============================================================================

/**
 * @brief Специализация TTypeLimits для всех арифметических типов.
 * @details Делегирует к TNumericLimits. Дополнительно предоставляет *Ticks-методы
 *          для единообразного интерфейса с временными типами (возвращают то же, что и Min/Max/Lowest).
 */
template<typename InType>
requires (std::is_arithmetic_v<InType>)
struct TTypeLimits<InType>
{
	/** @brief Минимальное значение типа. */
	[[nodiscard]] static constexpr InType Min()
	{
		return TNumericLimits<InType>::Min();
	}

	/** @brief Максимальное значение типа. */
	[[nodiscard]] static constexpr InType Max()
	{
		return TNumericLimits<InType>::Max();
	}

	/** @brief Наименьшее значение типа (для знаковых = Min, для беззнаковых = Min). */
	[[nodiscard]] static constexpr InType Lowest()
	{
		return TNumericLimits<InType>::Lowest();
	}
	
	/** @brief Алиас Min() для единообразия с временными типами. */
	[[nodiscard]] static constexpr InType MinTicks()
	{
		return Min();
	}
	
	/** @brief Алиас Max() для единообразия с временными типами. */
	[[nodiscard]] static constexpr InType MaxTicks()
	{
		return Max();
	}

	/** @brief Алиас Lowest() для единообразия с временными типами. 
	 * 
	 */
	[[nodiscard]] static constexpr InType LowestTicks()
	{
		return Lowest();
	}
};

