// Copyright UnrealEssence. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * @file SimpleLOG.h
 * @brief Упрощённая система логирования для Unreal Engine.
 *
 * @details Предоставляет иерархию макросов для удобного вывода сообщений в лог:
 *
 * @section hierarchy Иерархия макросов:
 *
 * @subsection extended E-префикс (ELOG, EWARNING, EERROR) — РАСШИРЕННЫЙ лог
 * - Требуют указания КАСТОМНОЙ категории (LogCat)
 * - Автоматически добавляют: время, ID потока, имя функции, номер строки, имя файла
 * - Используются для детальной отладки с полной информацией о контексте
 *
 * @subsection short S-префикс (SLOG, SWARNING, SERROR) — РАСШИРЕННЫЙ лог в LogTemp
 * - То же самое что E-лог, но с дефолтной категорией LogTemp
 * - Не нужно указывать категорию
 *
 * @subsection simple Без префиксов (LOG, WARNING, ERROR, DISPLAY, MESSAGE, FATAL) — УПРОЩЁННЫЙ лог
 * - Минимальный синтаксис без подробностей (только сообщение)
 * - Категория LogTemp по умолчанию
 * - Используют макрос LOGS, который убирает необходимость TEXT()
 *
 * @subsection format F-постфикс (LOGF, WARNINGF, ERRORF...) — УПРОЩЁННЫЙ лог С ФОРМАТИРОВАНИЕМ
 * - То же что и без префиксов, но с поддержкой аргументов (printf-style)
 * - Используют макрос LOGS_ARGS
 *
 * @subsection unique U-префикс (ULOG, UWARNING, UERROR) — УНИКАЛЬНАЯ категория
 * - Динамически создают уникальную категорию для конкретного сообщения
 * - Используются когда нужна отдельная категория под конкретное сообщение
 *
 * @subsection unique_format U+F (ULOGF, UWARNINGF...) — УНИКАЛЬНАЯ категория С ФОРМАТИРОВАНИЕМ
 * - То же что U-префикс, но с поддержкой аргументов
 *
 * @version 1.0
 * @note Все макросы не требуют оборачивать строку в TEXT() — это делается автоматически.
 *
 * @example
 * @code
 *   // Расширенный лог в кастомную категорию
 *   EWARNING(LogAI, "AI stuck at %s", *TargetName);
 *
 *   // Расширенный лог в LogTemp
 *   SLOG("Detailed message with function info");
 *
 *   // Упрощённый лог
 *   LOG("Simple message");
 *   WARNING("Warning message");
 *
 *   // Упрощённый лог с форматированием
 *   LOGF("Health: %d, Mana: %d", 100, 50);
 *
 *   // Уникальная категория
 *   ULOG(MyUniqueCategory, "Message in unique category");
 *   ULOGF(MyUniqueCategory, "Formatted: %d", 42);
 * @endcode
*/

// ============================================================================
// Кастомные категории логирования
// ============================================================================

/** @brief Категория для игровой логики (Gameplay). */
DECLARE_LOG_CATEGORY_EXTERN(LogGame, Log, All);

/** @brief Категория для логики ИИ. */
DECLARE_LOG_CATEGORY_EXTERN(LogAI, Log, All);

/** @brief Категория для UI/UMG. */
DECLARE_LOG_CATEGORY_EXTERN(LogUI, Log, All);

/** @brief Категория для сетевого взаимодействия. */
DECLARE_LOG_CATEGORY_EXTERN(LogNetwork, Log, All);

/** @brief Категория конкретно для плагина UnrealEssence. Требует DEFINE_LOG_CATEGORY в .cpp. */
DECLARE_LOG_CATEGORY_EXTERN(LogUnrealEssencePlugin, Log, All);
// DEFINE_LOG_CATEGORY(LogUnrealEssencePlugin); // раскомментировать в .cpp


// ============================================================================
// E-префикс: ELOG / EWARNING / EERROR — РАСШИРЕННЫЙ лог в КАСТОМНУЮ категорию
// ============================================================================

