// Copyright UnrealEssence. All Rights Reserved.

#pragma once
#include <type_traits>
#include <string>

/**
 * @file TypeTraits.h
 * @brief Набор трейтов и алиасов для проверки операторов, методов типов,
 *        получения безопасных типов для арифметических операций и определения
 *        минимального типа, вмещающего значение без переполнения.
 *
 * @details Содержит:
 *          - Трейты наличия операторов (+, -, *, /, %, ==, <, &, |, ++, [] и т.д.)
 *          - Трейты наличия методов UE-стиля (ToString, Equals, GetTicks, Num, IsValid...)
 *          - Комбинированные алиасы (is_comparable_v, is_ordered_v, has_size_v...)
 *          - SafeUpType / SafeUpUnType — повышение типа для безопасного хранения диапазона
 *          - GreatestSafeType / GreatestSafeUnType — наибольший безопасный тип для двух типов
 *          - TargetSafeType / TargetSafeUnType — безопасный тип с ограничением роста до Target
 *          - Макросы DEFINE_HAS_METHOD / DEFINE_HAS_METHOD_WITH_ARGS для быстрой генерации трейтов
 *
 * @note Поддерживает больше версий UE, чем TypeConcepts.h (не требует C++20).
 * @version 1.0
 */


/**
 * @namespace traits
 * @brief Пространство имён трейтов для проверки типов и выбора безопасных типов.
*/
namespace traits
{
    
    // ========================================================================
    // Вспомогательная структура void_t (для SFINAE)
    // ========================================================================

    /** @brief Аналог std::void_t для использования в SFINAE-проверках. */
    template<typename...>
    using void_t = void;

    /// ========================================================================
    // Арифметические операторы
    // ========================================================================

    /** @brief Проверяет наличие operator+(T, U). */
    template<typename T, typename U = T, typename = void>
    struct has_plus_operator : std::false_type {};
    template<typename T, typename U>
    struct has_plus_operator<T, U, void_t<decltype(std::declval<T>() + std::declval<U>())>> : std::true_type {};
    template<typename T, typename U = T>
    inline constexpr bool has_plus_operator_v = has_plus_operator<T, U>::value;
    
    /** @brief Проверяет наличие operator-(T, U). */
    template<typename T, typename U = T, typename = void>
    struct has_minus_operator : std::false_type {};
    template<typename T, typename U>
    struct has_minus_operator<T, U, void_t<decltype(std::declval<T>() - std::declval<U>())>> : std::true_type {};
    template<typename T, typename U = T>
    inline constexpr bool has_minus_operator_v = has_minus_operator<T, U>::value;
    
    /** @brief Проверяет наличие operator*(T, U). */
    template<typename T, typename U = T, typename = void>
    struct has_multiply_operator : std::false_type {};
    template<typename T, typename U>
    struct has_multiply_operator<T, U, void_t<decltype(std::declval<T>() * std::declval<U>())>> : std::true_type {};
    template<typename T, typename U = T>
    inline constexpr bool has_multiply_operator_v = has_multiply_operator<T, U>::value;
    
    /** @brief Проверяет наличие operator/(T, U). */
    template<typename T, typename U = T, typename = void>
    struct has_divide_operator : std::false_type {};
    template<typename T, typename U>
    struct has_divide_operator<T, U, void_t<decltype(std::declval<T>() / std::declval<U>())>> : std::true_type {};
    template<typename T, typename U = T>
    inline constexpr bool has_divide_operator_v = has_divide_operator<T, U>::value;
    
    /** @brief Проверяет наличие operator%(T, U). 
     * 
     */
    template<typename T, typename U = T, typename = void>
    struct has_modulus_operator : std::false_type {};
    template<typename T, typename U>
    struct has_modulus_operator<T, U, void_t<decltype(std::declval<T>() % std::declval<U>())>> : std::true_type {};
    template<typename T, typename U = T>
    inline constexpr bool has_modulus_operator_v = has_modulus_operator<T, U>::value;

    
    // ========================================================================
    // Операторы присваивания
    // ========================================================================
    
    /** @brief Проверяет наличие operator=(T, U). 
     * 
     */
    template<typename T, typename U = T, typename = void>
    struct has_assign_operator : std::false_type {};
    template<typename T, typename U>
    struct has_assign_operator<T, U, void_t<decltype(std::declval<T>() = std::declval<U>())>> : std::true_type {};
    template<typename T, typename U = T>
    inline constexpr bool has_assign_operator_v = has_assign_operator<T, U>::value;
    
    
    
