# ESP32-WROOM-32U Hardware Reference

## Purpose
Hardware specifications, pin configurations, and physical characteristics for the ESP32-WROOM-32U DevKitC board. Use this when selecting pins, making connections, or understanding hardware capabilities.

## Hardware Identification

**Board**: ESP32-WROOM-32U DevKitC
- **Module**: ESP32-WROOM-32U-N4 (4 MB flash, external antenna)
- **Module Variant**: ESP-32U TYPE-C WROOM with antenna
- **Chip**: ESP32 (Dual-core Xtensa LX6, 240 MHz)
- **Architecture**: Xtensa LX6 dual-core 32-bit microprocessor
- **USB-to-Serial**: CP2102 chip (confirmed - requires CP2102 drivers)
- **Purchase Link**: [AliExpress Item 1005010136688086](https://www.aliexpress.com/item/1005010136688086.html)

## Key Hardware Features

✅ **WiFi**: 2.4 GHz 802.11 b/g/n (full WiFi support)
✅ **Bluetooth**: Bluetooth Classic + BLE (can use both simultaneously)
✅ **Dual-Core**: Xtensa LX6 dual-core (240 MHz) - can utilize both cores
✅ **External Antenna**: U.FL/IPEX connector for 2.4G antenna (included)
✅ **Flash**: 4 MB (N4 variant)
✅ **SRAM**: 520 KB total
✅ **GPIO**: 34 GPIO pins available
✅ **5V Tolerant Pins**: GPIO 2, 4, 12, 13, 14, 15, 25, 26, 27

## Pin Configuration Reference

### Power Pins
- **3V3**: 3.3V power output (from onboard regulator)
- **GND**: Ground (multiple pins available)
- **VIN**: 5V input (when not using USB)
- **EN**: Enable/Reset pin (reset when pulled low)

### Communication Pins
- **TX**: UART Transmit (typically GPIO 1)
- **RX**: UART Receive (typically GPIO 3)
- **I2C**: Multiple GPIOs support I2C (default: GPIO 21=SDA, GPIO 22=SCL)
- **SPI**: Multiple SPI interfaces available
- **UART**: 3 UART interfaces (UART0, UART1, UART2)

### Confirmed Pin Headers (from board)

**Left Side** (top to bottom):
- 3V3, EN, UP, UN, GPIO 31, GPIO 35, GPIO 32, GPIO 33, GPIO 25

**Right Side** (top to bottom):
- GND, GPIO 23, GPIO 22, TX, RX, GPIO 21, GND, GPIO 19, GPIO 18, GPIO 5

### Special Pins
- **Boot Button**: GPIO 0 (pulled low to enter bootloader)
- **Reset Button**: EN pin
- **Built-in LED**: Typically GPIO 2 (may vary by board)
- **Strapping Pins**: GPIO 0, 2, 5, 12, 15 (affect boot behavior - use carefully)

### ADC Pins
- **12-bit ADC**: GPIO 0-15, GPIO 25-27 (0-3.3V range)
- **Common ADC pins**: GPIO 32, 33, 34, 35, 36, 39
- **Note**: GPIO 34, 35, 36, 39 are input-only (no output capability)

### DAC Pins
- **8-bit DAC**: GPIO 25, GPIO 26

### Touch Pins
- **Capacitive Touch**: GPIO 0, 2, 4, 12, 13, 14, 15, 27, 32, 33

### 5V Tolerant Pins
- GPIO 2, 4, 12, 13, 14, 15, 25, 26, 27
- **All other GPIOs are 3.3V only** - do not exceed 3.3V!

## Important Hardware Considerations

### Voltage Levels
- **Most GPIOs**: 3.3V logic level
- **5V Tolerant**: Only GPIO 2, 4, 12, 13, 14, 15, 25, 26, 27
- **Never exceed 3.3V** on non-5V-tolerant pins
- **Input-only pins** (GPIO 34, 35, 36, 39): Cannot be used as outputs

### Boot Behavior
- **Strapping pins** (GPIO 0, 2, 5, 12, 15) affect boot mode
- **GPIO 0**: Boot button - pulled low to enter bootloader
- May need to hold Boot button during upload if auto-reset doesn't work

### USB-to-Serial
- **Chip**: CP2102 (confirmed from board)
- **Not native USB** (unlike ESP32-S2/S3)
- Requires CP2102 drivers for programming
- Serial communication at 115200 baud (default)

### Antenna
- **External antenna connector**: U.FL/IPEX connector
- **2.4G antenna included** with purchase
- Connect antenna for better WiFi/Bluetooth range
- Can work without external antenna (uses internal if available)

### Memory Constraints
- **SRAM**: 520 KB total (manage carefully)
- **Flash**: 4 MB (N4 variant)
- **No PSRAM** on this variant
- Use `F()` macro for strings to save RAM

### Dual-Core Considerations
- Can assign tasks to specific cores (0 or 1)
- Useful for separating WiFi/Bluetooth tasks from sensor tasks
- Use `xTaskCreatePinnedToCore()` for core assignment

## Code Patterns and Best Practices

### Memory Management

#### String Literals
- **Always use `F()` macro** for string literals to save RAM:
  ```cpp
  Serial.println(F("This string is stored in flash, not RAM"));
  ```
- **Keep Serial output reasonable** - ESP32 has 520 KB SRAM
- **Use PROGMEM** for large constant arrays

#### RAM Optimization
```cpp
// Good: String in flash
Serial.println(F("Status: OK"));

// Bad: String in RAM
Serial.println("Status: OK");
```

### WiFi Usage

```cpp
#include <WiFi.h>

// WiFi is fully supported
WiFi.mode(WIFI_STA);
WiFi.begin("SSID", "password");

// Wait for connection
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(F("."));
}

Serial.println(F("\nWiFi connected!"));
Serial.print(F("IP address: "));
Serial.println(WiFi.localIP());

// Can use WiFi and Bluetooth simultaneously
```

### Bluetooth Usage

#### Bluetooth Classic
```cpp
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

void setup() {
  SerialBT.begin("ESP32-Device"); // Device name
  Serial.begin(115200);
}

void loop() {
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
}
```

#### BLE (Bluetooth Low Energy)
```cpp
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// BLE setup code...
// Can use both WiFi and Bluetooth at the same time
```

### Dual-Core Usage

```cpp
#include <Arduino.h>

// Task function
void taskFunction(void *parameter) {
  for(;;) {
    // Your task code here
    delay(1000);
  }
}

void setup() {
  // Assign task to specific core
  xTaskCreatePinnedToCore(
    taskFunction,    // Function to run
    "TaskName",      // Task name
    10000,           // Stack size (bytes)
    NULL,            // Parameters
    1,               // Priority (0-25, higher = more priority)
    NULL,            // Task handle
    0                // Core ID (0 or 1)
  );
}
```

### GPIO Configuration

#### Digital I/O
```cpp
// Standard digital output
pinMode(pin, OUTPUT);
digitalWrite(pin, HIGH);
digitalWrite(pin, LOW);

// Input with pullup
pinMode(pin, INPUT_PULLUP);
int state = digitalRead(pin);

// Input with pulldown
pinMode(pin, INPUT_PULLDOWN);
int state = digitalRead(pin);
```

#### Analog Input (ADC)
```cpp
#define ADC_PIN 32  // GPIO 32 (ADC1_CH4)

void setup() {
  Serial.begin(115200);
  // ADC is automatically configured
}

void loop() {
  int raw = analogRead(ADC_PIN);  // 0-4095 (12-bit)
  float voltage = (raw / 4095.0) * 3.3;  // Convert to voltage
  
  Serial.print(F("Raw: "));
  Serial.print(raw);
  Serial.print(F(" | Voltage: "));
  Serial.print(voltage, 2);
  Serial.println(F("V"));
  
  delay(1000);
}
```

#### PWM Output
```cpp
#define PWM_PIN 2

void setup() {
  // Use analogWrite for simple PWM (0-255)
  analogWrite(PWM_PIN, 128);  // 50% duty cycle
  
  // Or use ledc functions for more control
  ledcSetup(0, 5000, 8);  // Channel 0, 5kHz, 8-bit resolution
  ledcAttachPin(PWM_PIN, 0);
  ledcWrite(0, 128);  // 50% duty cycle
}
```

#### Touch Sensor
```cpp
#define TOUCH_PIN 4  // GPIO 4 (touch capable)

void setup() {
  Serial.begin(115200);
}

void loop() {
  int touchValue = touchRead(TOUCH_PIN);
  Serial.print(F("Touch value: "));
  Serial.println(touchValue);
  
  if (touchValue < 40) {  // Threshold (adjust as needed)
    Serial.println(F("Touched!"));
  }
  
  delay(100);
}
```

### Power Management

```cpp
#include <esp_sleep.h>

// Deep sleep (very low power ~10 µA)
void deepSleep(uint64_t time_in_us) {
  esp_sleep_enable_timer_wakeup(time_in_us);
  esp_deep_sleep_start();
}

// Light sleep (~0.8 mA)
void lightSleep(uint64_t time_in_us) {
  esp_sleep_enable_timer_wakeup(time_in_us);
  esp_light_sleep_start();
}

// Example: Deep sleep for 10 seconds
void setup() {
  Serial.begin(115200);
  Serial.println(F("Going to sleep..."));
  delay(1000);
  deepSleep(10 * 1000000);  // 10 seconds in microseconds
}
```
### Common Libraries

#### Built-in Libraries
- `WiFi.h` - WiFi functionality
- `BluetoothSerial.h` - Bluetooth Classic
- `BLEDevice.h` - Bluetooth Low Energy
- `driver/adc.h` - ADC control
- `esp_adc_cal.h` - ADC calibration
- `esp_sleep.h` - Sleep modes

#### External Libraries
- Install via Arduino Library Manager as needed
- Check `README.md` and `docs/hardware-specifications.md` for project-specific libraries

## Code Examples

### Basic LED Blink
```cpp
#define LED_PIN 2  // Common LED pin (verify on your board)

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println(F("LED ON"));
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  Serial.println(F("LED OFF"));
  delay(1000);
}
```

### WiFi Connection with Error Handling
```cpp
#include <WiFi.h>

const char* ssid = "YourSSID";
const char* password = "YourPassword";

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.print(F("Connecting to WiFi"));
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(F("."));
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("\nWiFi connected!"));
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
  } else {
    Serial.println(F("\nWiFi connection failed!"));
  }
}

void loop() {
  // Your code here
}
```

## Code Style Guidelines

### Always Include
- `F()` macro for all string literals
- Error handling for WiFi/Bluetooth connections
- Appropriate delays for hardware initialization
- Serial output for debugging (use `F()` macro)

### Pin Selection
- Always specify exact GPIO pin numbers
- Include voltage level warnings for 3.3V vs 5V tolerant pins
- Mention if a pin is input-only or has special functions
- Avoid strapping pins (GPIO 0, 2, 5, 12, 15) unless necessary

### When to Use Dual-Core
- Separate WiFi/Bluetooth tasks from sensor tasks
- CPU-intensive operations on one core, I/O on another
- Real-time critical tasks on dedicated core

## When to Use This Skill

Use this skill when:
- Writing code for the ESP32-WROOM-32U
- Selecting GPIO pins for connections
- Understanding pin capabilities and limitations
- Checking voltage levels and compatibility
- Identifying hardware features
- Planning hardware connections
- Understanding memory constraints
- Implementing WiFi or Bluetooth features
- Working with GPIO pins
- Managing memory and power
- Following project code conventions
- Creating new features or functions
