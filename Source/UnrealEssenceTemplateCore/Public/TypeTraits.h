// Copyright UnrealEssence. All Rights Reserved.

#pragma once
#include <type_traits>
#include <string>

namespace traits
{
    
    // ========== Вспомогательная структура ==========
    template<typename...>
    using void_t = void;

    // ========== Арифметические операторы ==========
    
    // operator+
    template<typename T, typename U = T, typename = void>
    struct has_plus_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_plus_operator<T, U, void_t<decltype(std::declval<T>() + std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_plus_operator_v = has_plus_operator<T, U>::value;
    
    // operator-
    template<typename T, typename U = T, typename = void>
    struct has_minus_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_minus_operator<T, U, void_t<decltype(std::declval<T>() - std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_minus_operator_v = has_minus_operator<T, U>::value;
    
    // operator*
    template<typename T, typename U = T, typename = void>
    struct has_multiply_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_multiply_operator<T, U, void_t<decltype(std::declval<T>() * std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_multiply_operator_v = has_multiply_operator<T, U>::value;
    
    // operator/
    template<typename T, typename U = T, typename = void>
    struct has_divide_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_divide_operator<T, U, void_t<decltype(std::declval<T>() / std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_divide_operator_v = has_divide_operator<T, U>::value;
    
    // operator%
    template<typename T, typename U = T, typename = void>
    struct has_modulus_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_modulus_operator<T, U, void_t<decltype(std::declval<T>() % std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_modulus_operator_v = has_modulus_operator<T, U>::value;

    
// ========== Операторы присваивания ==========
    
    
    
    // operator=
    template<typename T, typename U = T, typename = void>
    struct has_assign_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_assign_operator<T, U, void_t<decltype(std::declval<T>() = std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_assign_operator_v = has_assign_operator<T, U>::value;
    
    
    
// ========== Арифметические операторы С ПРИСВАИВАНИЕМ !!! ==========
    

    
// operator+=
    template<typename T, typename U = T, typename = void>
    struct has_plus_assign_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_plus_assign_operator<T, U, void_t<decltype(std::declval<T>() += std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_plus_assign_operator_v = has_plus_assign_operator<T, U>::value;
// -=
    template<typename T, typename U = T, typename = void>
    struct has_minus_assign_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_minus_assign_operator<T, U, void_t<decltype(std::declval<T>() -= std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_minus_assign_operator_v = has_minus_assign_operator<T, U>::value;
// *=    
    template<typename T, typename U = T, typename = void>
    struct has_multiply_assign_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_multiply_assign_operator<T, U, void_t<decltype(std::declval<T>() *= std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_multiply_assign_operator_v = has_multiply_assign_operator<T, U>::value;
// /=    
    template<typename T, typename U = T, typename = void>
    struct has_divide_assign_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_divide_assign_operator<T, U, void_t<decltype(std::declval<T>() /= std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_divide_assign_operator_v = has_divide_assign_operator<T, U>::value;

// %=    
    template<typename T, typename U = T, typename = void>
    struct has_modulus_assign_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_modulus_assign_operator<T, U, void_t<decltype(std::declval<T>() %= std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_modulus_assign_operator_v = has_modulus_assign_operator<T, U>::value;
   
    
    
// ========== Операторы инкремента/декремента ==========
    
    
    
    // operator++ (префиксный)
    template<typename T, typename = void>
    struct has_pre_increment_operator : std::false_type {};
    
    template<typename T>
    struct has_pre_increment_operator<T, void_t<decltype(++std::declval<T>())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_pre_increment_operator_v = has_pre_increment_operator<T>::value;
    
    // operator++ (постфиксный)
    template<typename T, typename = void>
    struct has_post_increment_operator : std::false_type {};
    
    template<typename T>
    struct has_post_increment_operator<T, void_t<decltype(std::declval<T>()++)>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_post_increment_operator_v = has_post_increment_operator<T>::value;
    
    // operator-- (префиксный)
    template<typename T, typename = void>
    struct has_pre_decrement_operator : std::false_type {};
    
    template<typename T>
    struct has_pre_decrement_operator<T, void_t<decltype(--std::declval<T>())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_pre_decrement_operator_v = has_pre_decrement_operator<T>::value;
    
    // operator-- (постфиксный)
    template<typename T, typename = void>
    struct has_post_decrement_operator : std::false_type {};
    
    template<typename T>
    struct has_post_decrement_operator<T, void_t<decltype(std::declval<T>()--)>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_post_decrement_operator_v = has_post_decrement_operator<T>::value;

    
    
// ========== Логические операторы ==========
    
    
    // operator==
    template<typename T, typename U = T, typename = void>
    struct has_equals_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_equals_operator<T, U, void_t<decltype(std::declval<T>() == std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_equals_operator_v = has_equals_operator<T, U>::value;
    
    // operator!=
    template<typename T, typename U = T, typename = void>
    struct has_not_equals_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_not_equals_operator<T, U, void_t<decltype(std::declval<T>() != std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_not_equals_operator_v = has_not_equals_operator<T, U>::value;
    
    // operator<
    template<typename T, typename U = T, typename = void>
    struct has_less_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_less_operator<T, U, void_t<decltype(std::declval<T>() < std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_less_operator_v = has_less_operator<T, U>::value;
    
    // operator<=
    template<typename T, typename U = T, typename = void>
    struct has_less_equal_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_less_equal_operator<T, U, void_t<decltype(std::declval<T>() <= std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_less_equal_operator_v = has_less_equal_operator<T, U>::value;
    
    // operator>
    template<typename T, typename U = T, typename = void>
    struct has_greater_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_greater_operator<T, U, void_t<decltype(std::declval<T>() > std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_greater_operator_v = has_greater_operator<T, U>::value;
    
    // operator>=
    template<typename T, typename U = T, typename = void>
    struct has_greater_equal_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_greater_equal_operator<T, U, void_t<decltype(std::declval<T>() >= std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_greater_equal_operator_v = has_greater_equal_operator<T, U>::value;

    // ========== Логические операторы (булевы) ==========
    
    // operator&&
    template<typename T, typename U = T, typename = void>
    struct has_logical_and_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_logical_and_operator<T, U, void_t<decltype(std::declval<T>() && std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_logical_and_operator_v = has_logical_and_operator<T, U>::value;
    
    // operator||
    template<typename T, typename U = T, typename = void>
    struct has_logical_or_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_logical_or_operator<T, U, void_t<decltype(std::declval<T>() || std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_logical_or_operator_v = has_logical_or_operator<T, U>::value;
    
    // operator!
    template<typename T, typename = void>
    struct has_logical_not_operator : std::false_type {};
    
    template<typename T>
    struct has_logical_not_operator<T, void_t<decltype(!std::declval<T>())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_logical_not_operator_v = has_logical_not_operator<T>::value;

// ========== Бинарные операторы ==========
    
    // operator& (битовое И)
    template<typename T, typename U = T, typename = void>
    struct has_bitwise_and_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_bitwise_and_operator<T, U, void_t<decltype(std::declval<T>() & std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_bitwise_and_operator_v = has_bitwise_and_operator<T, U>::value;
    
    // operator| (битовое ИЛИ)
    template<typename T, typename U = T, typename = void>
    struct has_bitwise_or_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_bitwise_or_operator<T, U, void_t<decltype(std::declval<T>() | std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_bitwise_or_operator_v = has_bitwise_or_operator<T, U>::value;
    
    // operator^ (битовое XOR)
    template<typename T, typename U = T, typename = void>
    struct has_bitwise_xor_operator : std::false_type {};
    
    template<typename T, typename U>
    struct has_bitwise_xor_operator<T, U, void_t<decltype(std::declval<T>() ^ std::declval<U>())>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_bitwise_xor_operator_v = has_bitwise_xor_operator<T, U>::value;
    
    // operator~ (битовое НЕ)
    template<typename T, typename = void>
    struct has_bitwise_not_operator : std::false_type {};
    
    template<typename T>
    struct has_bitwise_not_operator<T, void_t<decltype(~std::declval<T>())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_bitwise_not_operator_v = has_bitwise_not_operator<T>::value;


    
    

    
    // ========== Операторы доступа ==========
    
    
    
// operator[] (индексация)
    template<typename T, typename Index = size_t, typename = void>
    struct has_subscript_operator : std::false_type {};
    
    template<typename T, typename Index>
    struct has_subscript_operator<T, Index, void_t<decltype(std::declval<T>()[std::declval<Index>()])>>
        : std::true_type {};
    
    template<typename T, typename Index = size_t>
    inline constexpr bool has_subscript_operator_v = has_subscript_operator<T, Index>::value;
    
// operator* (разыменование)
    template<typename T, typename = void>
    struct has_dereference_operator : std::false_type {};
    
    template<typename T>
    struct has_dereference_operator<T, void_t<decltype(*std::declval<T>())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_dereference_operator_v = has_dereference_operator<T>::value;
    
// operator-> (доступ к члену)
    template<typename T, typename = void>
    struct has_member_access_operator : std::false_type {};
    
    template<typename T>
    struct has_member_access_operator<T, void_t<decltype(std::declval<T>().operator->())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_member_access_operator_v = has_member_access_operator<T>::value;

    
    
// ========== Методы-аналоги операторов (UE стиль) ==========
    
    
    
// Методы сравнения
    template<typename T, typename U = T, typename = void>
    struct has_equals_method : std::false_type {};
    
    template<typename T, typename U>
    struct has_equals_method<T, U, void_t<decltype(std::declval<T>().Equals(std::declval<U>()))>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_equals_method_v = has_equals_method<T, U>::value;
    
// Compare/CompareTo метод
    template<typename T, typename U = T, typename = void>
    struct has_compare_method : std::false_type {};
    
    template<typename T, typename U>
    struct has_compare_method<T, U, void_t<decltype(std::declval<T>().Compare(std::declval<U>()))>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_compare_method_v = has_compare_method<T, U>::value;
    
// Методы сложения (Add, Plus)
    template<typename T, typename U = T, typename = void>
    struct has_add_method : std::false_type {};
    
    template<typename T, typename U>
    struct has_add_method<T, U, void_t<decltype(std::declval<T>().Add(std::declval<U>()))>>
        : std::true_type {};
    
    template<typename T, typename U = T>
    inline constexpr bool has_add_method_v = has_add_method<T, U>::value;
    
// Пguiuguoghohiophiphjipjphoojpkj[pkpk]
    template<typename T, typename U, typename = void>
    struct has_pow_function : std::false_type {};

    template<typename T, typename U>
    struct has_pow_function<T, U, void_t<decltype(std::is_empty_v<T>(std::declval<T>(), std::declval<U>()))>>
        : std::true_type {};

    template<typename T, typename U>
    inline constexpr bool has_pow_function_v = has_pow_function<T, U>::value;

// Проверка наличия метода Pow (UE стиль)
    template<typename T, typename = void>
    struct has_pow_method : std::false_type {};

    template<typename T>
    struct has_pow_method<T, void_t<decltype(std::declval<T>().Pow(std::declval<T>()))>>
        : std::true_type {};

    template<typename T>
    inline constexpr bool has_pow_method_v = has_pow_method<T>::value;
    
// Метод ToString (классика UE)
    template<typename T, typename = void>
    struct has_to_string_method : std::false_type {};
    
    template<typename T>
    struct has_to_string_method<T, void_t<decltype(std::declval<T>().ToString())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_to_string_method_v = has_to_string_method<T>::value;
    
// Метод GetTicks (для FDateTime/FTimespan)
    template<typename T, typename = void>
    struct has_get_ticks_method : std::false_type {};
    
    template<typename T>
    struct has_get_ticks_method<T, void_t<decltype(std::declval<T>().GetTicks())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_get_ticks_method_v = has_get_ticks_method<T>::value;
    
// Метод GetTotalSeconds (для FTimespan)
    template<typename T, typename = void>
    struct has_get_total_seconds_method : std::false_type {};
    
    template<typename T>
    struct has_get_total_seconds_method<T, void_t<decltype(std::declval<T>().GetTotalSeconds())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_get_total_seconds_method_v = has_get_total_seconds_method<T>::value;
    
// GetSize/Length/Num (контейнеры)
    template<typename T, typename = void>
    struct has_get_size_method : std::false_type {};
    
    template<typename T>
    struct has_get_size_method<T, void_t<decltype(std::declval<T>().GetSize())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_get_size_method_v = has_get_size_method<T>::value;
    
    template<typename T, typename = void>
    struct has_length_method : std::false_type {};
    
    template<typename T>
    struct has_length_method<T, void_t<decltype(std::declval<T>().Length())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_length_method_v = has_length_method<T>::value;
    
    template<typename T, typename = void>
    struct has_num_method : std::false_type {};
    
    template<typename T>
    struct has_num_method<T, void_t<decltype(std::declval<T>().Num())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_num_method_v = has_num_method<T>::value;
    
// IsValid/IsEmpty/IsNull (проверки состояния)
    template<typename T, typename = void>
    struct has_is_valid_method : std::false_type {};
    
    template<typename T>
    struct has_is_valid_method<T, void_t<decltype(std::declval<T>().IsValid())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_is_valid_method_v = has_is_valid_method<T>::value;
    
    template<typename T, typename = void>
    struct has_is_empty_method : std::false_type {};
    
    template<typename T>
    struct has_is_empty_method<T, void_t<decltype(std::declval<T>().IsEmpty())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_is_empty_method_v = has_is_empty_method<T>::value;
    
    template<typename T, typename = void>
    struct has_is_null_method : std::false_type {};
    
    template<typename T>
    struct has_is_null_method<T, void_t<decltype(std::declval<T>().IsNull())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_is_null_method_v = has_is_null_method<T>::value;
    
// Clear/Reset/Empty (методы очистки)
    template<typename T, typename = void>
    struct has_clear_method : std::false_type {};
    
    template<typename T>
    struct has_clear_method<T, void_t<decltype(std::declval<T>().Clear())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_clear_method_v = has_clear_method<T>::value;
    
    template<typename T, typename = void>
    struct has_reset_method : std::false_type {};
    
    template<typename T>
    struct has_reset_method<T, void_t<decltype(std::declval<T>().Reset())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_reset_method_v = has_reset_method<T>::value;
    
    template<typename T, typename = void>
    struct has_empty_method : std::false_type {};
    
    template<typename T>
    struct has_empty_method<T, void_t<decltype(std::declval<T>().Empty())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_empty_method_v = has_empty_method<T>::value;
    
// Contains/Find (поиск)
    template<typename T, typename U, typename = void>
    struct has_contains_method : std::false_type {};
    
    template<typename T, typename U>
    struct has_contains_method<T, U, void_t<decltype(std::declval<T>().Contains(std::declval<U>()))>>
        : std::true_type {};
    
    template<typename T, typename U>
    inline constexpr bool has_contains_method_v = has_contains_method<T, U>::value;
// Find  
    template<typename T, typename U, typename = void>
    struct has_find_method : std::false_type {};
    
    template<typename T, typename U>
    struct has_find_method<T, U, void_t<decltype(std::declval<T>().Find(std::declval<U>()))>>
        : std::true_type {};
    
    template<typename T, typename U>
    inline constexpr bool has_find_method_v = has_find_method<T, U>::value;
    
// Serialization (UE специфика)
    template<typename T, typename Archive, typename = void>
    struct has_serialize_method : std::false_type {};
    
    template<typename T, typename Archive>
    struct has_serialize_method<T, Archive, void_t<decltype(std::declval<T>().Serialize(std::declval<Archive&>()))>>
        : std::true_type {};
    
    template<typename T, typename Archive>
    inline constexpr bool has_serialize_method_v = has_serialize_method<T, Archive>::value;

    
    
    
    // ========== Универсальные методы для строк (FString, FName, FText) ==========
    
    template<typename T, typename = void>
    struct has_is_none_method : std::false_type {};
    
    template<typename T>
    struct has_is_none_method<T, void_t<decltype(std::declval<T>().IsNone())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_is_none_method_v = has_is_none_method<T>::value;
    
// GetDisplayName (для UObject в редакторе)
    template<typename T, typename = void>
    struct has_get_display_name_method : std::false_type {};
    
    template<typename T>
    struct has_get_display_name_method<T, void_t<decltype(std::declval<T>().GetDisplayName())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_get_display_name_method_v = has_get_display_name_method<T>::value;
    
// GetClass (для UObject)
    template<typename T, typename = void>
    struct has_get_class_method : std::false_type {};
    
    template<typename T>
    struct has_get_class_method<T, void_t<decltype(std::declval<T>().GetClass())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_get_class_method_v = has_get_class_method<T>::value;
    
// GetName (для UObject/FName)
    template<typename T, typename = void>
    struct has_get_name_method : std::false_type {};
    
    template<typename T>
    struct has_get_name_method<T, void_t<decltype(std::declval<T>().GetName())>>
        : std::true_type {};
    
    template<typename T>
    inline constexpr bool has_get_name_method_v = has_get_name_method<T>::value;
    
    
    
// ========== Комбинированные проверки (удобные алиасы) ==========
    
    // Является ли тип сравнимым (хоть как-то)
    template<typename T>
    inline constexpr bool is_comparable_v = has_equals_operator_v<T> || has_not_equals_operator_v<T> || has_equals_method_v<T>;
    
    // Является ли тип упорядочиваемым
    template<typename T>
    inline constexpr bool is_ordered_v = has_less_operator_v<T> || has_greater_operator_v<T> || has_compare_method_v<T>;
    
    // Имеет ли тип размер (контейнер или строка)
    template<typename T>
    inline constexpr bool has_size_v = has_get_size_method_v<T> || has_length_method_v<T> || has_num_method_v<T>;
    
    // Можно ли очистить
    template<typename T>
    inline constexpr bool is_cleareable_v = has_clear_method_v<T> || has_reset_method_v<T> || has_empty_method_v<T>;
    
    
    
 

// Complex type check    
    
    template<typename T>
    inline constexpr bool is_time_type_v = std::is_same_v<FTimespan, T> || std::is_same_v<FDateTime, T> || has_get_ticks_method_v<T>;
    
    template<typename T>
    inline constexpr bool is_time_arithmetic_v = std::is_arithmetic_v<T> || traits::is_time_type_v<T>;
    
    
    // ============================================================================
    // SafeUpType - определяет тип, способный вместить положительное значение диапазона T (от T Max - T Min)
    // ============================================================================
    
    // ----- Базовый шаблон (по умолчанию - сам тип) -----
    template<typename T, typename = void>
    struct SafeUpUnsignTypeImpl
    {
        using Type = T;  // Для неподдерживаемых типов
    };
    
    template<typename T, typename = void>
    struct SafeUpTypeImpl
    {
        using Type = T;  // Для неподдерживаемых типов
    };
    
    // ----- Целочисленные типы (std::is_integral) -----
    
    // Для signed целых - преобразуем в unsigned того же или большего размера
    template<typename T>
    struct SafeUpUnsignTypeImpl<T, std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>>>
    {
        using Type = std::make_unsigned_t<T>;  // int -> unsigned int, long long -> unsigned long long
    };
    
    // Для unsigned целых - оставляем как есть (уже неотрицательные)
    template<typename T>
    struct SafeUpUnsignTypeImpl<T, std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>>>
    {
        using Type = T;  // unsigned уже безопасен
    };
    
    template<typename T>
    struct SafeUpTypeImpl<T, std::enable_if_t<std::is_same_v<T, int32>>>
    {
        using Type = int64;
    };
    
    // Специальный случай: int8_t/char - расширяем до uint16_t для безопасности
    template<>
    struct SafeUpUnsignTypeImpl<int8_t, void>
    {
        using Type = uint16_t;  // int8_t может быть до 127, uint8_t только до 255 - лучше uint16_t
    };
    
    template<>
    struct SafeUpUnsignTypeImpl<int16_t, void>
    {
        using Type = uint32_t;  // int16_t: -32768..32767, uint16_t: 0..65535 - достаточно
    };
    
    // ----- Типы с плавающей запятой -----
    
    // float -> double (больше точность и диапазон)
    template<typename T>
    struct SafeUpUnsignTypeImpl<T, std::enable_if_t<std::is_same_v<T, float>>>
    {
        using Type = double;
    };
    
    template<typename T>
    struct SafeUpTypeImpl<T, std::enable_if_t<std::is_same_v<T, float>>>
    {
        using Type = double;
    };
    
    // double -> long double
    template<typename T>
    struct SafeUpUnsignTypeImpl<T, std::enable_if_t<std::is_same_v<T, double>>>
    {
        using Type = long double;
    };
    
    template<typename T>
    struct SafeUpTypeImpl<T, std::enable_if_t<std::is_same_v<T, double>>>
    {
        using Type = long double;
    };
    
    // long double -> long double (оставляем как есть)
    template<typename T>
    struct SafeUpUnsignTypeImpl<T, std::enable_if_t<std::is_same_v<T, long double>>>
    {
        using Type = long double;
    };
    
    
    // ----- Удобный алиас с Unsign для int type -----
    template<typename T>
    using SafeUpUnType = SafeUpUnsignTypeImpl<T>::Type;
    
    // для повышения типа всегда не в Unsign вариант, а сразу в больший, но поддержка меньше типов чем SafeUpUnType
    template<typename T>
    using SafeUpType = SafeUpTypeImpl<T>::Type;
    
    // ----- Проверка на возможность преобразования -----
    template<typename T>
    inline constexpr bool has_safe_up_unsign_type_v = (std::is_unsigned_v<T> && std::is_same_v<SafeUpUnType<T>, T>) || ((!std::is_unsigned_v<T>) && (!std::is_same_v<SafeUpUnType<T>, T>));
    
    template<typename T>
    inline constexpr bool has_safe_up_type_v = !std::is_same_v<SafeUpType<T>, T>;
    
    // ============================================================================
    // GreatestSafeType - наибольший безопасный тип для двух типов
    // ============================================================================
    
    // ----- Вспомогательный трейт: какой тип больше -----
    template<typename T, typename U>
    struct LargerTypeImpl
    {
    private:
        static constexpr bool T_is_larger = sizeof(T) >= sizeof(U);
    public:
        using Type = std::conditional_t<T_is_larger, T, U>;
    };
    
    // алиас
    template<typename T, typename U>
    using LargerType = LargerTypeImpl<T, U>::Type;
    
    
    // ----- Вспомогательный трейт: какой тип меньше -----
    template<typename T, typename U>
    struct SmallerTypeImpl
    {
    private:
        static constexpr bool T_is_Smaller = sizeof(T) <= sizeof(U);
    public:
        using Type = std::conditional_t<T_is_Smaller, T, U>;
    };
    
    // алиас
    template<typename T, typename U>
    using SmallerType = SmallerTypeImpl<T, U>::Type;
    
    
    
    // ----- Получение безопасного типа для двух типов -----
    template<typename T, typename U, typename = void>
    struct GreatestSafeUnTypeImpl
    {
        // Получаем безопасные версии каждого типа
        using SafeT = SafeUpUnType<T>;
        using SafeU = SafeUpUnType<U>;
        
        // Берём больший из них
        using Type = LargerType<SafeT, SafeU>;
    };
    
    // алиас
    template<typename T, typename U>
    using GreatestSafeUnType = GreatestSafeUnTypeImpl<T, U>::Type;
    
    // ----- Получение безопасного типа для двух типов (без Unsign)-----
    template<typename T, typename U, typename = void>
    struct GreatestSafeTypeImpl
    {
        // Получаем безопасные версии каждого типа
        using SafeT = SafeUpType<T>;
        using SafeU = SafeUpType<U>;
        
        // Берём больший из них
        using Type = LargerType<SafeT, SafeU>;
    };
    
    // алиас
    template<typename T, typename U>
    using GreatestSafeType = GreatestSafeTypeImpl<T, U>::Type;
    
    
    // ----- Получение безопасного типа для двух типов с ограничением роста типа до TargetType (актуально для вложенных применений безопасной функции -> для ограничения нужно указать целевой тип, в противном случае ограничения не будет)-----
    template<typename T, typename U, typename TargetUpType = GreatestSafeUnType<T, U>, typename = void>
    struct GreatestSafeUnTargetTypeImpl
    {
    private:
        static constexpr bool T_is_Target = sizeof(T) == sizeof(TargetUpType) || sizeof(U) == sizeof(TargetUpType); // condition
        using SafeT = GreatestSafeUnType<T, U>; // option Safe Type
    public:
        // Берём больший из безопасной версии меньшего и большего
        using Type = std::conditional_t<T_is_Target, TargetUpType, SafeT>;
    };
    
    // алиас
    template<typename T, typename U, typename TargetUpType = GreatestSafeUnType<T, U> >
    using GreatestSafeUnTargetType = GreatestSafeUnTargetTypeImpl<T, U, TargetUpType>::Type;
    
    // алиас 2 (меньшее из TargetUpType и GreatestSafeType, но >= чем больший тип из T и U)
    template<typename TargetUpType, typename T, typename U>
    requires (has_safe_up_unsign_type_v<T> && has_safe_up_unsign_type_v<U> && has_safe_up_unsign_type_v<TargetUpType>)
    using TargetSafeUnType = LargerType< LargerType<T, U>, SmallerType<TargetUpType, GreatestSafeUnType<T, U>>>;
    
    // ----- Получение безопасного типа для двух типов с ограничением роста типа до TargetType (актуально для вложенных применений безопасной функции -> для ограничения нужно указать целевой тип, в противном случае ограничения не будет)-----
    template<typename T, typename U, typename TargetUpType = GreatestSafeType<T, U>, typename = void>
    struct GreatestSafeTargetTypeImpl
    {
    private:
        static constexpr bool T_is_Target = sizeof(T) == sizeof(TargetUpType) || sizeof(U) == sizeof(TargetUpType); // condition
        using SafeT = GreatestSafeType<T, U>; // option Safe Type
    public:
        // Берём больший из безопасной версии меньшего и большего
        using Type = std::conditional_t<T_is_Target, TargetUpType, SafeT>;
    };
    
    // алиас
    template<typename T, typename U, typename TargetUpType = GreatestSafeType<T, U> >
    using GreatestSafeTargetType = GreatestSafeTargetTypeImpl<T, U, TargetUpType>::Type;
    
    // алиас 2 (меньшее из TargetUpType и GreatestSafeType, но >= чем больший тип из T и U)
    template<typename TargetUpType, typename T, typename U>
    requires (has_safe_up_type_v<T> && has_safe_up_type_v<U> && has_safe_up_type_v<TargetUpType>)
    using TargetSafeType = LargerType< LargerType<T, U>, SmallerType<TargetUpType, GreatestSafeType<T, U>>>;
    
}






// Макросы для быстрого создания новых проверок (опционально)

#define DEFINE_HAS_METHOD(method_name, return_type) \
    namespace traits { \
        template<typename T, typename = void> \
        struct has_##method_name##_method : std::false_type {}; \
        \
        template<typename T> \
        struct has_##method_name##_method<T, void_t<decltype(std::declval<T>().method_name())>> \
            : std::true_type {}; \
        \
        template<typename T> \
        inline constexpr bool has_##method_name##_method##_v = has_##method_name##_method<T>::value; \
    }

#define DEFINE_HAS_METHOD_WITH_ARGS(method_name, ...) \
    namespace traits { \
        template<typename T, typename = void> \
        struct has_##method_name##_method : std::false_type {}; \
        \
        template<typename T> \
        struct has_##method_name##_method<T, void_t<decltype(std::declval<T>().method_name(__VA_ARGS__))>> \
            : std::true_type {}; \
        \
        template<typename T> \
        inline constexpr bool has_##method_name##_method##_v = has_##method_name##_method<T>::value; \
    }

// Использование макросов для генерации проверок прочих методов:
namespace traits
{
    //DEFINE_HAS_METHOD(GetSize);
    //DEFINE_HAS_METHOD(Serialize);
    //DEFINE_HAS_METHOD(IsValid);
}