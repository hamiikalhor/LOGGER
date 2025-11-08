# Minimal UART Logger for STM32 & ESP32


A **minimal, clean, efficient UART logger** for **STM32** and **ESP32**, supporting:

* Log levels (`ERROR`, `WARN`, `INFO`, `DEBUG`)
* Optional **timestamp**
* Optional **colorized output** for terminals
* Optional **file name & line number** for debugging
* Switchable platform using a single `#define`

Perfect for **embedded projects** that need lightweight logging over UART.

---

## Features

* ✅ Minimal and **header-only**
* ✅ Configurable **log levels**
* ✅ Works on **STM32 HAL** and **ESP-IDF**
* ✅ Optional **terminal colors**
* ✅ Optional **timestamping**
* ✅ Optional **file & line info**
* ✅ Thread-safe (when using HAL/IDF UART drivers)

---

## Installation

Simply **copy `logger.h`** into your project and include it:

```c
#include "logger.h"
```

---

## Configuration

### Platform Selection

Before including `logger.h`, define your platform:

```c
#define USE_STM32   // or
#define USE_ESP32
```

### Optional Settings

```c
#define LOGGER_USE_TIMESTAMP    1  // 0 = disable, 1 = enable
#define LOGGER_ENABLE_COLORS    1  // 0 = disable, 1 = enable
#define LOGGER_LEVEL            LOG_LEVEL_DEBUG  // default global level
```

---

## Usage

```c
#define USE_STM32   // or USE_ESP32
#include "logger.h"

void main(void) {
    LOG_INFO("System started");
    LOG_DEBUG("Temperature: %.2f°C", 36.75);
    LOG_WARN("Battery low");
    LOG_ERROR("Sensor read failed");
}
```

### Output Example (Colorized)

```
[123] I: System started
[124] D: Temperature: 36.75°C
[125] W: Battery low
[126] E: Sensor read failed
```

---

## Log Levels

| Macro       | Description            | Color (if enabled) |
| ----------- | ---------------------- | ------------------ |
| `LOG_ERROR` | Critical errors        | Red                |
| `LOG_WARN`  | Warnings               | Yellow             |
| `LOG_INFO`  | Informational messages | Green              |
| `LOG_DEBUG` | Debug messages         | Cyan               |

You can globally set the **active log level**:

```c
#define LOGGER_LEVEL LOG_LEVEL_INFO
```

Messages below this level will be ignored.

---

## Advanced: File Name & Line Number

Enable file and line info for easier debugging:

```c
#define LOGGER_USE_FILELINE 1
```

Output example:

```
[123] I: main.c:42 System started
```

---

## Supported Platforms

| Platform | UART API                        |
| -------- | ------------------------------- |
| STM32    | `HAL_UART_Transmit` (STM32 HAL) |
| ESP32    | `uart_write_bytes` (ESP-IDF)    |

Switch platforms with a single macro:

```c
#define USE_STM32
// OR
#define USE_ESP32
```

---

## Notes

* **Buffer size**: 160 bytes (can be increased in `logger.h`)
* **Blocking UART**: `HAL_UART_Transmit` or `uart_write_bytes` is used; consider non-blocking if needed for performance-critical tasks.
* **No dynamic memory**: All buffers are stack-allocated.

---



## Example Project Structure

```
project/
├─ main.c
├─ logger.h
├─ usart.h        # STM32 HAL UART handles
└─ CMakeLists.txt # or Makefile / CubeMX project
```

---

## Contribution

* Fork the repository
* Create a branch for your feature/fix
* Submit a pull request
