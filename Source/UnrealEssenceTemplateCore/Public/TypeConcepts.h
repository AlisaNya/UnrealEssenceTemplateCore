// Copyright UnrealEssence. All Rights Reserved.

#pragma once

#include <type_traits>
#include <concepts>

// C++20 версия TypeTraits на концептах (поддержка в ранних версиях UE5 ограничена !!! в них как и в UE4 используем TypeTraits вариант)

namespace concepts
{
	// complex type check
	template<typename T>
	concept IsArithmeticAndTime = std::is_arithmetic_v<T> || std::is_same_v<FTimespan, T> || std::is_same_v<FDateTime, T>;;
	
	// ========== Базовые проверки через concepts ==========
    
	template<typename T>
	concept HasIsValid = requires(const T& t) { t.IsValid(); };
    
	template<typename T>
	concept HasIsEmpty = requires(const T& t) { t.IsEmpty(); };
    
	template<typename T>
	concept HasIsDefault = requires(const T& t) { t.IsDefault(); };
    
	template<typename T>
	concept HasNum = requires(const T& t) { t.Num(); };
    
	template<typename T>
	concept HasEqualsOperator = requires(const T& a, const T& b) { a == b; };
    
	template<typename T>
	concept HasToString = requires(const T& t) { { t.ToString() } -> std::convertible_to<FString>; };
	
	
	// ========== Функция проверки на дефолтность (C++20 концепты) ==========
    
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