    // ========================================================================
    // Арифметические операторы с присваиванием (+=, -=, *=, /=, %=)
    // ========================================================================

    /** @brief Проверяет наличие operator+=(T, U). */
    template<typename T, typename U = T, typename = void>
    struct has_plus_assign_operator : std::false_type {};
    template<typename T, typename U>
    struct has_plus_assign_operator<T, U, void_t<decltype(std::declval<T>() += std::declval<U>())>> : std::true_type {};
    template<typename T, typename U = T>
    inline constexpr bool has_plus_assign_operator_v = has_plus_assign_operator<T, U>::value;
    
    /** @brief Проверяет наличие operator-=(T, U). 
     * 
     */
    template<typename T, typename U = T, typename = void>
    struct has_minus_assign_operator : std::false_type {};
    template<typename T, typename U>
    struct has_minus_assign_operator<T, U, void_t<decltype(std::declval<T>() -= std::declval<U>())>> : std::true_type {};
    template<typename T, typename U = T>
    inline constexpr bool has_minus_assign_operator_v = has_minus_assign_operator<T, U>::value;
    
    /** @brief Проверяет наличие operator*=(T, U). */
    template<typename T, typename U = T, typename = void>
    struct has_multiply_assign_operator : std::false_type {};
    template<typename T, typename U>
    struct has_multiply_assign_operator<T, U, void_t<decltype(std::declval<T>() *= std::declval<U>())>> : std::true_type {};
    template<typename T, typename U = T>
    inline constexpr bool has_multiply_assign_operator_v = has_multiply_assign_operator<T, U>::value;
   
    /** @brief Проверяет наличие operator/=(T, U). */
    template<typename T, typename U = T, typename = void>
    struct has_divide_assign_operator : std::false_type {};
    template<typename T, typename U>
    struct has_divide_assign_operator<T, U, void_t<decltype(std::declval<T>() /= std::declval<U>())>> : std::true_type {};
    template<typename T, typename U = T>
    inline constexpr bool has_divide_assign_operator_v = has_divide_assign_operator<T, U>::value;

    /** @brief Проверяет наличие operator%=(T, U). */
    template<typename T, typename U = T, typename = void>
    struct has_modulus_assign_operator : std::false_type {};
    template<typename T, typename U>
    struct has_modulus_assign_operator<T, U, void_t<decltype(std::declval<T>() %= std::declval<U>())>> : std::true_type {};
    template<typename T, typename U = T>
    inline constexpr bool has_modulus_assign_operator_v = has_modulus_assign_operator<T, U>::value;
   
    
    
    
    // ========================================================================
	// Операторы инкремента/декремента
	// ========================================================================

	/** @brief Проверяет наличие префиксного operator++(T). */
	template<typename T, typename = void>
	struct has_pre_increment_operator : std::false_type {};
	template<typename T>
	struct has_pre_increment_operator<T, void_t<decltype(++std::declval<T>())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_pre_increment_operator_v = has_pre_increment_operator<T>::value;

	/** @brief Проверяет наличие постфиксного operator++(T). */
	template<typename T, typename = void>
	struct has_post_increment_operator : std::false_type {};
	template<typename T>
	struct has_post_increment_operator<T, void_t<decltype(std::declval<T>()++)>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_post_increment_operator_v = has_post_increment_operator<T>::value;

	/** @brief Проверяет наличие префиксного operator--(T). */
	template<typename T, typename = void>
	struct has_pre_decrement_operator : std::false_type {};
	template<typename T>
	struct has_pre_decrement_operator<T, void_t<decltype(--std::declval<T>())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_pre_decrement_operator_v = has_pre_decrement_operator<T>::value;

	/** @brief Проверяет наличие постфиксного operator--(T). */
	template<typename T, typename = void>
	struct has_post_decrement_operator : std::false_type {};
	template<typename T>
	struct has_post_decrement_operator<T, void_t<decltype(std::declval<T>()--)>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_post_decrement_operator_v = has_post_decrement_operator<T>::value;


	// ========================================================================
	// Операторы сравнения
	// ========================================================================

	/** @brief Проверяет наличие operator==(T, U). */
	template<typename T, typename U = T, typename = void>
	struct has_equals_operator : std::false_type {};
	template<typename T, typename U>
	struct has_equals_operator<T, U, void_t<decltype(std::declval<T>() == std::declval<U>())>> : std::true_type {};
	template<typename T, typename U = T>
	inline constexpr bool has_equals_operator_v = has_equals_operator<T, U>::value;

