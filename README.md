# UnrealEssenceTemplateCore

> **Universal Template Core for other plugins — for UnrealEssence series**
>
> Плагин-ядро, предоставляющий набор многократно используемых шаблонных утилит для серии плагинов **UnrealEssence**.

---

## 📌 Описание

**UnrealEssenceTemplateCore** — это фундаментальный плагин, содержащий:

- 🧩 Упрощённую систему логирования с расширенной информацией
- 🔍 Набор C++20 концептов для проверки типов
- 🛡️ Трейты для проверки наличия операторов и методов
- 📏 Расширенные лимиты для числовых и временных типов
- 🔄 Универсальные адаптеры для безопасной конвертации типов

Все компоненты спроектированы для **бесшовной работы с множеством типов** через кастомные шаблонные методы и функции.

---

## 📁 Структура плагина

```
UnrealEssenceTemplateCore/
├── Source/
│   └── UnrealEssenceTemplateCore/
│       ├── Public/
│       │   ├── SimpleLOG.h        # Упрощённая система логирования
│       │   ├── TypeConcepts.h     # C++20 концепты
│       │   ├── TypeTraits.h       # Трейты и алиасы
│       │   ├── TypeLimits.h       # Расширенные лимиты типов
│       │   └── TypeTool.h         # Адаптеры и утилиты
│       └── Private/
│           └── ...
└── UnrealEssenceTemplateCore.uplugin
```

---

## 🚀 Возможности

### 1. SimpleLOG — упрощённая система логирования

Иерархия макросов для удобного вывода сообщений:

| Префикс | Назначение | Категория |
|---------|------------|-----------|
| **E** (ELOG, EWARNING, EERROR) | Расширенный лог с полной информацией (время, поток, функция, строка, файл) | Указывается вручную |
| **S** (SLOG, SWARNING, SERROR) | То же, что E, но в категорию **LogTemp** | LogTemp |
| Без префикса (LOG, WARNING, ERROR) | Упрощённый лог без подробностей | LogTemp |
| **F** постфикс (LOGF, WARNINGF...) | Упрощённый лог **с форматированием** (printf-style) | LogTemp |
| **U** (ULOG, UWARNING, UERROR) | Лог в **динамически созданной уникальной категории** | Уникальная |
| **U+F** (ULOGF, UWARNINGF...) | То же, что U, но с форматированием | Уникальная |

#### Примеры использования

```cpp
// Расширенный лог в кастомную категорию
EWARNING(LogAI, "AI stuck at %s", *TargetName);

// Расширенный лог в LogTemp
SLOG("Detailed message with function info");

// Упрощённый лог
LOG("Simple message");
WARNING("Warning message");

// Упрощённый лог с форматированием
LOGF("Health: %d, Mana: %d", 100, 50);

// Уникальная категория
ULOG(MyUniqueCategory, "Message in unique category");
ULOGF(MyUniqueCategory, "Formatted: %d", 42);
```

---

### 2. TypeConcepts — C++20 концепты

Набор концептов для проверки типов (требует C++20):

```cpp
#include "TypeConcepts.h"

// Проверка: арифметический ИЛИ временной тип
template<typename T> requires concepts::IsArithmeticAndTime<T>
void Process(T value);

// Проверка наличия методов
template<typename T> requires concepts::HasToString<T>
FString Convert(T value) { return value.ToString(); }

// Универсальная проверка дефолтности
bool isDefault = concepts::IsDefaultValue(myValue);
```

**Поддерживаемые концепты:**
- `IsArithmeticAndTime<T>` — арифметический или временной тип
- `HasIsValid<T>`, `HasIsEmpty<T>`, `HasIsDefault<T>` — наличие соответствующих методов
- `HasNum<T>` — наличие метода `Num()` (контейнеры UE)
- `HasEqualsOperator<T>` — наличие `operator==`
- `HasToString<T>` — наличие метода `ToString()`

> ⚠️ Для ранних версий UE (UE4 / ранние UE5) используйте **TypeTraits.h** вместо концептов.

---

### 3. TypeTraits — трейты и алиасы

Трейты для проверки наличия операторов и методов, не требующие C++20:

#### Проверка операторов

```cpp
#include "TypeTraits.h"

static_assert(traits::has_plus_operator_v<int>);          // operator+
static_assert(traits::has_equals_operator_v<FVector>);    // operator==
static_assert(traits::has_subscript_operator_v<TArray<int>>); // operator[]
```

**Поддерживаемые операторы:**
- Арифметические: `+`, `-`, `*`, `/`, `%`
- Присваивания: `=`, `+=`, `-=`, `*=`, `/=`, `%=`
- Инкремент/декремент: `++`, `--` (префикс и постфикс)
- Сравнения: `==`, `!=`, `<`, `<=`, `>`, `>=`
- Логические: `&&`, `||`, `!`
- Битовые: `&`, `|`, `^`, `~`
- Доступ: `[]`, `*`, `->`

#### Проверка методов UE-стиля

```cpp
static_assert(traits::has_to_string_method_v<FVector>);
static_assert(traits::has_get_ticks_method_v<FDateTime>);
static_assert(traits::has_num_method_v<TArray<int>>);
static_assert(traits::has_is_valid_method_v<TWeakObjectPtr<>>);
```

#### Комбинированные алиасы

