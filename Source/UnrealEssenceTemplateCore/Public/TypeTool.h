// Copyright UnrealEssence. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TypeTraits.h"
#include "TypeConcepts.h"
#include "TypeLimits.h"
#include "SimpleLOG.h"

/**
 * @file TypeTool.h
 * @brief Адаптеры для единообразной бесшовной работы с арифметическими и временными типами.
 *
 * @details Предоставляет две структуры:
 *          - TType     — адаптер для безопасной конвертации и ограничения значений
 *          - TTypeTool — универсальные утилиты (IsDefault, ToString, GetString)
 *
 * @note Поддерживается на любой версии UE — детект временных типов идёт через TypeTraits
 *       (по наличию метода .GetTicks()), что позволяет расширять поддержку на кастомные типы.
 * @version 0.1
 */


// ============================================================================
// TType — адаптер для безопасной конвертации и ограничения
// ============================================================================

/**
 * @struct TType
 * @brief Структура-адаптер для единообразной бесшовной обработки и безопасной конвертации
 *        временных и арифметических типов.
 *
 * @details Ключевая идея: временные типы (FTimespan, FDateTime) приводятся к int64-тикам,
 *          обрабатываются арифметически, а затем восстанавливаются в исходный тип.
 *          Все операции сопровождаются ограничениями (clamp) для предотвращения переполнений.
 *
 * @warning Поддерживается на любой версии UE — детект временных типов через TypeTraits
 *          (по наличию метода .GetTicks()). Вы можете добавить поддержку своих типов,
 *          просто реализовав в них метод GetTicks().
*/
struct TType
{
	/**
	 * @brief Преобразует временной тип в int64 тики, остальные типы возвращает как есть.
	 *
	 * @details Служит "прослойкой" перед арифметической обработкой:
	 *          - Для FTimespan / FDateTime (и любых типов с GetTicks()) → возвращает int64 тики
	 *          - Для остальных типов → возвращает значение без изменений
	 *
	 * @tparam T Тип входного значения
	 * @param  A Входное значение
	 * @return int64 (тики) для временных типов, либо само значение для остальных
	 *
	 * @note Для обратного преобразования используйте TType::DataRepair<SourceType>(Result).
	 * @see DataRepair
	*/
	template <typename T>
	[[nodiscard]] static constexpr UE_FORCEINLINE_HINT auto DataAdapter(T A)
	{
		if constexpr (traits::is_time_type_v<T>) { return A.GetTicks(); }
		else                                     { return A; }
	}
	
	
	/**
	 * @brief Восстанавливает временной тип из int64 тиков, остальные типы кастит в целевой.
	 *
	 * @details Служит "прослойкой" после арифметической обработки:
	 *          - Если TargetType — временной и InType == int64 → конструирует TargetType из тиков
	 *          - Иначе → выполняет обычный каст (TargetType)A
	 *
	 * @tparam TargetType Целевой (исходный) тип результата
	 * @tparam InType     Тип входного значения (обычно int64 или арифметический)
	 * @param  A          Входное значение
	 * @return Значение в TargetType
	 *
	 * @warning Проверку размерности и допустимых значений не выполняет !!! 
	 * - Только восстановление типа как ответная часть адаптера - все проверки делаем до ее вызова !!! 
	 * - Если нужно преобразование с проверками используйте - DataTranslator, либо TypeLimits - для просто ограничения без сложных преобразований!!!
	 * @note Для прямого преобразования используйте TType::DataAdapter(InValue).
	 * @see DataAdapter
	*/
	template <typename TargetType, typename InType>
	requires (std::is_convertible_v<InType, TargetType> || (traits::is_time_type_v<TargetType> && std::is_same_v<int64, InType>))
	[[nodiscard]] static constexpr UE_FORCEINLINE_HINT auto DataRepair(InType A)
	{
		if constexpr (traits::is_time_type_v<TargetType> && std::is_same_v<int64, InType>)
		{
			return TargetType(A);
		}
		else if constexpr (!traits::is_time_type_v<TargetType>)
		{
			return (TargetType)A;
		}
		else
		{
			ERROR("No Support Type - Cast InType to TargetType is Fail. returns - value source type");
			return A;
		}
	}
	
	
	/**
	 * @brief Ограничивает значение в пределах безопасного диапазона целевого типа.
	 *
	 * @details Возвращает результат В ИСХОДНОМ ТИПE (InType).
	 *          - Для временных InType: clamp тиков в диапазоне [MinTicks(TargetType), MaxTicks(TargetType)]
	 *            и восстановление в исходный временной тип
	 *          - Для арифметических InType: clamp в диапазоне целевого типа
	 *
	 * @tparam TargetType Целевой тип (должен быть арифметическим или временным)
	 * @tparam InType     Тип входного значения
	 * @param  A          Входное значение
	 * @return Значение типа InType, ограниченное диапазоном TargetType
	 *
	 * @note Если нужно получить результат в виде тиков (для временных типов), используйте TypeTLim.
	 * @see TypeTLim
	*/
	template <typename TargetType, typename InType>
	requires (traits::is_time_arithmetic_v<TargetType>)
	[[nodiscard]] static constexpr UE_FORCEINLINE_HINT InType TypeLimits(InType A)
	{
		if constexpr (traits::is_time_type_v<InType>)
		{
			return TType::DataRepair<InType>(FMath::Clamp(TType::DataAdapter(A), (int64)TTypeLimits<TargetType>::MinTicks(), (int64)TTypeLimits<TargetType>::MaxTicks()));
		}
		else if constexpr (std::is_arithmetic_v<InType>)
		{
			return FMath::Clamp(TType::DataAdapter(A), (InType)TTypeLimits<TargetType>::MinTicks(), (InType)TTypeLimits<TargetType>::MaxTicks());
		}
		else
		{
			WARNING("No Support Type - No Arithmetic, No TimeType - returns = source value");
			return A;
		}
	};
	