	/** @brief Проверяет наличие operator!=(T, U). */
	template<typename T, typename U = T, typename = void>
	struct has_not_equals_operator : std::false_type {};
	template<typename T, typename U>
	struct has_not_equals_operator<T, U, void_t<decltype(std::declval<T>() != std::declval<U>())>> : std::true_type {};
	template<typename T, typename U = T>
	inline constexpr bool has_not_equals_operator_v = has_not_equals_operator<T, U>::value;

	/** @brief Проверяет наличие operator<(T, U). */
	template<typename T, typename U = T, typename = void>
	struct has_less_operator : std::false_type {};
	template<typename T, typename U>
	struct has_less_operator<T, U, void_t<decltype(std::declval<T>() < std::declval<U>())>> : std::true_type {};
	template<typename T, typename U = T>
	inline constexpr bool has_less_operator_v = has_less_operator<T, U>::value;

	/** @brief Проверяет наличие operator<=(T, U). */
	template<typename T, typename U = T, typename = void>
	struct has_less_equal_operator : std::false_type {};
	template<typename T, typename U>
	struct has_less_equal_operator<T, U, void_t<decltype(std::declval<T>() <= std::declval<U>())>> : std::true_type {};
	template<typename T, typename U = T>
	inline constexpr bool has_less_equal_operator_v = has_less_equal_operator<T, U>::value;

	/** @brief Проверяет наличие operator>(T, U). */
	template<typename T, typename U = T, typename = void>
	struct has_greater_operator : std::false_type {};
	template<typename T, typename U>
	struct has_greater_operator<T, U, void_t<decltype(std::declval<T>() > std::declval<U>())>> : std::true_type {};
	template<typename T, typename U = T>
	inline constexpr bool has_greater_operator_v = has_greater_operator<T, U>::value;

	/** @brief Проверяет наличие operator>=(T, U). */
	template<typename T, typename U = T, typename = void>
	struct has_greater_equal_operator : std::false_type {};
	template<typename T, typename U>
	struct has_greater_equal_operator<T, U, void_t<decltype(std::declval<T>() >= std::declval<U>())>> : std::true_type {};
	template<typename T, typename U = T>
	inline constexpr bool has_greater_equal_operator_v = has_greater_equal_operator<T, U>::value;

    

    
    // ========================================================================
	// Логические операторы (&&, ||, !)
	// ========================================================================

	/** @brief Проверяет наличие operator&&(T, U). */
	template<typename T, typename U = T, typename = void>
	struct has_logical_and_operator : std::false_type {};
	template<typename T, typename U>
	struct has_logical_and_operator<T, U, void_t<decltype(std::declval<T>() && std::declval<U>())>> : std::true_type {};
	template<typename T, typename U = T>
	inline constexpr bool has_logical_and_operator_v = has_logical_and_operator<T, U>::value;

	/** @brief Проверяет наличие operator||(T, U). */
	template<typename T, typename U = T, typename = void>
	struct has_logical_or_operator : std::false_type {};
	template<typename T, typename U>
	struct has_logical_or_operator<T, U, void_t<decltype(std::declval<T>() || std::declval<U>())>> : std::true_type {};
	template<typename T, typename U = T>
	inline constexpr bool has_logical_or_operator_v = has_logical_or_operator<T, U>::value;

	/** @brief Проверяет наличие operator!(T). */
	template<typename T, typename = void>
	struct has_logical_not_operator : std::false_type {};
	template<typename T>
	struct has_logical_not_operator<T, void_t<decltype(!std::declval<T>())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_logical_not_operator_v = has_logical_not_operator<T>::value;


	// ========================================================================
	// Бинарные (битовые) операторы
	// ========================================================================

	/** @brief Проверяет наличие operator&(T, U). */
	template<typename T, typename U = T, typename = void>
	struct has_bitwise_and_operator : std::false_type {};
	template<typename T, typename U>
	struct has_bitwise_and_operator<T, U, void_t<decltype(std::declval<T>() & std::declval<U>())>> : std::true_type {};
	template<typename T, typename U = T>
	inline constexpr bool has_bitwise_and_operator_v = has_bitwise_and_operator<T, U>::value;

	/** @brief Проверяет наличие operator|(T, U). */
	template<typename T, typename U = T, typename = void>
	struct has_bitwise_or_operator : std::false_type {};
	template<typename T, typename U>
	struct has_bitwise_or_operator<T, U, void_t<decltype(std::declval<T>() | std::declval<U>())>> : std::true_type {};
	template<typename T, typename U = T>
	inline constexpr bool has_bitwise_or_operator_v = has_bitwise_or_operator<T, U>::value;

