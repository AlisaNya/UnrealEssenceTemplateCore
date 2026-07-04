// Copyright UnrealEssence. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TypeTraits.h"
#include "TypeConcepts.h"
#include "TypeLimits.h"
#include "SimpleLOG.h"

/** 
  * @struct TType
  * @brief Структура Адаптер
  * @details Предоставляющая методы для единообразной бесшовной обработки и безопасной конвертации временных и арифметических типов
  * @version 0.1
  * @todo : шаманить с requires
  * 
  * @warning Поддерживается на любой версии UE
  * - благодаря детекту временных типов через TypeTraits
  * - по наличию метода .GetTicks() позволяя расширить поддержку на кастомные временные типы без вмешательства в код
  */
struct TType
{
	/** 
	  * @brief TimeType -> ArithmeticType (int64 Ticks), ignored OtherType!
	  * 
	  * @details Адаптер Типа
	  * - преобразует временные типы в int64 Ticks
	  * - служит как прослойка для упрощения работы с временными типами арифметическим путем
	  * - используется перед сложной арифметической обработкой данных
	  * - Все типы кроме временных возвращает как есть!
	  * 
	  * @version 0.1
	  * @todo : шаманить с requires
	  * 
	  * @warning Детектит временные типы (FTimespan / FDateTime) и другие - по наличию метода .GetTicks(); определяя который вы можете добавить поддержку ваших типов/структур.
	  * 
	  * @param A inValue
	  * @tparam T InType
	  * 
	  * @example auto CalcData = TType::DataAdapter(InData)
	  * @return InType or int64 Ticks
	  * 
	  * @note Для обратного автоматизированного преобразования используйте TType::DataRepair<SourceType>(Result)
	  * @see TType::DataRepair
	  */
	template <typename T>
	[[nodiscard]] static constexpr auto DataAdapter(T A) { if constexpr (traits::is_time_type_v<T>) { return A.GetTicks(); } else { return A; }}
	
	
	/** 
	  * @brief InArithmeticType -> TargetArithmeticType, Time in Arithmetic -> SourceTimeType;
	  * 
	  * @details Адаптер Типа
	  * - восстанавливает временные типы в исходный тип
	  * - прочие типы кастит в целевой
	  * - служит как прослойка для возврата результата арифметических операций с временными типами в исходный вид
	  * - используется после арифметической обработки данных
	  * 
	  * @version 0.1
	  * @todo : шаманить с requires
	  * @warning Автоматизируемые действия;
	  * - к НЕ временным типам применяет каст к целевому!
	  * - к временным типам применяет конструктор через тики
	  * 
	  * @param A inValue
	  * @tparam TargetType Целевой/Исходный тип
	  * @tparam InType InType (ArithmeticType or TimeType in int64 Ticks)
	  * 
	  * @example return TType::DataRepair<SourceType>(ResultData)
	  * @return For TargetType == TimeType and InType == int64 -> return TimeType == TargetType, else return InType -> 
	  * 
	  * @note Для прямого автоматизированного преобразования используйте TType::DataAdapter(InValue)
	  * @see TType::DataRepair
	  */
	template <typename TargetType, typename InType>
	requires (std::is_convertible_v<InType, TargetType> || (traits::is_time_type_v<TargetType> && std::is_same_v<int64, InType>)) // или InType должен быть int64 Ticks && TargetType должен быть временным типом, или InType должен быть способен каститься в TargetType
	[[nodiscard]] static constexpr auto DataRepair(InType A) 
	{ 
		if constexpr (traits::is_time_type_v<TargetType> && std::is_same_v<int64, InType>) { return TargetType(A); } 
		else if constexpr (! traits::is_time_type_v<TargetType>) { return (TargetType)A; }
		else
		{
			ERROR("No Support Type - Cast InType to TargetType is Fail. returns - value source type");
			return A;
		}
	}
	
	
	/** 
	  * @brief SourceType -> LimitedValue in SourceType
	  * @details Ограничивает значение текущего типа под допустимые значения целевого типа для безопасных преобразований
	  * 
	  * @version 0.1
	  * @todo : шаманить с requires
	  * @warning Возвращает результат временных типов В ИСХОДНОМ ТИПЕ!
	  * 
	  * @param A inValue
	  * @tparam TargetType Целевой тип
	  * @tparam InType InType
	  * 
	  * @example TType::TypeLimits<FTimespan>(Int64 Ticks)			// вернет  int64 тики ограниченные безопасным значением для FTimespan
	  * @example TType::TypeLimits<uint8>(Int32 value)				// вернет допустимое безопасное значение
	  * @example TType::TypeLimits<FDateTime>(FTimespan)			// вернет FTimespan с ограниченным допустимым безопасным для преобразования в FDateTime значением (поведение отличается от TypeTLim)
	  * @return InType Limited from safe TargetType values
	  * 
	  * @note Если необходимо ограничить значение временных типов и при этом вернуть его в виде тиков используйте - TType::TypeTLim<TargetType>(inValue);
	  * @see TypeTLim
	  */
	template <typename TargetType, typename InType>
	requires (traits::is_time_arithmetic_v<TargetType>)
	[[nodiscard]] static constexpr InType TypeLimits(InType A)
	{
		if constexpr (traits::is_time_type_v<InType>) { return TType::DataRepair<InType>(FMath::Clamp(TType::DataAdapter(A), TTypeLimits<TargetType>::MinTicks() , TTypeLimits<TargetType>::MaxTicks())); }
		else if constexpr (std::is_arithmetic_v<InType>) return FMath::Clamp(TType::DataAdapter(A), (InType)TTypeLimits<TargetType>::MinTicks() , (InType)TTypeLimits<TargetType>::MaxTicks());
		else 
		{
			WARNING("No Support Type - No Arithmetic, No TimeType - returns = source value");
			return A;
		}
	};
	
