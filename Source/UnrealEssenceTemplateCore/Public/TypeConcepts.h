// Copyright UnrealEssence. All Rights Reserved.

#pragma once

#include <type_traits>
#include <concepts>

/**
 * @file TypeConcepts.h
 * @brief Набор C++20 концептов и универсальная проверка дефолтности значения.
 *
 * @details Предоставляет:
 *          - Концепт IsArithmeticAndTime для единой работы с числовыми и временными типами
 *          - Концепты для проверки наличия методов (IsValid, IsEmpty, ToString и т.д.)
 *          - Универсальную функцию IsDefaultValue() для определения дефолтности ЛЮБОГО типа
 *
 * @warning Требует C++20. Для более ранних версий UE (UE4 / ранние UE5) используйте TypeTraits.h.
 * @version 1.0
 */

/**
 * @namespace concepts
 * @brief Пространство имён C++20-концептов для проверки типов.
 */
namespace concepts
{
	// ========================================================================
	// Комплексные концепты
	// ========================================================================

	/**
	 * @concept IsArithmeticAndTime
	 * @brief Проверяет, является ли тип арифметическим (int, float...) или временным (FTimespan, FDateTime).
	 * @details Используется для единообразной обработки числовых и временных типов в шаблонных функциях через адаптер.
	 */
	template<typename T>
	concept IsArithmeticAndTime = std::is_arithmetic_v<T> || std::is_same_v<FTimespan, T> || std::is_same_v<FDateTime, T>;
	
	// ========================================================================
	// Концепты проверки наличия методов
	// ========================================================================

	/** @concept HasIsValid — тип имеет метод IsValid() const. */
	template<typename T>
	concept HasIsValid = requires(const T& t) { t.IsValid(); };
    
	/** @concept HasIsEmpty — тип имеет метод IsEmpty() const. */
	template<typename T>
	concept HasIsEmpty = requires(const T& t) { t.IsEmpty(); };
    
	/** @concept HasIsDefault — тип имеет метод IsDefault() const. */
	template<typename T>
	concept HasIsDefault = requires(const T& t) { t.IsDefault(); };
    
	/** @concept HasNum — тип имеет метод Num() const (контейнеры UE: TArray, FString...). */
	template<typename T>
	concept HasNum = requires(const T& t) { t.Num(); };
    
	/** @concept HasEqualsOperator — тип поддерживает оператор ==. */
	template<typename T>
	concept HasEqualsOperator = requires(const T& a, const T& b) { a == b; };
    
	/** @concept HasToString — тип имеет метод ToString(), возвращаемое значение конвертируется в FString. */
	template<typename T>
	concept HasToString = requires(const T& t) { { t.ToString() } -> std::convertible_to<FString>; };
	
	// ========================================================================
	// Проверка на итерируемый тип
	// ========================================================================
	
	template<typename T>
	concept IsIterable = requires(T t) {
		{ t.begin() } -> std::input_or_output_iterator;
		{ t.end() } -> std::input_or_output_iterator;
	};
	
	// ========================================================================
	// Универсальная проверка дефолтности значения
	// ========================================================================

	/**
	 * @brief Определяет, является ли значение дефолтным для своего типа.
	 *
	 * @details Проверяет дефолтность по цепочке приоритетов:
	 *          1. Указатели → сравнение с nullptr
	 *          2. Арифметика и enum → сравнение с 0
	 *          3. Метод IsDefault() если есть
	 *          4. Метод IsEmpty() если есть
	 *          5. Метод IsValid() — если !IsValid(), считается дефолтным;
	 *             дополнительно проверяется разыменовываемость (*Value)
	 *          6. Метод Num() == 0 (контейнеры UE)
	 *          7. Метод empty() (STL-контейнеры)
	 *          8. Оператор == с дефолтно-сконструированным объектом
	 *          9. Иначе → false (тип не поддерживается)
	 *
	 * @tparam T   Тип значения
	 * @param  Value  Проверяемое значение
	 * @return True  если значение считается дефолтным
	 *
	 * @note Поддерживает кастомные типы — достаточно реализовать один из методов
	 *       IsDefault / IsEmpty / IsValid / Num / empty / operator==.
	 */
	template<typename T>
	bool IsDefaultValue(const T& Value)
	{
		using RawType = std::remove_cv_t<std::remove_reference_t<T>>;
        
		if constexpr (std::is_pointer_v<RawType>)
		{
			return Value == nullptr;
		}
		else if constexpr (std::is_arithmetic_v<RawType> || std::is_enum_v<RawType>)
		{
			return Value == RawType{0};
		}
		else if constexpr (HasIsDefault<RawType>)
		{
			return Value.IsDefault();
		}
		else if constexpr (HasIsEmpty<RawType>)
		{
			return Value.IsEmpty();
		}
		else if constexpr (HasIsValid<RawType>)
		{
			if (!Value.IsValid()) return true;
			if constexpr (requires { *Value; }) return IsDefaultValue(*Value);
			return false;
		}
		else if constexpr (HasNum<RawType>)
		{
			return Value.Num() == 0;
		}
		else if constexpr (requires { Value.empty(); })
		{
			return Value.empty();
		}
		else if constexpr (HasEqualsOperator<RawType> && std::is_default_constructible_v<RawType>)
		{
			return Value == RawType{};
		}
		else
		{
			return false;
		}
	}
}