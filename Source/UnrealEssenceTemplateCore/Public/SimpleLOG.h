// Copyright UnrealEssence. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// Кастомные категории логирования

DECLARE_LOG_CATEGORY_EXTERN(LogGame, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogAI, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogUI, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogNetwork, Log, All);

DECLARE_LOG_CATEGORY_EXTERN(LogUnrealEssencePlugin, Log, All); // лог для конкретно данного плагина в .h
// DEFINE_LOG_CATEGORY(LogUnrealEssencePlugin); // для .cpp

// -- SimpleLOG for Custom Category -- //
#define ELOG(LogCat, Format, ...) \
UE_LOG(LogCat, Log, TEXT("%s(%d): ") Format , \
ANSI_TO_TCHAR(__FUNCTION__), __LINE__, ##__VA_ARGS__ )

#define EWARNING(LogCat, Format, ...) \
UE_LOG(LogCat, Warning, TEXT("[%s] [Thread: %d] %s(%d): ") Format , \
*FDateTime::Now().ToString(TEXT("%H:%M:%S.%s")), \
FPlatformTLS::GetCurrentThreadId(), \
ANSI_TO_TCHAR(__FUNCTION__), __LINE__, ##__VA_ARGS__ )

#define EERROR(LogCat, Format, ...) \
UE_LOG(LogCat, Error, TEXT("[%s] [Thread: %d] %s %s(%d): ") Format TEXT("\n%s"), \
*FDateTime::Now().ToString(TEXT("%H:%M:%S.%s")), \
FPlatformTLS::GetCurrentThreadId(), \
*GetNameSafe(this), \
ANSI_TO_TCHAR(__FUNCTION__), __LINE__, ##__VA_ARGS__ , \
ANSI_TO_TCHAR(__FILE__))
// -- SimpleLOG for Custom Category-- //

// очень длинные сообщения о файле в ERROR() - пишем отдельной строкой для лучшей читаемости !!!




// -- SimpleLOG overload for LogTemp Category -- //
#define SLOG(Format, ...)		ELOG(LogTemp, Format, ##__VA_ARGS__ )
#define SWARNING(Format, ...)	EWARNING(LogTemp, Format, ##__VA_ARGS__ )
#define SERROR(Format, ...)		EERROR(LogTemp, Format, ##__VA_ARGS__ )
// -- SimpleLOG overload for LogTemp Category -- //
// Макросы для C++ (категория по умолчанию - LogTemp)




// упрощенный синтаксис (без форматирования) - только само сообщение!

#define LOGS(cat, lvl, msg) UE_LOG(cat, lvl, TEXT(msg))	// LOGS базовая обертка (не требует заключать сообщение в TEXT(format) )

#define LOG(msg) LOGS(LogTemp, Log, msg)			// обычное СЕРОЕ сообщение лога
#define DISPLAY(msg) LOGS(LogTemp, Display, msg)	// БЕЛОЕ сообщение в логе
#define MESSAGE(msg) LOGS(LogTemp, Display, msg)	// БЕЛОЕ сообщение в логе (более интуитивная перегрузка)
#define WARNING(msg) LOGS(LogTemp, Warning, msg)	// ЖЁЛТОЕ сообщение предупреждения в логе
#define ERROR(msg) LOGS(LogTemp, Error, msg)		// КРАСНОЕ сообщение об ошибке
#define FATAL(msg) LOGS(LogTemp, Fatal, msg)		// АВАРИЙНОЕ ЗАВЕРШЕНИЕ ПРОГРАММЫ (краш) с красным сообщением об фатальной ошибке

#define LOG_NET(msg) LOGS(LogNet, Log, msg)
#define LOG_LOAD(msg) LOGS(LogLoad, Log, msg)

// С форматированием

#define LOGS_ARGS(cat, lvl, format, ...) UE_LOG(cat, lvl, TEXT(format), ##__VA_ARGS__)	// LOGS базовая обертка (не требует заключать сообщение в TEXT(format) ) + ARGS (с форматированием)

// упрощенный синтаксис (С форматированием)

#define LOGF(format, ...) LOGS_ARGS(LogTemp, Log, format, ##__VA_ARGS__)				// обычное СЕРОЕ сообщение лога (С форматированием)
#define DISPLAYF(format, ...) LOGS_ARGS(LogTemp, Display, format, ##__VA_ARGS__)		// БЕЛОЕ сообщение в логе (С форматированием)
#define MESSAGEF(format, ...) LOGS_ARGS(LogTemp, Display, format, ##__VA_ARGS__)		// БЕЛОЕ сообщение в логе (С форматированием) - более интуитивная перегрузка
#define WARNINGF(format, ...) LOGS_ARGS(LogTemp, Warning, format, ##__VA_ARGS__)		// ЖЁЛТОЕ сообщение предупреждения в логе (С форматированием)
#define ERRORF(format, ...) LOGS_ARGS(LogTemp, Error, format, ##__VA_ARGS__)			// КРАСНОЕ сообщение об ошибке (С форматированием)
#define FATALF(format, ...) LOGS_ARGS(LogTemp, Fatal, format, ##__VA_ARGS__)			// АВАРИЙНОЕ ЗАВЕРШЕНИЕ ПРОГРАММЫ (краш) с красным сообщением об фатальной ошибке (С форматированием)
//
//
// // базовая обертка - сама создает категорию + упрощенный синтаксис
// #define LOGS_UNIQUE_CATHEGORY(NewCategoryName, Verbosity, Format) \
// do { \
// DEFINE_LOG_CATEGORY_STATIC(NewCategoryName, Log, All); \
// UE_LOG(NewCategoryName, Verbosity, TEXT(Format)); \
// } while(0)
//
//
// #define ULOG(NewCategoryName, msg) LOGS_UNIQUE_CATHEGORY(NewCategoryName, Log, msg)
// #define UDISPLAY(NewCategoryName, msg) LOGS_UNIQUE_CATHEGORY(NewCategoryName, Display, msg)
// #define UMESSAGE(NewCategoryName, msg) LOGS_UNIQUE_CATHEGORY(NewCategoryName, Display, msg)
// #define UWARNING(NewCategoryName, msg) LOGS_UNIQUE_CATHEGORY(NewCategoryName, Warning, msg)
// #define UERROR(NewCategoryName, msg) LOGS_UNIQUE_CATHEGORY(NewCategoryName, Error, msg)
//
//
// // базовая обертка - сама создает категорию + упрощенный синтаксис (С форматированием)
// #define LOGS_UNIQUE_CATHEGORY_ARGS(NewCategoryName, Verbosity, Format, ...) \
// do { \
// DEFINE_LOG_CATEGORY_STATIC(NewCategoryName, Log, All); \
// UE_LOG(NewCategoryName, Verbosity, TEXT(Format), ##__VA_ARGS__); \
// } while(0)
//
// #define ULOGF(NewCategoryName, format, ...) LOGS_UNIQUE_CATHEGORY_ARGS(NewCategoryName, Log, format, ##__VA_ARGS__)
// #define UDISPLAYF(NewCategoryName, format, ...) LOGS_UNIQUE_CATHEGORY_ARGS(NewCategoryName, Display, format, ##__VA_ARGS__)
// #define UMESSAGEF(NewCategoryName, format, ...) LOGS_UNIQUE_CATHEGORY_ARGS(NewCategoryName, Display, format, ##__VA_ARGS__)
// #define UWARNINGF(NewCategoryName, format, ...) LOGS_UNIQUE_CATHEGORY_ARGS(NewCategoryName, Warning, format, ##__VA_ARGS__)
// #define UERRORF(NewCategoryName, format, ...) LOGS_UNIQUE_CATHEGORY_ARGS(NewCategoryName, Error, format, ##__VA_ARGS__)

// Использование

/**
void Test()
{
	LOG("Simple message");
	LOGF("Health: %d, Mana: %d", 100, 50);
	ULOG(MyCathegory, "Mana");
}
*/

// чистый шаблон комментария
 
/** 
 * @struct 
 * @brief 
 * 
 * @details 
 * - 
 * - 
 * 
 * @version 
 * @todo : 
 * @warning 
 * 
 * @param 
 * @tparam 
 * @example 
 * @return 
 * 
 * @note 
 * @see 
 */