	/**
	 * @brief Ограничивает значение в пределах безопасного диапазона целевого типа.
	 *
	 * @details Возвращает результат В ВИДЕ ТИКОВ (int64) для временных типов.
	 *          - Для временных InType: clamp тиков → возврат int64
	 *          - Для арифметических InType: clamp в диапазоне целевого типа
	 *
	 * @tparam TargetType Целевой тип (должен быть арифметическим или временным)
	 * @tparam InType     Тип входного значения
	 * @param  A          Входное значение
	 * @return Значение типа InType или int64 (для временных типов)
	 *
	 * @note Если нужно вернуть результат в исходном временном типе, используйте TypeLimits.
	 * @see TypeLimits
	*/
	template <typename TargetType, typename InType>
	requires (traits::is_time_arithmetic_v<TargetType>)
	[[nodiscard]] static constexpr UE_FORCEINLINE_HINT auto TypeTLim(InType A)
	{
		if constexpr (traits::is_time_type_v<InType>)
		{
			return FMath::Clamp(TType::DataAdapter(A), (int64)TTypeLimits<TargetType>::MinTicks() , (int64)TTypeLimits<TargetType>::MaxTicks());
		}
		else if constexpr (std::is_arithmetic_v<InType>)
		{
			return FMath::Clamp(TType::DataAdapter(A), (InType)TTypeLimits<TargetType>::MinTicks() , (InType)TTypeLimits<TargetType>::MaxTicks());
		}
		else 
		{
			WARNING("No Support Type - No Arithmetic, No TimeType - returns = source value");
			return A;
		}
	};
	