	/** 
	  * @brief SourceType -> LimitedValue in SourceType or SourceTimeType -> LimitedValue in Ticks - int64 Type
	  * @details Ограничивает значение текущего типа под допустимые значения целевого типа для безопасных преобразований
	  * 
	  * @version 0.1
	  * @todo : шаманить с requires
	  * @warning Возвращает результат временных типов В ВИДЕ ТИКОВ!
	  * 
	  * @param A inValue
	  * @tparam TargetType Целевой тип
	  * @tparam InType InType
	  * 
	  * @example TType::TypeLimits<FTimespan>(Int64 Ticks)			// вернет  int64 тики ограниченные безопасным значением для FTimespan
	  * @example TType::TypeLimits<uint8>(Int32 value)				// вернет допустимое безопасное значение
	  * @example TType::TypeLimits<FDateTime>(FTimespan)			// вернет int64 Ticks с ограниченным допустимым безопасным для преобразования в FDateTime значением (поведение отличается от TypeLimits)
	  * @return InType or int64 Ticks (for only TimeTypes) Limited from safe TargetType values 
	  * 
	  * @note Если необходимо ограничить значение временных типов и при этом вернуть его в ИСХОДНОМ виде, то используйте - TType::TypeLimits<TargetType>(inValue);
	  * @see TypeLimits
	  */
	template <typename TargetType, typename InType>
	requires (traits::is_time_arithmetic_v<TargetType>)
	[[nodiscard]] static constexpr auto TypeTLim(InType A)
	{
		if constexpr (traits::is_time_type_v<InType>) { return FMath::Clamp(TType::DataAdapter(A), TTypeLimits<TargetType>::MinTicks() , TTypeLimits<TargetType>::MaxTicks()); }
		else if constexpr (std::is_arithmetic_v<InType>) return FMath::Clamp(TType::DataAdapter(A), (InType)TTypeLimits<TargetType>::MinTicks() , (InType)TTypeLimits<TargetType>::MaxTicks());
		else 
		{
			WARNING("No Support Type - No Arithmetic, No TimeType - returns = source value");
			return A;
		}
	};
	