	/** @brief Проверяет наличие operator^(T, U). */
	template<typename T, typename U = T, typename = void>
	struct has_bitwise_xor_operator : std::false_type {};
	template<typename T, typename U>
	struct has_bitwise_xor_operator<T, U, void_t<decltype(std::declval<T>() ^ std::declval<U>())>> : std::true_type {};
	template<typename T, typename U = T>
	inline constexpr bool has_bitwise_xor_operator_v = has_bitwise_xor_operator<T, U>::value;

	/** @brief Проверяет наличие operator~(T). */
	template<typename T, typename = void>
	struct has_bitwise_not_operator : std::false_type {};
	template<typename T>
	struct has_bitwise_not_operator<T, void_t<decltype(~std::declval<T>())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_bitwise_not_operator_v = has_bitwise_not_operator<T>::value;
    

    

	
	// ========================================================================
	// Операторы доступа
	// ========================================================================

	/** @brief Проверяет наличие operator[](T, Index). */
	template<typename T, typename Index = size_t, typename = void>
	struct has_subscript_operator : std::false_type {};
	template<typename T, typename Index>
	struct has_subscript_operator<T, Index, void_t<decltype(std::declval<T>()[std::declval<Index>()])>> : std::true_type {};
	template<typename T, typename Index = size_t>
	inline constexpr bool has_subscript_operator_v = has_subscript_operator<T, Index>::value;

	/** @brief Проверяет наличие operator*(T) — разыменование. */
	template<typename T, typename = void>
	struct has_dereference_operator : std::false_type {};
	template<typename T>
	struct has_dereference_operator<T, void_t<decltype(*std::declval<T>())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_dereference_operator_v = has_dereference_operator<T>::value;

	/** @brief Проверяет наличие operator->(T) — доступ к члену через указатель. */
	template<typename T, typename = void>
	struct has_member_access_operator : std::false_type {};
	template<typename T>
	struct has_member_access_operator<T, void_t<decltype(std::declval<T>().operator->())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_member_access_operator_v = has_member_access_operator<T>::value;
    
    
    

    
    // ========================================================================
	// Методы-аналоги операторов (UE-стиль)
	// ========================================================================

	/** @brief Проверяет наличие метода Equals(U). */
	template<typename T, typename U = T, typename = void>
	struct has_equals_method : std::false_type {};
	template<typename T, typename U>
	struct has_equals_method<T, U, void_t<decltype(std::declval<T>().Equals(std::declval<U>()))>> : std::true_type {};
	template<typename T, typename U = T>
	inline constexpr bool has_equals_method_v = has_equals_method<T, U>::value;

	/** @brief Проверяет наличие метода Compare(U). */
	template<typename T, typename U = T, typename = void>
	struct has_compare_method : std::false_type {};
	template<typename T, typename U>
	struct has_compare_method<T, U, void_t<decltype(std::declval<T>().Compare(std::declval<U>()))>> : std::true_type {};
	template<typename T, typename U = T>
	inline constexpr bool has_compare_method_v = has_compare_method<T, U>::value;

	/** @brief Проверяет наличие метода Add(U). */
	template<typename T, typename U = T, typename = void>
	struct has_add_method : std::false_type {};
	template<typename T, typename U>
	struct has_add_method<T, U, void_t<decltype(std::declval<T>().Add(std::declval<U>()))>> : std::true_type {};
	template<typename T, typename U = T>
	inline constexpr bool has_add_method_v = has_add_method<T, U>::value;

	/** @brief Проверяет наличие метода Pow(T). (UE-стиль). */
	template<typename T, typename = void>
	struct has_pow_method : std::false_type {};
	template<typename T>
	struct has_pow_method<T, void_t<decltype(std::declval<T>().Pow(std::declval<T>()))>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_pow_method_v = has_pow_method<T>::value;

	/** @brief Проверяет наличие метода ToString(). */
	template<typename T, typename = void>
	struct has_to_string_method : std::false_type {};
	template<typename T>
	struct has_to_string_method<T, void_t<decltype(std::declval<T>().ToString())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_to_string_method_v = has_to_string_method<T>::value;

	/** @brief Проверяет наличие метода GetTicks() (FDateTime / FTimespan). */
	template<typename T, typename = void>
	struct has_get_ticks_method : std::false_type {};
	template<typename T>
	struct has_get_ticks_method<T, void_t<decltype(std::declval<T>().GetTicks())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_get_ticks_method_v = has_get_ticks_method<T>::value;