/**
 * @def ELOG(LogCat, Format, ...)
 * @brief РАСШИРЕННЫЙ Log-уровень в указанную КАСТОМНУЮ категорию.
 * @details Автоматически добавляет имя функции и номер строки.
 * @param LogCat  Категория лога (например, LogAI, LogGame)
 * @param Format  Форматированная строка (без TEXT())
 * @param ...     Аргументы форматирования
*/
#define ELOG(LogCat, Format, ...) \
UE_LOG(LogCat, Log, TEXT("%s(%d): ") Format , \
ANSI_TO_TCHAR(__FUNCTION__), __LINE__, ##__VA_ARGS__ )

/**
 * @def EWARNING(LogCat, Format, ...)
 * @brief РАСШИРЕННЫЙ Warning-уровень в указанную КАСТОМНУЮ категорию.
 * @details Добавляет время, ID потока, имя функции и номер строки.
 * @param LogCat  Категория лога
 * @param Format  Форматированная строка (без TEXT())
 * @param ...     Аргументы форматирования
*/
#define EWARNING(LogCat, Format, ...) \
UE_LOG(LogCat, Warning, TEXT("[%s] [Thread: %d] %s(%d): ") Format , \
*FDateTime::Now().ToString(TEXT("%H:%M:%S.%s")), \
FPlatformTLS::GetCurrentThreadId(), \
ANSI_TO_TCHAR(__FUNCTION__), __LINE__, ##__VA_ARGS__ )

/**
 * @def EERROR(LogCat, Format, ...)
 * @brief РАСШИРЕННЫЙ Error-уровень в указанную КАСТОМНУЮ категорию.
 * @details Добавляет время, ID потока, функцию, строку и имя файла.
 * @note Имя файла выводится отдельной строкой для читаемости.
 * @param LogCat  Категория лога
 * @param Format  Форматированная строка (без TEXT())
 * @param ...     Аргументы форматирования
*/
#define EERROR(LogCat, Format, ...) \
UE_LOG(LogCat, Error, TEXT("[%s] [Thread: %d] %s %s(%d): ") Format TEXT("\n%s"), \
*FDateTime::Now().ToString(TEXT("%H:%M:%S.%s")), \
FPlatformTLS::GetCurrentThreadId(), \
ANSI_TO_TCHAR(__FUNCTION__), __LINE__, ##__VA_ARGS__ , \
ANSI_TO_TCHAR(__FILE__))

/**
 * @def ENSERROR(LogCat, Format, ...)
 * @brief РАСШИРЕННЫЙ Error-уровень в указанную КАСТОМНУЮ категорию.
 * @details Добавляет время, ID потока, имя объекта (this), функцию, строку и имя файла.
 * @note Имя файла выводится отдельной строкой для читаемости.
 * @param LogCat  Категория лога
 * @param Format  Форматированная строка (без TEXT())
 * @param ...     Аргументы форматирования
 * @warning Для использования ТОЛЬКО в статических методах (из-за *GetNameSafe(this))
*/
#define ENSERROR(LogCat, Format, ...) \
UE_LOG(LogCat, Error, TEXT("[%s] [Thread: %d] %s %s(%d): ") Format TEXT("\n%s"), \
*FDateTime::Now().ToString(TEXT("%H:%M:%S.%s")), \
FPlatformTLS::GetCurrentThreadId(), \
*GetNameSafe(this), \
ANSI_TO_TCHAR(__FUNCTION__), __LINE__, ##__VA_ARGS__ , \
ANSI_TO_TCHAR(__FILE__))

// ============================================================================
// S-префикс: SLOG / SWARNING / SERROR — РАСШИРЕННЫЙ лог в LogTemp (дефолтная)
// ============================================================================

/**
 * @def SLOG(Format, ...)
 * @brief РАСШИРЕННЫЙ Log-уровень в категорию LogTemp.
 * @details То же что ELOG, но не требует указания категории (использует LogTemp).
 * @param Format  Форматированная строка (без TEXT())
 * @param ...     Аргументы форматирования
 */
#define SLOG(Format, ...)		ELOG(LogTemp, Format, ##__VA_ARGS__ )

/**
 * @def SWARNING(Format, ...)
 * @brief РАСШИРЕННЫЙ Warning-уровень в категорию LogTemp.
 * @details То же что EWARNING, но не требует указания категории.
 */
#define SWARNING(Format, ...)	EWARNING(LogTemp, Format, ##__VA_ARGS__ )

/**
 * @def SERROR(Format, ...)
 * @brief РАСШИРЕННЫЙ Error-уровень в категорию LogTemp.
 * @details То же что EERROR, но не требует указания категории.
 */
#define SERROR(Format, ...)		EERROR(LogTemp, Format, ##__VA_ARGS__ )


// ============================================================================
// Без префиксов: LOG / DISPLAY / WARNING / ERROR / FATAL — УПРОЩЁННЫЙ лог в LogTemp
// ============================================================================

/**
 * @def LOGS(cat, lvl, msg)
 * @brief Базовая обёртка над UE_LOG. Автоматически оборачивает msg в TEXT().
 * @param cat  Категория лога
 * @param lvl  Уровень лога (Log, Warning, Error...)
 * @param msg  Сообщение (без TEXT())
 */
#define LOGS(cat, lvl, msg) UE_LOG(cat, lvl, TEXT(msg))	// LOGS базовая обертка (не требует заключать сообщение в TEXT(format) )

/**
 * @def LOG(msg)
 * @brief УПРОЩЁННЫЙ Log-уровень (серый текст) в LogTemp.
 * @param msg  Простое сообщение без форматирования (без TEXT())
 */
#define LOG(msg) LOGS(LogTemp, Log, msg)

/**
 * @def DISPLAY(msg)
 * @brief УПРОЩЁННЫЙ Display-уровень (белый текст) в LogTemp.
 * @param msg  Простое сообщение без форматирования
 */
#define DISPLAY(msg) LOGS(LogTemp, Display, msg)

/**
 * @def MESSAGE(msg)
 * @brief УПРОЩЁННЫЙ Display-уровень (белый текст) в LogTemp. Интуитивный алиас DISPLAY.
 * @param msg  Простое сообщение без форматирования
 */
#define MESSAGE(msg) LOGS(LogTemp, Display, msg)

/**
 * @def WARNING(msg)
 * @brief УПРОЩЁННЫЙ Warning-уровень (жёлтый текст) в LogTemp.
 * @param msg  Простое сообщение без форматирования
 */
#define WARNING(msg) LOGS(LogTemp, Warning, msg)

/**
 * @def ERROR(msg)
 * @brief УПРОЩЁННЫЙ Error-уровень (красный текст) в LogTemp.
 * @param msg  Простое сообщение без форматирования
 */
#define ERROR(msg) LOGS(LogTemp, Error, msg)

/**
 * @def FATAL(msg)
 * @brief УПРОЩЁННЫЙ Fatal-уровень (красный текст) в LogTemp. 
 * @warning Аварийное завершение программы. Гарантированно приводит к крашу движка! (стандартное поведение в UE для логирования FATAL - фатальных ошибок с аварийным завершением выполнения)
 * @param msg  Простое сообщение без форматирования
 */
#define FATAL(msg) LOGS(LogTemp, Fatal, msg)

#define VERBOSE(msg) LOGS(LogTemp, Verbose, msg)

#define VERYVERBOSE(msg) LOGS(LogTemp, VeryVerbose, msg)


/**
 * @def LOG_NET(msg)
 * @brief УПРОЩЁННЫЙ Log-уровень в категории LogNet (сетевые события).
 * @param msg  Простое сообщение без форматирования
 */
#define LOG_NET(msg) LOGS(LogNet, Log, msg)

/**
 * @def LOG_LOAD(msg)
 * @brief УПРОЩЁННЫЙ Log-уровень в категории LogLoad (события загрузки).
 * @param msg  Простое сообщение без форматирования
 */
#define LOG_LOAD(msg) LOGS(LogLoad, Log, msg)






// ============================================================================
// F-постфикс: LOGF / WARNINGF / ERRORF — УПРОЩЁННЫЙ лог С ФОРМАТИРОВАНИЕМ
// ============================================================================

/**
 * @def LOGS_ARGS(cat, lvl, format, ...)
 * @brief Базовая обёртка над UE_LOG с поддержкой аргументов форматирования.
 * @param cat     Категория лога
 * @param lvl     Уровень лога
 * @param format  Форматированная строка (без TEXT())
 * @param ...     Аргументы форматирования
 */
#define LOGS_ARGS(cat, lvl, format, ...) UE_LOG(cat, lvl, TEXT(format), ##__VA_ARGS__)	// LOGS базовая обертка (не требует заключать сообщение в TEXT(format) ) + ARGS (с форматированием)

/**
 * @def LOGF(format, ...)
 * @brief УПРОЩЁННЫЙ Log-уровень С ФОРМАТИРОВАНИЕМ в LogTemp.
 * @param format  Форматированная строка (printf-style, без TEXT())
 * @param ...     Аргументы форматирования
 */
#define LOGF(format, ...) LOGS_ARGS(LogTemp, Log, format, ##__VA_ARGS__)

/**
 * @def DISPLAYF(format, ...)
 * @brief УПРОЩЁННЫЙ Display-уровень С ФОРМАТИРОВАНИЕМ в LogTemp.
 */
#define DISPLAYF(format, ...) LOGS_ARGS(LogTemp, Display, format, ##__VA_ARGS__)

/**
 * @def MESSAGEF(format, ...)
 * @brief УПРОЩЁННЫЙ Display-уровень С ФОРМАТИРОВАНИЕМ в LogTemp. Алиас DISPLAYF.
 */
#define MESSAGEF(format, ...) LOGS_ARGS(LogTemp, Display, format, ##__VA_ARGS__)

/**
 * @def WARNINGF(format, ...)
 * @brief УПРОЩЁННЫЙ Warning-уровень С ФОРМАТИРОВАНИЕМ в LogTemp.
 */
#define WARNINGF(format, ...) LOGS_ARGS(LogTemp, Warning, format, ##__VA_ARGS__)

/**
 * @def ERRORF(format, ...)
 * @brief УПРОЩЁННЫЙ Error-уровень С ФОРМАТИРОВАНИЕМ в LogTemp.
 */
#define ERRORF(format, ...) LOGS_ARGS(LogTemp, Error, format, ##__VA_ARGS__)

/**
 * @def FATALF(format, ...)
 * @brief УПРОЩЁННЫЙ Fatal-уровень С ФОРМАТИРОВАНИЕМ в LogTemp. 
 * @warning Аварийное завершение программы. Гарантированно приводит к крашу движка! (стандартное поведение в UE для логирования FATAL - фатальных ошибок с аварийным завершением выполнения)
 */
#define FATALF(format, ...) LOGS_ARGS(LogTemp, Fatal, format, ##__VA_ARGS__)

#define VERBOSEF(format, ...) LOGS_ARGS(LogTemp, Verbose, format, ##__VA_ARGS__)

#define VERYVERBOSEF(format, ...) LOGS_ARGS(LogTemp, VeryVerbose, format, ##__VA_ARGS__)

// ============================================================================
// U-префикс: ULOG / UWARNING / UERROR — УНИКАЛЬНАЯ динамическая категория
// ============================================================================

/**
 * @def LOGS_UNIQUE_CATHEGORY(NewCategoryName, Verbosity, Format)
 * @brief Базовая обёртка: создаёт статическую категорию лога и пишет в неё.
 * @note Категория определяется через DEFINE_LOG_CATEGORY_STATIC.
 * @param NewCategoryName  Имя новой категории
 * @param Verbosity        Уровень лога
 * @param Format           Сообщение (без TEXT())
 */
#define LOGS_UNIQUE_CATHEGORY(NewCategoryName, Verbosity, Format) \
do { \
DEFINE_LOG_CATEGORY_STATIC(NewCategoryName, Log, All); \
UE_LOG(NewCategoryName, Verbosity, TEXT(Format)); \
} while(0)

/**
 * @def ULOG(NewCategoryName, msg)
 * @brief Log-уровень в УНИКАЛЬНОЙ динамически созданной категории.
 * @param NewCategoryName  Имя новой категории (создаётся автоматически)
 * @param msg              Простое сообщение без форматирования
 */
#define ULOG(NewCategoryName, msg) LOGS_UNIQUE_CATHEGORY(NewCategoryName, Log, msg)

/**
 * @def UDISPLAY(NewCategoryName, msg)
 * @brief Display-уровень в УНИКАЛЬНОЙ динамически созданной категории.
 */
#define UDISPLAY(NewCategoryName, msg) LOGS_UNIQUE_CATHEGORY(NewCategoryName, Display, msg)

/**
 * @def UMESSAGE(NewCategoryName, msg)
 * @brief Display-уровень в УНИКАЛЬНОЙ динамически созданной категории. Алиас UDISPLAY.
 */
#define UMESSAGE(NewCategoryName, msg) LOGS_UNIQUE_CATHEGORY(NewCategoryName, Display, msg)

/**
 * @def UWARNING(NewCategoryName, msg)
 * @brief Warning-уровень в УНИКАЛЬНОЙ динамически созданной категории.
 */
#define UWARNING(NewCategoryName, msg) LOGS_UNIQUE_CATHEGORY(NewCategoryName, Warning, msg)

/**
 * @def UERROR(NewCategoryName, msg)
 * @brief Error-уровень в УНИКАЛЬНОЙ динамически созданной категории.
 */
#define UERROR(NewCategoryName, msg) LOGS_UNIQUE_CATHEGORY(NewCategoryName, Error, msg)


/**
 * @def LOGS_UNIQUE_CATHEGORY_ARGS(NewCategoryName, Verbosity, Format, ...)
 * @brief Базовая обёртка с форматированием для уникальной категории.
 */
#define LOGS_UNIQUE_CATHEGORY_ARGS(NewCategoryName, Verbosity, Format, ...) \
do { \
DEFINE_LOG_CATEGORY_STATIC(NewCategoryName, Log, All); \
UE_LOG(NewCategoryName, Verbosity, TEXT(Format), ##__VA_ARGS__); \
} while(0)

/**
 * @def ULOGF(NewCategoryName, format, ...)
 * @brief Log-уровень С ФОРМАТИРОВАНИЕМ в УНИКАЛЬНОЙ динамически созданной категории.
 */
#define ULOGF(NewCategoryName, format, ...) LOGS_UNIQUE_CATHEGORY_ARGS(NewCategoryName, Log, format, ##__VA_ARGS__)

/**
 * @def UDISPLAYF(NewCategoryName, format, ...)
 * @brief Display-уровень С ФОРМАТИРОВАНИЕМ в УНИКАЛЬНОЙ динамически созданной категории.
 */
#define UDISPLAYF(NewCategoryName, format, ...) LOGS_UNIQUE_CATHEGORY_ARGS(NewCategoryName, Display, format, ##__VA_ARGS__)

/**
 * @def UMESSAGEF(NewCategoryName, format, ...)
 * @brief Display-уровень С ФОРМАТИРОВАНИЕМ в УНИКАЛЬНОЙ динамически созданной категории. Алиас UDISPLAYF.
 */
#define UMESSAGEF(NewCategoryName, format, ...) LOGS_UNIQUE_CATHEGORY_ARGS(NewCategoryName, Display, format, ##__VA_ARGS__)

/**
 * @def UWARNINGF(NewCategoryName, format, ...)
 * @brief Warning-уровень С ФОРМАТИРОВАНИЕМ в УНИКАЛЬНОЙ динамически созданной категории.
 */
#define UWARNINGF(NewCategoryName, format, ...) LOGS_UNIQUE_CATHEGORY_ARGS(NewCategoryName, Warning, format, ##__VA_ARGS__)

/**
 * @def UERRORF(NewCategoryName, format, ...)
 * @brief Error-уровень С ФОРМАТИРОВАНИЕМ в УНИКАЛЬНОЙ динамически созданной категории.
 */
#define UERRORF(NewCategoryName, format, ...) LOGS_UNIQUE_CATHEGORY_ARGS(NewCategoryName, Error, format, ##__VA_ARGS__)

// Использование

/**
void Test()
{
	LOG("Simple message");
	LOGF("Health: %d, Mana: %d", 100, 50);
	ULOG(MyUniqueCategory, "Mana");
 EERROR("Health Low %f", GetHealth());
}
*/
