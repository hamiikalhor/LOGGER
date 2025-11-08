#pragma once
#include <stdio.h>
#include <stdint.h>

// ============================================================
// PLATFORM SELECTION
// ============================================================

// Define exactly one platform before including logger.h
// STM32 with UART:  #define USE_STM32_UART
// STM32 with RTT:   #define USE_STM32_RTT
// ESP32 UART:       #define USE_ESP32

#define LOGGER_USE_TIMESTAMP   1
#define LOGGER_ENABLE_COLORS   1
#define LOGGER_LEVEL           LOG_LEVEL_DEBUG
//#define LOGGER_USE_FILELINE   1  // Uncomment to add file:line info

// ============================================================
// PLATFORM ABSTRACTION
// ============================================================

#if defined(USE_STM32_UART)
#include "usart.h"    // STM32 HAL UART handle
#include "stm32f4xx_hal.h"
static inline void logger_write(const char *buf, int len) {
    HAL_UART_Transmit(&huart2, (uint8_t*)buf, len, HAL_MAX_DELAY);
}
static inline uint32_t logger_timestamp(void) { return HAL_GetTick(); }

#elif defined(USE_STM32_RTT)
#include "SEGGER_RTT.h"
static inline void logger_write(const char *buf, int len) {
    (void)len; // RTT ignores length
    SEGGER_RTT_WriteString(0, buf);
}
static inline uint32_t logger_timestamp(void) { return HAL_GetTick(); } // optional

#elif defined(USE_ESP32)
#include "driver/uart.h"
#include "esp_timer.h"
#define LOGGER_UART_NUM UART_NUM_0
static inline void logger_write(const char *buf, int len) {
    uart_write_bytes(LOGGER_UART_NUM, buf, len);
}
static inline uint32_t logger_timestamp(void) { return (uint32_t)(esp_timer_get_time() / 1000); }

#else
#error "Please define one platform: USE_STM32_UART, USE_STM32_RTT, USE_ESP32"
#endif

// ============================================================
// LOG LEVELS
// ============================================================

typedef enum {
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
} log_level_t;

#if LOGGER_ENABLE_COLORS
#define CLR_RESET  "\033[0m"
#define CLR_RED    "\033[31m"
#define CLR_YELLOW "\033[33m"
#define CLR_GREEN  "\033[32m"
#define CLR_CYAN   "\033[36m"
#else
#define CLR_RESET  ""
#define CLR_RED    ""
#define CLR_YELLOW ""
#define CLR_GREEN  ""
#define CLR_CYAN   ""
#endif

// ============================================================
// CORE LOG MACRO
// ============================================================

#ifdef LOGGER_USE_FILELINE
#define LOG(level, color, tag, fmt, ...) do { \
    if(level <= LOGGER_LEVEL) { \
        char buf[160]; \
        int len; \
        if(LOGGER_USE_TIMESTAMP) \
            len = snprintf(buf, sizeof(buf), "%s[%lu] %s %s:%d: " fmt "%s\r\n", \
                color, (unsigned long)logger_timestamp(), tag, __FILE__, __LINE__, ##__VA_ARGS__, CLR_RESET); \
        else \
            len = snprintf(buf, sizeof(buf), "%s%s %s:%d: " fmt "%s\r\n", \
                color, tag, __FILE__, __LINE__, ##__VA_ARGS__, CLR_RESET); \
        logger_write(buf, len); \
    } \
} while(0)
#else
#define LOG(level, color, tag, fmt, ...) do { \
    if(level <= LOGGER_LEVEL) { \
        char buf[160]; \
        int len; \
        if(LOGGER_USE_TIMESTAMP) \
            len = snprintf(buf, sizeof(buf), "%s[%lu] %s: " fmt "%s\r\n", \
                color, (unsigned long)logger_timestamp(), tag, ##__VA_ARGS__, CLR_RESET); \
        else \
            len = snprintf(buf, sizeof(buf), "%s%s: " fmt "%s\r\n", \
                color, tag, ##__VA_ARGS__, CLR_RESET); \
        logger_write(buf, len); \
    } \
} while(0)
#endif

// ============================================================
// CONVENIENCE MACROS
// ============================================================

#define LOG_ERROR(fmt, ...) LOG(LOG_LEVEL_ERROR, CLR_RED,    "E", fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  LOG(LOG_LEVEL_WARN,  CLR_YELLOW, "W", fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  LOG(LOG_LEVEL_INFO,  CLR_GREEN,  "I", fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG(LOG_LEVEL_DEBUG, CLR_CYAN,   "D", fmt, ##__VA_ARGS__)