	/**
	 * @brief Полный цикл безопасного всенаправленного преобразования между любыми арифметическими и временными типами: адаптация → ограничение → восстановление.
	 *
	 * @details Производит:
	 *          1. DataAdapter  — приведение к тикам (для временных) или как есть
	 *          2. TypeTLim     — ограничение под безопасный диапазон LimitFromType / OutType
	 *          3. DataRepair   — восстановление в OutType
	 *
	 * @tparam OutType       Выходной тип (и целевой для ограничений, если LimitFromType не задан)
	 * @tparam LimitFromType Тип, чей безопасный диапазон использовать для ограничения (по умолчанию = OutType) 
	 * используется в сложных сценариях с промежуточным преобразованием, если не задан то промежуточный этап пропускается!
	 * @tparam InType        Тип входного значения
	 * @param  A             Входное значение
	 * @return Значение типа OutType, ограниченное безопасным диапазоном
	 *
	 * @note Если LimitFromType == OutType — идёт упрощённая цепочка.
	 *       Если LimitFromType != OutType — значение ограничивается сначала под LimitFromType,
	 *       затем под OutType (двойная защита для сложных сценариев).
	*/
	template <typename OutType, typename LimitFromType = OutType, typename InType>
	requires (((std::is_convertible_v<InType, OutType> && !traits::is_time_type_v<InType>) || traits::is_time_type_v<InType>) && traits::is_time_arithmetic_v<LimitFromType>)
	[[nodiscard]] static constexpr UE_FORCEINLINE_HINT OutType DataTranslator(InType A)
	{
		// // идем по упрощенной цепочке если LimitFromType не задан и, следовательно, = OutType
		// if constexpr (std::is_same_v<LimitFromType, OutType>)
		// {
		// 	// ограничиваемся выходным типом
		// 	return TType::DataRepair<OutType>(TType::TypeTLim<LimitFromType>(TType::DataAdapter(A)));
		// } 
		// else
		// {
		// 	// ограничиваемся промежуточным типом отличным от выходного, но с учетом чтоб точно вместиться в выходной тип (2 этапа !!!)!!!
		// 	return TType::DataRepair<OutType>(TType::TypeTLim<OutType>(TType::TypeTLim<LimitFromType>(TType::DataAdapter(A))));
		// };
		
		// альтернатива через Traits (за 1 этап все ограничения!!!) ограничиваемся сразу тем типом который меньше (EXPERIMENTAL!!!)
		return TType::DataRepair<OutType>(TType::TypeTLim<traits::SmallerType<OutType, LimitFromType>>(TType::DataAdapter(A)));
	};
};




	// ============================================================================
	// TTypeTool — универсальные утилиты (IsDefault, ToString, GetString)
	// ============================================================================

/**
 * @struct TTypeTool
 * @brief Набор универсальных утилит для работы с любыми типами:
 *        - IsDefault — определение дефолтности значения
 *        - ToString  — универсальное преобразование в FString
 *        - GetString — ToString с опциональной очисткой дефолтных значений
*/
struct TTypeTool
{
	