	/** @brief Проверяет наличие метода GetTotalSeconds() (FTimespan). */
	template<typename T, typename = void>
	struct has_get_total_seconds_method : std::false_type {};
	template<typename T>
	struct has_get_total_seconds_method<T, void_t<decltype(std::declval<T>().GetTotalSeconds())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_get_total_seconds_method_v = has_get_total_seconds_method<T>::value;

	/** @brief Проверяет наличие метода GetSize(). */
	template<typename T, typename = void>
	struct has_get_size_method : std::false_type {};
	template<typename T>
	struct has_get_size_method<T, void_t<decltype(std::declval<T>().GetSize())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_get_size_method_v = has_get_size_method<T>::value;

	/** @brief Проверяет наличие метода Length(). */
	template<typename T, typename = void>
	struct has_length_method : std::false_type {};
	template<typename T>
	struct has_length_method<T, void_t<decltype(std::declval<T>().Length())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_length_method_v = has_length_method<T>::value;

	/** @brief Проверяет наличие метода Num() (контейнеры UE). */
	template<typename T, typename = void>
	struct has_num_method : std::false_type {};
	template<typename T>
	struct has_num_method<T, void_t<decltype(std::declval<T>().Num())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_num_method_v = has_num_method<T>::value;

	/** @brief Проверяет наличие метода IsValid(). */
	template<typename T, typename = void>
	struct has_is_valid_method : std::false_type {};
	template<typename T>
	struct has_is_valid_method<T, void_t<decltype(std::declval<T>().IsValid())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_is_valid_method_v = has_is_valid_method<T>::value;

	/** @brief Проверяет наличие метода IsEmpty(). */
	template<typename T, typename = void>
	struct has_is_empty_method : std::false_type {};
	template<typename T>
	struct has_is_empty_method<T, void_t<decltype(std::declval<T>().IsEmpty())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_is_empty_method_v = has_is_empty_method<T>::value;

	/** @brief Проверяет наличие метода IsNull(). */
	template<typename T, typename = void>
	struct has_is_null_method : std::false_type {};
	template<typename T>
	struct has_is_null_method<T, void_t<decltype(std::declval<T>().IsNull())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_is_null_method_v = has_is_null_method<T>::value;

	/** @brief Проверяет наличие метода Clear(). */
	template<typename T, typename = void>
	struct has_clear_method : std::false_type {};
	template<typename T>
	struct has_clear_method<T, void_t<decltype(std::declval<T>().Clear())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_clear_method_v = has_clear_method<T>::value;

	/** @brief Проверяет наличие метода Reset(). */
	template<typename T, typename = void>
	struct has_reset_method : std::false_type {};
	template<typename T>
	struct has_reset_method<T, void_t<decltype(std::declval<T>().Reset())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_reset_method_v = has_reset_method<T>::value;

	/** @brief Проверяет наличие метода Empty(). */
	template<typename T, typename = void>
	struct has_empty_method : std::false_type {};
	template<typename T>
	struct has_empty_method<T, void_t<decltype(std::declval<T>().Empty())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_empty_method_v = has_empty_method<T>::value;

	/** @brief Проверяет наличие метода Contains(U). */
	template<typename T, typename U, typename = void>
	struct has_contains_method : std::false_type {};
	template<typename T, typename U>
	struct has_contains_method<T, U, void_t<decltype(std::declval<T>().Contains(std::declval<U>()))>> : std::true_type {};
	template<typename T, typename U>
	inline constexpr bool has_contains_method_v = has_contains_method<T, U>::value;

	/** @brief Проверяет наличие метода Find(U). */
	template<typename T, typename U, typename = void>
	struct has_find_method : std::false_type {};
	template<typename T, typename U>
	struct has_find_method<T, U, void_t<decltype(std::declval<T>().Find(std::declval<U>()))>> : std::true_type {};
	template<typename T, typename U>
	inline constexpr bool has_find_method_v = has_find_method<T, U>::value;

	/** @brief Проверяет наличие метода Serialize(Archive&). */
	template<typename T, typename Archive, typename = void>
	struct has_serialize_method : std::false_type {};
	template<typename T, typename Archive>
	struct has_serialize_method<T, Archive, void_t<decltype(std::declval<T>().Serialize(std::declval<Archive&>()))>> : std::true_type {};
	template<typename T, typename Archive>
	inline constexpr bool has_serialize_method_v = has_serialize_method<T, Archive>::value;
    
  
	
	
	
	
	// ========================================================================
	// Универсальные методы для строк / UObject
	// ========================================================================