	/** 
	  * @brief Safe Data Translator
	  * 
	  * @details Преобразует один тип в другой производя необходимые ограничения для безопасного преобразования под результирующий тип или другой целевой
	  * 
	  * @version 0.1
	  * @todo : шаманить с requires
	  * @warning Если возвращаемый тип отличается от целевого - то значение будет ограничено под целевой тип, а затем под возвращаемый тип для безопасности на случай если он меньше чем целевой
	  * 
	  * @param A inValue
	  * @tparam OutType OutType and Target for out value Limits (for LimitFromType == OutType -> ignored OutLimits, use only proxy value Limits)
	  * @tparam LimitFromType Target for proxy value Limits (default LimitFromType == OutType)
	  * @tparam InType InType
	  * 
	  * @example TType::DataTranslator<FDateTime>(FTimespan A)				// ограничит тики Timespan и безопасно преобразует в DateTime
	  * @example TType::DataTranslator<FDateTime>(int64 Ticks)				// ограничит тики и безопасно преобразует в DateTime
	  * @example TType::DataTranslator<int64,FDateTime>(FTimespan A)		// ограничит тики на безопасном для FDateTime уровне и вернет как тики
	  * @example TType::DataTranslator<FTimespan,FDateTime>(FTimespan A)	// ограничит тики Timespan на безопасном для FDateTime уровне и вернет как Timespan
	  * @example TType::DataTranslator<FTimespan,uint8>(int64 value)		// ограничит int64 безопасным для uint8 значением (тип прежний), ограничится безопасным значением для FTimespan(тип прежний), преобразуется в FTimespan
	  * 
	  * @return limited value in OutType
	  * 
	  * @note Производит полный цикл безопасного преобразования данных (типы преобразует в тики -> ограничивает значение под целевой тип -> выводит результат в целевом типе либо другом возвращаемом типе )
	  * @see TType::DataAdapter(InValue), TType::TypeTLim<LimitFromType>(InValue), TType::DataRepair<OutType>(InValue)
	  */
	template <typename OutType, typename LimitFromType = OutType, typename InType>
	requires (((std::is_convertible_v<InType, OutType> && !traits::is_time_type_v<InType>) || traits::is_time_type_v<InType>) && traits::is_time_arithmetic_v<LimitFromType>)
	[[nodiscard]] static constexpr OutType DataTranslator(InType A)
	{
		if constexpr (std::is_same_v<LimitFromType, OutType>) { return TType::DataRepair<OutType>(TType::TypeTLim<LimitFromType>(TType::DataAdapter(A))); } // идем по упрощенной цепочке если LimitFromType = OutType
		else { return TType::DataRepair<OutType>(TType::TypeTLim<OutType>(TType::TypeTLim<LimitFromType>(TType::DataAdapter(A)))); };
	};
};

/** 
  * @struct TType
  * @brief Структура Адаптер
  * @details Предоставляющая методы для единообразной бесшовной обработки и безопасной конвертации временных и арифметических типов
  * @version 0.1
  * @todo : шаманить с requires
  * 
  * @warning Поддерживается на любой версии UE
  * - благодаря детекту временных типов через TypeTraits
  * - по наличию метода .GetTicks() позволяя расширить поддержку на кастомные временные типы без вмешательства в код
  */
struct TTypeTool
{
	
	/**
	 * DefaultValueDetector
	 * @tparam InKeyType 
	 * @tparam InValueType 
	 * @param Element 
	 * @warning True - если ключ или значение являются значением типа по умолчанию
	 * @return 
	 */
	template <typename InKeyType, typename InValueType>
	[[nodiscard]] static constexpr bool IsDefault(const TPair<InKeyType, InValueType>& Element)
	{
		return TTypeTool::IsDefault(Element.Key) || TTypeTool::IsDefault(Element.Value);
	};
	
	/**
	 * DefaultValueDetector
	 * @tparam T 
	 * @param Element 
	 * @return 
	 */
	template <typename T>
	[[nodiscard]] static constexpr bool IsDefault(const T& Element)
	{
		// Any floating type
		if constexpr (std::is_floating_point_v<T>) { return FMath::IsNearlyEqual(T(), Element); }
		// For any type using A.Equals(B) method
		else if constexpr  (traits::has_equals_method_v<T>) { return T().Equals(Element); }
		// For using equals operator ( A == B ) // && std::is_default_constructible_v<T>
		else if constexpr (traits::has_equals_operator_v<T>) { return T() == Element || T{} == Element; } // для любых типов с оператором == 
		else
		{
			WARNING("No Support Type");
			return false; // тип не поддерживается (тк нет операторов сравнения и методов аналогов)
		} 
	};
	
	
	