	/**
	 * @brief Определяет, является ли TPair дефолтным (ключ ИЛИ значение дефолтны).
	 * @tparam InKeyType   Тип ключа
	 * @tparam InValueType Тип значения
	 * @param  Element     Проверяемая пара
	 * @return true, если ключ ИЛИ значение являются дефолтными
	*/
	template <typename InKeyType, typename InValueType>
	[[nodiscard]] static constexpr UE_FORCEINLINE_HINT bool IsDefault(const TPair<InKeyType, InValueType>& Element)
	{
		return TTypeTool::IsDefault(Element.Key) || TTypeTool::IsDefault(Element.Value);
	};
	
	
	/**
	 * @brief Универсальное определение дефолтности значения для своего типа
	 *
	 * @details Проверяет дефолтность по цепочке приоритетов:
	 *          1. Указатели → сравнение с nullptr
	 *          2. Арифметика и enum → сравнение с 0
	 *          3. Метод IsDefault()
	 *          4. Метод IsEmpty()
	 *          5. Метод IsNull()
	 *          6. Метод IsValid()
	 *          7. Метод Num() == 0 (контейнеры UE)
	 *          8. Метод empty() (STL-контейнеры)
	 *          9. Метод Equals() с дефолтно-сконструированным объектом (например FTransform) 
	 *          10. Оператор == с дефолтно-сконструированным объектом
	 *          11. Иначе → false (тип не поддерживается)
	 *
	 * @tparam T   Тип значения
	 * @param  Value  Проверяемое значение
	 * @return True  если значение считается дефолтным
	 *
	 * @note Поддерживает кастомные типы — достаточно реализовать один из методов
	 *       IsDefault() / IsEmpty() / IsValid() / IsNull() / Num() / empty() / Equals() / operator==.
	 */
	template<typename T>
	[[nodiscard]] static constexpr UE_FORCEINLINE_HINT bool IsDefault(const T& Value)
	{
		using RawType = std::remove_cv_t<std::remove_reference_t<T>>;
        
		if constexpr (std::is_pointer_v<RawType>)
		{
			WARNING("Pointer");
			return Value == nullptr;
		}
		// Any floating type
		if constexpr (std::is_floating_point_v<T>)
		{
			return FMath::IsNearlyEqual(RawType{0}, Value);
		}
		else if constexpr (std::is_same_v<RawType, bool>)
		{
			return Value == false; // default for bool is false
		}
		else if constexpr (std::is_integral_v<RawType> || std::is_enum_v<RawType>)
		{
			return Value == RawType{0};
		}
		else if constexpr (traits::has_IsDefault_method_v<RawType>) 
		{
			WARNING("IsDefault()");
			return Value.IsDefault();
		}
		else if constexpr (traits::has_is_empty_method_v<RawType>)
		{
			WARNING("IsEmpty()");
			return Value.IsEmpty();
		}
		else if constexpr (traits::has_is_null_method_v<RawType>)
		{
			WARNING("IsNull()");
			return Value.IsNull();
		}
		else if constexpr (traits::has_is_valid_method_v<RawType>)
		{
			WARNING("IsValid()");
			return !Value.IsValid();
		}
		else if constexpr (traits::has_num_method_v<RawType>)
		{
			WARNING("Num()");
			return Value.Num() == 0;
		}
		else if constexpr (traits::has_empty_method_v<RawType>)
		{
			WARNING("Empty()");
			return Value.Empty();
		}
		else if constexpr (std::is_same_v<RawType, FMatrix>)
		{
			WARNING("Value.Equals(FMatrix::Identity)");
			return Value.Equals(FMatrix::Identity);
		}
		else if constexpr (std::is_same_v<RawType, FLinearColor>)
		{
			WARNING("Value == FLinearColor::Transparent");
			return Value == FLinearColor::Transparent;
		}
		else if constexpr (traits::has_is_none_method_v<RawType>)
		{
			WARNING("IsNone()");
			return Value.IsNone();
		}
		// else if constexpr (std::is_same_v<RawType, FName>)
		// {
		// 	return Value == FName("None"); // Element.IsNone();
		// }
		// For any type using A.Equals(B) method
		else if constexpr  (traits::has_equals_method_v<T> && std::is_default_constructible_v<RawType>)
		{
			WARNING("Equals()");
			return Value.Equals(T());
		}
		// For using equals operator ( A == B ) // && std::is_default_constructible_v<T>
		else if constexpr (traits::has_equals_operator_v<T> && std::is_default_constructible_v<RawType>)
		{
			WARNING("== T()");
			return Value == T();
		}
		else if constexpr (traits::has_dereference_operator_v<T>)
		{
			WARNING("Dereference()");
			return TTypeTool::IsDefault(*Value);
		}
		else
		{
			ERROR("No Support Type");
			return false;
		}
	}
	
	
	/**
	 * @brief Универсальное преобразование TPair в FString.
	 * @tparam InKeyType   Тип ключа
	 * @tparam InValueType Тип значения
	 * @param  Element     Пара для преобразования
	 * @param  PairSeparator Разделитель между ключом и значением (по умолчанию " / ")
	 * @return FString вида "Key <sep> Value"
	*/
	template <typename InKeyType, typename InValueType>
	[[nodiscard]] static constexpr UE_FORCEINLINE_HINT FString ToString(const TPair<InKeyType, InValueType>& Element, const FString& PairSeparator = FString(TEXT(" / ")))
	{
		return TTypeTool::ToString(Element.Key) + PairSeparator + TTypeTool::ToString(Element.Value);
	};
	