	/** @brief Проверяет наличие метода IsNone() (FName). */
	template<typename T, typename = void>
	struct has_is_none_method : std::false_type {};
	template<typename T>
	struct has_is_none_method<T, void_t<decltype(std::declval<T>().IsNone())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_is_none_method_v = has_is_none_method<T>::value;

	/** @brief Проверяет наличие метода GetDisplayName() (UObject в редакторе). */
	template<typename T, typename = void>
	struct has_get_display_name_method : std::false_type {};
	template<typename T>
	struct has_get_display_name_method<T, void_t<decltype(std::declval<T>().GetDisplayName())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_get_display_name_method_v = has_get_display_name_method<T>::value;

	/** @brief Проверяет наличие метода GetClass() (UObject). */
	template<typename T, typename = void>
	struct has_get_class_method : std::false_type {};
	template<typename T>
	struct has_get_class_method<T, void_t<decltype(std::declval<T>().GetClass())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_get_class_method_v = has_get_class_method<T>::value;

	/** @brief Проверяет наличие метода GetName() (UObject / FName). 
	 * 
	 */
	template<typename T, typename = void>
	struct has_get_name_method : std::false_type {};
	template<typename T>
	struct has_get_name_method<T, void_t<decltype(std::declval<T>().GetName())>> : std::true_type {};
	template<typename T>
	inline constexpr bool has_get_name_method_v = has_get_name_method<T>::value;

	
	template<typename T, typename = void> \
	struct has_IsDefault_method : std::false_type {}; \
	template<typename T> \
	struct has_IsDefault_method<T, void_t<decltype(std::declval<T>().IsDefault())>> : std::true_type {};
	template<typename T> \
	inline constexpr bool has_IsDefault_method_v = has_IsDefault_method<T>::value;
	
	// ========================================================================
	// Комбинированные алиасы
	// ========================================================================

	/** @brief Тип сравним (есть ==, != или Equals). */
	template<typename T>
	inline constexpr bool is_comparable_v = has_equals_operator_v<T> || has_not_equals_operator_v<T> || has_equals_method_v<T>;

	/** @brief Тип упорядочиваем (есть <, > или Compare). */
	template<typename T>
	inline constexpr bool is_ordered_v = has_less_operator_v<T> || has_greater_operator_v<T> || has_compare_method_v<T>;

	/** @brief Тип имеет размер (контейнер / строка). */
	template<typename T>
	inline constexpr bool has_size_v = has_get_size_method_v<T> || has_length_method_v<T> || has_num_method_v<T>;

	/** @brief Тип можно очистить (Clear / Reset / Empty). */
	template<typename T>
	inline constexpr bool is_cleareable_v = has_clear_method_v<T> || has_reset_method_v<T> || has_empty_method_v<T>;

	/** @brief Тип является временным (FTimespan / FDateTime или имеет GetTicks()). */
	template<typename T>
	inline constexpr bool is_time_type_v = std::is_same_v<FTimespan, T> || std::is_same_v<FDateTime, T> || has_get_ticks_method_v<T>;

	/** @brief Тип является арифметическим ИЛИ временным. */
	template<typename T>
	inline constexpr bool is_time_arithmetic_v = std::is_arithmetic_v<T> || traits::is_time_type_v<T>;
    
    
	
	// ========================================================================
	// SafeUpType / SafeUpUnsignType — повышение типа для безопасного хранения
	// ========================================================================
	
	
    // ============================================================================
    // SafeUpType - определяет тип, способный вместить положительное значение диапазона T (от T Max - T Min)
    // ============================================================================
    
	/**
	 * @brief Базовый шаблон — для неподдерживаемых типов возвращает сам тип.
	 * @details SafeUpUnsignTypeImpl — повышение до беззнакового (или большего) типа.
	*/
	template<typename T, typename = void>
	struct SafeUpUnsignTypeImpl { using Type = T; };

	/**
	 * @brief Базовый шаблон — для неподдерживаемых типов возвращает сам тип.
	 * @details SafeUpTypeImpl — повышение до большего знакового типа.
	*/
	template<typename T, typename = void>
	struct SafeUpTypeImpl { using Type = T; };
    
    // signed integer → unsigned того же размера
	template<typename T>
	struct SafeUpUnsignTypeImpl<T, std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>>>
	{ using Type = std::make_unsigned_t<T>; };

	// unsigned integer → как есть
	template<typename T>
	struct SafeUpUnsignTypeImpl<T, std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>>>
	{ using Type = T; };

