#ifndef INC_DEBUG
#define INC_DEBUG

#ifndef DEBUG_COLOR
#define DEBUG_COLOR
#endif

//======================================================
// PLATFORM CHECK
//======================================================

#if defined(ARDUINO) && (defined(ESP32) || defined(ESP8266))

//======================================================
// COLOR
//======================================================

#ifdef DEBUG_COLOR

#define LOG_BLACK "\033[30m"
#define LOG_RED "\033[31m"
#define LOG_GREEN "\033[32m"
#define LOG_YELLOW "\033[33m"
#define LOG_BLUE "\033[34m"
#define LOG_MAGENTA "\033[35m"
#define LOG_CYAN "\033[36m"
#define LOG_WHITE "\033[37m"

#define LOG_BOLD "\033[1m"
#define LOG_RESET "\033[0m"

#else

#define LOG_BLACK ""
#define LOG_RED ""
#define LOG_GREEN ""
#define LOG_YELLOW ""
#define LOG_BLUE ""
#define LOG_MAGENTA ""
#define LOG_CYAN ""
#define LOG_WHITE ""

#define LOG_BOLD ""
#define LOG_RESET ""

#endif

//======================================================
// FILE NAME
//======================================================

static inline const char *FileName(const char *path)
{
    const char *file = path;

    while (*path)
    {
        if (*path == '/' || *path == '\\')
        {
            file = path + 1;
        }

        path++;
    }

    return file;
}

//======================================================
// CORE
//======================================================

#ifdef ESP32
#define CKC_CORE_ID xPortGetCoreID()
#else
#define CKC_CORE_ID 0
#endif

//======================================================
// BASE LOG
//======================================================

#if defined(DEBUG) || defined(DEBUG_COLOR)

#define LOG_BASE(headColor, msgColor, level, tag, format, ...) \
    do                                                         \
    {                                                          \
        if (Serial)                                            \
        {                                                      \
            Serial.printf(                                     \
                headColor LOG_BOLD                             \
                "[%10lu ms]"                                   \
                "[CORE %d]"                                    \
                "[%s]"                                         \
                "[%s:%d]"                                      \
                "[%s]"                                         \
                "[%s]: " msgColor format LOG_RESET "\n",       \
                millis(),                                      \
                CKC_CORE_ID,                                   \
                level,                                         \
                FileName(__FILE__),                            \
                __LINE__,                                      \
                __FUNCTION__,                                  \
                tag,                                           \
                ##__VA_ARGS__);                                \
        }                                                      \
    } while (0)

#else

#define LOG_BASE(...)

#endif

//======================================================
// NORMAL LOG
//======================================================

#define LOG_ERROR(tag, format, ...) \
    LOG_BASE(LOG_RED, LOG_WHITE, "ERROR", tag, format, ##__VA_ARGS__)

#define LOG_WARN(tag, format, ...) \
    LOG_BASE(LOG_YELLOW, LOG_WHITE, "WARN", tag, format, ##__VA_ARGS__)

#define LOG_INFO(tag, format, ...) \
    LOG_BASE(LOG_GREEN, LOG_WHITE, "INFO", tag, format, ##__VA_ARGS__)

#define LOG_DEBUG(tag, format, ...) \
    LOG_BASE(LOG_CYAN, LOG_WHITE, "DEBUG", tag, format, ##__VA_ARGS__)

//======================================================
// SPECIAL LOG
//======================================================

#define LOG_WIFI(tag, format, ...) \
    LOG_BASE(LOG_BLUE, LOG_WHITE, "WIFI", tag, format, ##__VA_ARGS__)

#define LOG_MQTT(tag, format, ...) \
    LOG_BASE(LOG_MAGENTA, LOG_WHITE, "MQTT", tag, format, ##__VA_ARGS__)

#define LOG_UART(tag, format, ...) \
    LOG_BASE(LOG_CYAN, LOG_WHITE, "UART", tag, format, ##__VA_ARGS__)

#define LOG_OK(tag, format, ...) \
    LOG_BASE(LOG_GREEN, LOG_GREEN, "OK", tag, format, ##__VA_ARGS__)

#define LOG_FAIL(tag, format, ...) \
    LOG_BASE(LOG_RED, LOG_RED, "FAIL", tag, format, ##__VA_ARGS__)

//======================================================
// MODBUS DEBUG
//======================================================

#ifdef MODBUS_DEBUG

#define MODBUS_LOG_ERROR(tag, format, ...) \
    LOG_BASE(LOG_RED, LOG_WHITE, "MB_ERR", tag, format, ##__VA_ARGS__)

#define MODBUS_LOG_WARN(tag, format, ...) \
    LOG_BASE(LOG_YELLOW, LOG_WHITE, "MB_WARN", tag, format, ##__VA_ARGS__)

#define MODBUS_LOG_INFO(tag, format, ...) \
    LOG_BASE(LOG_GREEN, LOG_WHITE, "MB_INFO", tag, format, ##__VA_ARGS__)

#define MODBUS_LOG_DEBUG(tag, format, ...) \
    LOG_BASE(LOG_CYAN, LOG_WHITE, "MB_DBG", tag, format, ##__VA_ARGS__)

#define MODBUS_CHECK_ERROR(tag, format, ...) \
    Serial.printf(format)

#define MODBUS_CHECK_WARN(tag, format, ...) \
    Serial.printf(format)

#define MODBUS_CHECK_INFO(tag, format, ...) \
    Serial.printf(format)

#define MODBUS_CHECK_DEBUG(tag, format, ...) \
    Serial.printf(format)

#else

#define MODBUS_LOG_ERROR(...)
#define MODBUS_LOG_WARN(...)
#define MODBUS_LOG_INFO(...)
#define MODBUS_LOG_DEBUG(...)

#define MODBUS_CHECK_ERROR(...)
#define MODBUS_CHECK_WARN(...)
#define MODBUS_CHECK_INFO(...)
#define MODBUS_CHECK_DEBUG(...)

#endif

#else

//======================================================
// NON ESP
//======================================================

#define LOG_ERROR(...)
#define LOG_WARN(...)
#define LOG_INFO(...)
#define LOG_DEBUG(...)

#define LOG_WIFI(...)
#define LOG_MQTT(...)
#define LOG_UART(...)

#define LOG_OK(...)
#define LOG_FAIL(...)

#define MODBUS_LOG_ERROR(...)
#define MODBUS_LOG_WARN(...)
#define MODBUS_LOG_INFO(...)
#define MODBUS_LOG_DEBUG(...)

#define MODBUS_CHECK_ERROR(...)
#define MODBUS_CHECK_WARN(...)
#define MODBUS_CHECK_INFO(...)
#define MODBUS_CHECK_DEBUG(...)

#endif

#endif