	/**
	 * @brief Универсальное преобразование значения в FString.
	 *
	 * @details Поддерживает:
	 *          - Любой тип с методом ToString()
	 *          - FTransform, FQuat, FVector4, FIntVector4
	 *          - Арифметические типы (float, double, int32, int64, uint8, bool)
	 *
	 * @tparam T Тип значения
	 * @param  A Входное значение
	 * @return FString-представление значения
	*/
	template <typename T>
	[[nodiscard]] static constexpr UE_FORCEINLINE_HINT FString ToString(const T& A) 
	{
		if constexpr		(std::is_same_v<T, FString>)
		{
			return A;
		}
		// AnyType - под любой тип у которого есть метод ToString();
		else if constexpr	(traits::has_to_string_method_v<T>)
		{
			return A.ToString();
		}	
		// Struct
		else if constexpr	(std::is_same_v<T, FTransform>)
		{
			return FString::Printf(TEXT("Translation: %s Rotation: %s Scale: %s"), *A.GetTranslation().ToString(), *A.Rotator().ToString(), *A.GetScale3D().ToString());
		}
		else if constexpr	(std::is_same_v<T, FQuat> || std::is_same_v<T, FVector4>)
		{
			return FString::Printf(TEXT("X=%.3f Y=%.3f Z=%.3f W=%.3f"), A.X, A.Y, A.Z, A.W);
		}
		else if constexpr	(std::is_same_v<T, FIntVector4>)
		{
			return FString::Printf(TEXT("X=%d Y=%d Z=%d W=%d"), A.X, A.Y, A.Z, A.W);
		}
		// ArithmeticType
		else if constexpr	(std::is_floating_point_v<T>)
		{
			return FString::SanitizeFloat(A);
		}
		else if constexpr	(std::is_same_v<T, int32> || std::is_same_v<T, uint8>)
		{
			return FString::Printf(TEXT("%d"), A);
		}
		else if constexpr	(std::is_same_v<T, int64>)
		{
			return FString::Printf(TEXT("%lld"), A);
		}
		else if constexpr	(std::is_same_v<T, bool>)
		{
			return A ? TEXT("true") : TEXT("false");
		}
		// Other
		else
		{
			WARNING("No Support Type");
			return FString();
		};
	}

	/**
	 * @brief Универсальное получение строки с опциональной очисткой дефолтных значений.
	 * @tparam T                 Тип значения
	 * @param  A                 Входное значение
	 * @param  bClearDefaultValues Если true — для дефолтных значений возвращается пустая строка
	 * @return FString-представление значения (или пустая строка, если дефолт и флаг установлен)
	*/
	template <typename T>
	[[nodiscard]] static constexpr UE_FORCEINLINE_HINT FString GetString(const T& A, bool bClearDefaultValues = false){
	
		if ( TTypeTool::IsDefault(A) && bClearDefaultValues)
		{
			// LOG("Default Value changed to empty string")
			return FString();
		}
			
		return TTypeTool::ToString(A);	// под любой тип
	};
	
	/**
	 * @brief Универсальное получение строки из TPair с опциональной очисткой дефолтных значений.
	 * @tparam InKeyType   Тип ключа
	 * @tparam InValueType Тип значения
	 * @param  Element     Пара для преобразования
	 * @param  bClearDefaultValues Если true — пары с дефолтными значениями возвращают пустую строку
	 * @param  PairSeparator Разделитель между ключом и значением (по умолчанию " / ")
	 * @return FString-представление пары (или пустая строка, если дефолт и флаг установлен)
	*/
	template <typename InKeyType, typename InValueType>
	[[nodiscard]] static constexpr UE_FORCEINLINE_HINT FString GetString(const TPair<InKeyType, InValueType>& Element, bool bClearDefaultValues = false, const FString& PairSeparator = FString(TEXT(" / "))){
	
		if ( TTypeTool::IsDefault(Element) && bClearDefaultValues) // если ключ дефолтный - пропускаем! пропуск дефолтных значений - опционально
		{
			// LOG("Default Value change to empty string")
			return FString();
		}
			
		return TTypeTool::ToString(Element, PairSeparator);	// под любой тип
	};
	
	/**
	 * @brief Безопасное деление с проверкой на ноль.
	 * @tparam T Тип делимого и делителя
	 * @param A Делимое
	 * @param B Делитель
	 * @param DefaultValue Значение по умолчанию если B == 0
	 * @return Результат деления или DefaultValue
	 */
	template <typename T>
	[[nodiscard]] static constexpr T SafeDivide(T A, T B, T DefaultValue = T{})
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			return FMath::IsNearlyZero(B) ? DefaultValue : A / B;
		}
		else
		{
			return B == T{} ? DefaultValue : A / B;
		}
	}
	
};