	// int32 → int64 (для безопасного хранения разницы Max-Min)
	template<typename T>
	struct SafeUpTypeImpl<T, std::enable_if_t<std::is_same_v<T, int32>>>
	{ using Type = int64; };

	// int8_t → uint16_t (т.к. диапазон int8: -128..127, разница = 255 — требует uint16)
	template<>
	struct SafeUpUnsignTypeImpl<int8_t, void> { using Type = uint16_t; };

	// int16_t → uint32_t (т.к. диапазон int16: -32768..32767, разница = 65535 — требует uint32)
	template<>
	struct SafeUpUnsignTypeImpl<int16_t, void> { using Type = uint32_t; };

	// float → double
	template<typename T>
	struct SafeUpUnsignTypeImpl<T, std::enable_if_t<std::is_same_v<T, float>>> { using Type = double; };
	template<typename T>
	struct SafeUpTypeImpl<T, std::enable_if_t<std::is_same_v<T, float>>> { using Type = double; };

	// double → long double
	template<typename T>
	struct SafeUpUnsignTypeImpl<T, std::enable_if_t<std::is_same_v<T, double>>> { using Type = long double; };
	template<typename T>
	struct SafeUpTypeImpl<T, std::enable_if_t<std::is_same_v<T, double>>> { using Type = long double; };

	// long double → long double
	template<typename T>
	struct SafeUpUnsignTypeImpl<T, std::enable_if_t<std::is_same_v<T, long double>>> { using Type = long double; };


	/** @brief Алиас: повышение типа до безопасного беззнакового. */
	template<typename T>
	using SafeUpUnType = SafeUpUnsignTypeImpl<T>::Type;

	/** @brief Алиас: повышение типа до большего знакового. */
	template<typename T>
	using SafeUpType = SafeUpTypeImpl<T>::Type;

	/** @brief Проверяет, имеет ли тип безопасное беззнаковое повышение. */
	template<typename T>
	inline constexpr bool has_safe_up_unsign_type_v = (std::is_unsigned_v<T> && std::is_same_v<SafeUpUnType<T>, T>)
	                                               || ((!std::is_unsigned_v<T>) && (!std::is_same_v<SafeUpUnType<T>, T>));

	/** @brief Проверяет, имеет ли тип безопасное повышение. */
	template<typename T>
	inline constexpr bool has_safe_up_type_v = !std::is_same_v<SafeUpType<T>, T>;
	
	
    
    // ========================================================================
	// Largest / Smaller — выбор большего/меньшего типа по sizeof
	// ========================================================================

	/** @brief Возвращает больший из двух типов по sizeof. */
	template<typename T, typename U>
	struct LargerTypeImpl
	{
	private:
		static constexpr bool T_is_larger = sizeof(T) >= sizeof(U);
	public:
		using Type = std::conditional_t<T_is_larger, T, U>;
	};
	template<typename T, typename U>
	using LargerType = LargerTypeImpl<T, U>::Type;

	/** @brief Возвращает меньший из двух типов по sizeof. */
	template<typename T, typename U>
	struct SmallerTypeImpl
	{
	private:
		static constexpr bool T_is_Smaller = sizeof(T) <= sizeof(U);
	public:
		using Type = std::conditional_t<T_is_Smaller, T, U>;
	};
	template<typename T, typename U>
	using SmallerType = SmallerTypeImpl<T, U>::Type;


	// ========================================================================
	// GreatestSafeType — наибольший безопасный тип для двух типов
	// ========================================================================

	/** @brief Наибольший безопасный беззнаковый тип для T и U. */
	template<typename T, typename U, typename = void>
	struct GreatestSafeUnTypeImpl
	{
		using SafeT = SafeUpUnType<T>;
		using SafeU = SafeUpUnType<U>;
		using Type = LargerType<SafeT, SafeU>;
	};
	template<typename T, typename U>
	using GreatestSafeUnType = GreatestSafeUnTypeImpl<T, U>::Type;

	/** @brief Наибольший безопасный знаковый тип для T и U. */
	template<typename T, typename U, typename = void>
	struct GreatestSafeTypeImpl
	{
		using SafeT = SafeUpType<T>;
		using SafeU = SafeUpType<U>;
		using Type = LargerType<SafeT, SafeU>;
	};
	template<typename T, typename U>
	using GreatestSafeType = GreatestSafeTypeImpl<T, U>::Type;


	// ========================================================================
	// GreatestSafeTargetType — безопасный тип с ограничением роста до TargetUpType
	// ========================================================================

