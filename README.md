# Minimal Cross-Platform Logger for STM32 & ESP32


A **minimal, clean, efficient logger** for **STM32** and **ESP32**, supporting:

* Log levels (`ERROR`, `WARN`, `INFO`, `DEBUG`)
* Optional **timestamp**
* Optional **colorized output** for terminals
* Optional **file name & line number** for debugging
* Switchable **output: UART or SEGGER RTT** on STM32
* UART on ESP32

Perfect for **embedded projects** that need lightweight logging over UART or RTT.

---

## Features

* ✅ Minimal and **header-only**
* ✅ Configurable **log levels**
* ✅ Works on **STM32 HAL UART**, **SEGGER RTT**, and **ESP-IDF UART**
* ✅ Optional **terminal colors**
* ✅ Optional **timestamping**
* ✅ Optional **file & line info**
* ✅ Thread-safe (when using HAL/IDF UART drivers or RTT)

---

## Installation

Simply **copy `logger.h`** into your project and include it:

```c
#include "logger.h"
```

---

## Configuration

### Platform / Output Selection

Define exactly one of the following **before** including `logger.h`:

```c
#define USE_STM32_UART   // STM32 using HAL UART
#define USE_STM32_RTT    // STM32 using SEGGER RTT
#define USE_ESP32        // ESP32 using UART
```

### Optional Settings

```c
#define LOGGER_USE_TIMESTAMP    1  // 0 = disable, 1 = enable
#define LOGGER_ENABLE_COLORS    1  // 0 = disable, 1 = enable
#define LOGGER_LEVEL            LOG_LEVEL_DEBUG  // default global level
//#define LOGGER_USE_FILELINE   1  // uncomment to add file:line info
```

---

## Usage

```c
#define USE_STM32_UART   // or USE_STM32_RTT / USE_ESP32
#include "logger.h"

int main(void) {
    LOG_INFO("System initialized");
    LOG_DEBUG("Temperature: %.2f°C", 36.5);
    LOG_WARN("Battery low");
    LOG_ERROR("Sensor error");
}
```

### Output Example (Colorized)

```
[123] I: System initialized
[124] D: Temperature: 36.50°C
[125] W: Battery low
[126] E: Sensor error
```

---

## Log Levels

| Macro       | Description            | Color (if enabled) |
| ----------- | ---------------------- | ------------------ |
| `LOG_ERROR` | Critical errors        | Red                |
| `LOG_WARN`  | Warnings               | Yellow             |
| `LOG_INFO`  | Informational messages | Green              |
| `LOG_DEBUG` | Debug messages         | Cyan               |

Global log level can be set:

```c
#define LOGGER_LEVEL LOG_LEVEL_INFO
```

Messages below this level will be ignored.

---

## Advanced: File Name & Line Number

Enable for easier debugging:

```c
#define LOGGER_USE_FILELINE 1
```

Example output:

```
[123] I: main.c:42 System initialized
```

---

## Supported Platforms

| Platform / Output | API / Function           |
| ----------------- | ------------------------ |
| STM32 UART        | `HAL_UART_Transmit`      |
| STM32 RTT         | `SEGGER_RTT_WriteString` |
| ESP32 UART        | `uart_write_bytes`       |

Switch output with the macros above.

---

## Notes

* **Buffer size**: 160 bytes (adjustable in `logger.h`)
* **Blocking output**: UART or RTT is used; consider non-blocking for performance-critical tasks.
* **No dynamic memory**: All buffers are stack-allocated.

---



## Example Project Structure

```
project/
├─ main.c
├─ logger.h
├─ usart.h        # STM32 HAL UART handles
├─ SEGGER_RTT.h   # Optional for RTT support
└─ CMakeLists.txt # or Makefile / CubeMX project
```

---

## Contribution

* Fork the repository
* Create a branch for your feature/fix
* Submit a pull request

---

## Git Commit Message Example

```
git commit -m "Add cross-platform logger with UART and SEGGER RTT support, log levels, timestamps, and optional file/line info"
```