```cpp
traits::is_comparable_v<T>      // сравним (==, !=, Equals)
traits::is_ordered_v<T>         // упорядочиваем (<, >, Compare)
traits::has_size_v<T>           // имеет размер (Num, Length, GetSize)
traits::is_cleareable_v<T>      // можно очистить (Clear, Reset, Empty)
traits::is_time_type_v<T>       // временной тип (FTimespan, FDateTime, GetTicks)
traits::is_time_arithmetic_v<T> // арифметический ИЛИ временной
```

#### Безопасные типы

```cpp
// Повышение типа для безопасного хранения диапазона
using SafeT = traits::SafeUpType<int32>;        // → int64
using SafeU = traits::SafeUpUnType<int32>;      // → uint32

// Наибольший безопасный тип для двух типов
using GreatestT = traits::GreatestSafeType<int32, float>;  // → double

// Безопасный тип с ограничением роста
using TargetT = traits::TargetSafeType<int64, int32, float>;
```

#### Макросы для генерации собственных трейтов

```cpp
// Генерация трейта для проверки метода без аргументов
DEFINE_HAS_METHOD(GetSize, void);

// Генерация трейта для проверки метода с аргументами
DEFINE_HAS_METHOD_WITH_ARGS(Serialize, Archive&);

// Использование
static_assert(traits::has_GetSize_method_v<MyClass>);
```

---

### 4. TypeLimits — расширенные лимиты типов

Единый интерфейс `TTypeLimits<T>` для получения Min/Max/Lowest значений:

```cpp
#include "TypeLimits.h"

// Для арифметических типов
int32 minInt = TTypeLimits<int32>::Min();
int32 maxInt = TTypeLimits<int32>::Max();

// Для временных типов
FTimespan minTime = TTypeLimits<FTimespan>::Min();
FTimespan maxTime = TTypeLimits<FTimespan>::Max();
int64 minTicks = TTypeLimits<FTimespan>::MinTicks();

// Для FDateTime
FDateTime minDate = TTypeLimits<FDateTime>::Min();
FDateTime maxDate = TTypeLimits<FDateTime>::Max();
```

**Особенности:**
- Поддержка `const`, `volatile`, `const volatile` типов
- Для временных типов — отдельные методы `MinTicks()`, `MaxTicks()`, `LowestTicks()`
- Для арифметических типов — делегирование к стандартному `TNumericLimits`

---

### 5. TypeTool — адаптеры и утилиты

#### TType — безопасная конвертация

```cpp
#include "TypeTool.h"

// Преобразование временного типа в тики
int64 ticks = TType::DataAdapter(myDateTime);

// Восстановление временного типа из тиков
FDateTime restored = TType::DataRepair<FDateTime>(ticks);

// Ограничение значения в безопасном диапазоне
int32 limited = TType::TypeLimits<int32>(myInt64Value);

// Полный цикл безопасного преобразования
FDateTime result = TType::DataTranslator<FDateTime>(myFTimespan);
```

**Методы TType:**
- `DataAdapter(T)` — временной тип → int64 тики
- `DataRepair<TargetType>(InType)` — восстановление в целевой тип
- `TypeLimits<TargetType>(InType)` — ограничение в безопасном диапазоне (возврат в исходном типе)
- `TypeTLim<TargetType>(InType)` — ограничение в безопасном диапазоне (возврат в тиках для временных)
- `DataTranslator<OutType, LimitFromType>(InType)` — полный цикл: адаптация → ограничение → восстановление

#### TTypeTool — универсальные утилиты

```cpp
// Проверка дефолтности
bool isDefault = TTypeTool::IsDefault(myValue);
bool isPairDefault = TTypeTool::IsDefault(myKeyValuePair);

// Универсальное преобразование в строку
FString str = TTypeTool::ToString(myValue);
FString pairStr = TTypeTool::ToString(myKeyValuePair, " : ");

// Преобразование с очисткой дефолтных значений
FString cleanStr = TTypeTool::GetString(myValue, true);  // если дефолт → пустая строка
```

**Поддерживаемые типы для ToString:**
- Любой тип с методом `ToString()`
- `FTransform`, `FQuat`, `FVector4`, `FIntVector4`
- Арифметические типы: `float`, `double`, `int32`, `int64`, `uint8`, `bool`

---

## 🎯 Использование в других плагинах

1. Добавьте зависимость в `.Build.cs`:

```csharp
PublicDependencyModuleNames.AddRange(new string[] {
    "Core",
    "CoreUObject",
    "Engine",
    "UnrealEssenceTemplateCore"  // ← добавить
});
```

2. Подключите необходимые заголовки:

```cpp
#include "SimpleLOG.h"
#include "TypeConcepts.h"
#include "TypeTraits.h"
#include "TypeLimits.h"
#include "TypeTool.h"
```

3. Используйте предоставленные утилиты в своём коде.

---

## 📋 Требования

- **Unreal Engine:** 5.0+ (рекомендуется 5.3+)
- **C++ Standard:** C++17 (минимум), C++20 (для TypeConcepts.h)
- **Платформы:** Windows, Linux, Mac, Console (все стандартные UE)

---

## 🤝 Вклад

Приветствуется! Если у вас есть предложения по улучшению или вы нашли баг — создайте Issue или Pull Request.

---

## 📄 Лицензия

Copyright © UnrealEssence. All Rights Reserved.

---

## 🔗 Связанные проекты

- **UnrealEssence** — основной плагин серии
- **UnrealEssenceRuntime** — runtime-компоненты
- **UnrealEssenceEditor** — редакторские инструменты
- **UnrealEssenceDeveloper** — утилиты для разработчиков

---

## 📞 Контакты

По вопросам использования и сотрудничества обращайтесь к команде UnrealEssence.