	/**
	 * @brief Безопасный беззнаковый тип для T и U с ограничением роста до TargetUpType.
	 * @details Актуально для вложенных применений — чтобы тип не разрастался бесконечно.
	 */
	template<typename T, typename U, typename TargetUpType = GreatestSafeUnType<T, U>, typename = void>
	struct GreatestSafeUnTargetTypeImpl
	{
	private:
		static constexpr bool T_is_Target = sizeof(T) == sizeof(TargetUpType) || sizeof(U) == sizeof(TargetUpType);
		using SafeT = GreatestSafeUnType<T, U>;
	public:
		using Type = std::conditional_t<T_is_Target, TargetUpType, SafeT>;
	};
	template<typename T, typename U, typename TargetUpType = GreatestSafeUnType<T, U>>
	using GreatestSafeUnTargetType = GreatestSafeUnTargetTypeImpl<T, U, TargetUpType>::Type;
	
	/**
	 * @brief Альтернативный алиас: меньшее из TargetUpType и GreatestSafeUnType, но >= большему из T и U.
	*/
	template<typename TargetUpType, typename T, typename U>
	requires (has_safe_up_unsign_type_v<T> && has_safe_up_unsign_type_v<U> && has_safe_up_unsign_type_v<TargetUpType>)
	using TargetSafeUnType = LargerType< LargerType<T, U>, SmallerType<TargetUpType, GreatestSafeUnType<T, U>>>;

	/**
	 * @brief Безопасный знаковый тип для T и U с ограничением роста до TargetUpType.
	 */
	template<typename T, typename U, typename TargetUpType = GreatestSafeType<T, U>, typename = void>
	struct GreatestSafeTargetTypeImpl
	{
	private:
		static constexpr bool T_is_Target = sizeof(T) == sizeof(TargetUpType) || sizeof(U) == sizeof(TargetUpType);
		using SafeT = GreatestSafeType<T, U>;
	public:
		using Type = std::conditional_t<T_is_Target, TargetUpType, SafeT>;
	};
	template<typename T, typename U, typename TargetUpType = GreatestSafeType<T, U>>
	using GreatestSafeTargetType = GreatestSafeTargetTypeImpl<T, U, TargetUpType>::Type;

	/**
	 * @brief Альтернативный алиас: меньшее из TargetUpType и GreatestSafeType, но >= большему из T и U.
	 */
	template<typename TargetUpType, typename T, typename U>
	requires (has_safe_up_type_v<T> && has_safe_up_type_v<U> && has_safe_up_type_v<TargetUpType>)
	using TargetSafeType = LargerType< LargerType<T, U>, SmallerType<TargetUpType, GreatestSafeType<T, U>>>;

	// тест на noexcept
	template<typename T, typename U = T>
	inline constexpr bool is_nothrow_addable_v = noexcept(std::declval<T>() + std::declval<U>());

	// тест на noexcept
	template<typename T, typename U = T>
	inline constexpr bool is_nothrow_comparable_v = noexcept(std::declval<T>() == std::declval<U>());
	
}



	// ============================================================================
	// Макросы для быстрой генерации собственных трейтов (проверка наличия любых методов)
	// ============================================================================

/**
 * @def DEFINE_HAS_METHOD(method_name, return_type)
 * @brief Генерирует трейт has_<method_name>_method для проверки наличия метода без аргументов.
*/
#define DEFINE_HAS_METHOD(method_name, return_type) \
	namespace traits { \
		template<typename T, typename = void> \
		struct has_##method_name##_method : std::false_type {}; \
		template<typename T> \
		struct has_##method_name##_method<T, void_t<decltype(std::declval<T>().method_name())>> : std::true_type {}; \
		template<typename T> \
		inline constexpr bool has_##method_name##_method##_v = has_##method_name##_method<T>::value; \
	}

/**
 * @def DEFINE_HAS_METHOD_WITH_ARGS(method_name, ...)
 * @brief Генерирует трейт has_<method_name>_method для проверки наличия метода с аргументами.
*/
#define DEFINE_HAS_METHOD_WITH_ARGS(method_name, ...) \
	namespace traits { \
		template<typename T, typename = void> \
		struct has_##method_name##_method : std::false_type {}; \
		template<typename T> \
		struct has_##method_name##_method<T, void_t<decltype(std::declval<T>().method_name(__VA_ARGS__))>> : std::true_type {}; \
		template<typename T> \
		inline constexpr bool has_##method_name##_method##_v = has_##method_name##_method<T>::value; \
	}