	/**
	 * Универсальное преобразование к строке
	 * @tparam InKeyType 
	 * @tparam InValueType 
	 * @param Element 
	 * @return 
	*/
	template <typename InKeyType, typename InValueType>
	[[nodiscard]] static constexpr UE_FORCEINLINE_HINT FString ToString(const TPair<InKeyType, InValueType>& Element, const FString& PairSeparator = FString(TEXT(" / ")))
	{
		return TTypeTool::ToString(Element.Key) + PairSeparator + TTypeTool::ToString(Element.Value);
	};
	
	/**
	 * @brief Универсальное преобразование к строке
	 * @tparam T inType
	 * @param A inValue
	 * @warning Поддерживаются арифметические типы, трансформ и любые типы с методом .ToString()
	 * @return FString result
	 */
	template <typename T>
	[[nodiscard]] static constexpr UE_FORCEINLINE_HINT FString ToString(const T& A) 
	{
		// AnyType - под любой тип у которого есть метод ToString();
		if constexpr		(traits::has_to_string_method_v<T>) 
			{ return A.ToString();}		
		// Struct
		else if constexpr	(std::is_same_v<T, FTransform>)		
			{ return FString::Printf(TEXT("Translation: %s Rotation: %s Scale: %s"), *A.GetTranslation().ToString(), *A.Rotator().ToString(), *A.GetScale3D().ToString());}
		else if constexpr	(std::is_same_v<T, FQuat> || std::is_same_v<T, FVector4>)
			{ return FString::Printf(TEXT("X=%.3f Y=%.3f Z=%.3f W=%.3f"), A.X, A.Y, A.Z, A.W);}
		else if constexpr	(std::is_same_v<T, FIntVector4>) 
			{ return FString::Printf(TEXT("X=%d Y=%d Z=%d W=%d"), A.X, A.Y, A.Z, A.W);}
		// ArithmeticType
		else if constexpr	(std::is_floating_point_v<T>)		
			{ return FString::SanitizeFloat(A); }
		else if constexpr	(std::is_same_v<T, int32> || std::is_same_v<T, uint8>) 
			{ return FString::Printf(TEXT("%d"), A);	}
		else if constexpr	(std::is_same_v<T, int64>)			
			{ return FString::Printf(TEXT("%lld"), A); }
		else if constexpr	(std::is_same_v<T, bool>)			
			{ return A ? TEXT("true") : TEXT("false"); }
		// Other
		else
		{
			WARNING("No Support Type");
			return FString();
		};
	}

	/**
	 * @brief Универсальное получение строки из любого типа с очисткой дефолтных значений
	 * @tparam T InType
	 * @param A InValue
	 * @param bClearDefaultValues Заменять значения по умолчанию на пустую строку?
	 * @return FString result
	 */
	template <typename T>
	[[nodiscard]] static constexpr FString GetString(const T& A, bool bClearDefaultValues = false){
	
		if ( TTypeTool::IsDefault(A) && bClearDefaultValues)
		{
			// LOG("Default Value change to empty string")
			return FString();
		}
			
		return TTypeTool::ToString(A);	// под любой тип
	};
	
	/**
	 * @brief Универсальное получение строки из любого типа с очисткой дефолтных значений
	 * @tparam InKeyType 
	 * @tparam InValueType 
	 * @param Element 
	 * @param PairSeparator Разделитель между парой ключ/начение
	 * @param bClearDefaultValues Заменять значения по умолчанию на пустую строку?
	 * @warning 
	 * - Пары с Дефолтными значениями пропускаются опционально в зависимости от флага bClearDefaultValues
	 * @return FString result
	 */
	template <typename InKeyType, typename InValueType>
	[[nodiscard]] static constexpr FString GetString(const TPair<InKeyType, InValueType>& Element, bool bClearDefaultValues = false, const FString& PairSeparator = FString(TEXT(" / "))){
	
		if ( TTypeTool::IsDefault(Element) && bClearDefaultValues) // если ключ дефолтный - пропускаем! пропуск дефолтных значений - опционально
		{
			// LOG("Default Value change to empty string")
			return FString();
		}
			
		return TTypeTool::ToString(Element, PairSeparator);	// под любой тип
	};
	
	
	
	
};