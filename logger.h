#pragma once
#include <stdio.h>
#include <stdint.h>

// ============================================================
// Configuration
// ============================================================

// Select platform: define one of these before including logger.h
// #define USE_STM32
// #define USE_ESP32

#define LOGGER_USE_TIMESTAMP   1   // 0 = off, 1 = include timestamp
#define LOGGER_ENABLE_COLORS   1   // 0 = off, 1 = colorized terminal
#define LOGGER_LEVEL           LOG_LEVEL_DEBUG  // default global level

// ============================================================
// Platform Abstraction Layer
// ============================================================

#if defined(USE_STM32)
#include "usart.h"
#define LOGGER_UART_HANDLE &huart2
#include "stm32f4xx_hal.h"

static inline void logger_write(const char *buf, int len) {
    HAL_UART_Transmit(LOGGER_UART_HANDLE, (uint8_t*)buf, len, HAL_MAX_DELAY);
}
static inline uint32_t logger_timestamp(void) {
    return HAL_GetTick();
}

#elif defined(USE_ESP32)
#include "driver/uart.h"
#include "esp_timer.h"
#define LOGGER_UART_NUM UART_NUM_0

static inline void logger_write(const char *buf, int len) {
    uart_write_bytes(LOGGER_UART_NUM, buf, len);
}
static inline uint32_t logger_timestamp(void) {
    return (uint32_t)(esp_timer_get_time() / 1000);
}

#else
#error "Please define USE_STM32 or USE_ESP32 before including logger.h"
#endif

// ============================================================
// Log Levels
// ============================================================

typedef enum {
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
} log_level_t;

// Optional color output
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
// Core Logging Macro
// ============================================================

#define LOG(level, color, tag, fmt, ...) do { \
    if (level <= LOGGER_LEVEL) { \
        char buf[160]; \
        int len; \
        if (LOGGER_USE_TIMESTAMP) \
            len = snprintf(buf, sizeof(buf), "%s[%lu] %s: " fmt "%s\r\n", \
                color, (unsigned long)logger_timestamp(), tag, ##__VA_ARGS__, CLR_RESET); \
        else \
            len = snprintf(buf, sizeof(buf), "%s%s: " fmt "%s\r\n", \
                color, tag, ##__VA_ARGS__, CLR_RESET); \
        logger_write(buf, len); \
    } \
} while(0)

// ============================================================
// Convenience Macros
// ============================================================

#define LOG_ERROR(fmt, ...) LOG(LOG_LEVEL_ERROR, CLR_RED,    "E", fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  LOG(LOG_LEVEL_WARN,  CLR_YELLOW, "W", fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  LOG(LOG_LEVEL_INFO,  CLR_GREEN,  "I", fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG(LOG_LEVEL_DEBUG, CLR_CYAN,   "D", fmt, ##__VA_ARGS__)
