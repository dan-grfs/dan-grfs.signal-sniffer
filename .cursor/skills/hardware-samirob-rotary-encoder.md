# SAMIROB OLED Rotary Encoder Module - Hardware Skill

## Hardware Identification

**Brand**: SAMIROB  
**Product**: OLED Rotary Encoder Module  
**Model**: 1.3-inch OLED version  
**Purchase Link**: [AliExpress Item 1005008817896911](https://www.aliexpress.com/item/1005008817896911.html)

**Module Components**:
- **Rotary Encoder**: EC11 type, 20 detents per rotation, 360° continuous rotation
- **OLED Display**: 1.3-inch (3.5 cm) OLED display, 128x64 pixels, I2C interface
- **Buttons**: CONFIRM button, BAK (Back) button, Encoder push button (non-functional on this variant)
- **Power**: 3.3V or 5V compatible
- **Interface**: 9-pin header with I2C for OLED

## ⚠️ Critical Library Note

**OLED Library**: Despite being labeled as SH1106, this module **REQUIRES Adafruit SSD1306 library** to work correctly! Always use SSD1306 library, not SH1106.

## Pin Configuration

### 9-Pin Header Pinout

| Pin Position | Wire Color | Label | Function | Arduino Pin (Example) |
|-------------|------------|-------|----------|----------------------|
| 1 (Top) | **Grey** | `CONFIRM` | Confirm button input | Pin 6 (with INPUT_PULLUP) |
| 2 | **Purple** | `OLED_SDA` | OLED I2C Data line | A4 (Uno) or SDA pin |
| 3 | **Blue** | `OLED_SCL` | OLED I2C Clock line | A5 (Uno) or SCL pin |
| 4 | **Green** | `ENCODER_PUSH` | Rotary encoder push button | Pin 5 (⚠️ **Non-functional**) |
| 5 | **Yellow** | `ENCODER_TRA` | Rotary encoder channel A | Pin 3 |
| 6 | **Orange** | `ENCODER_TRB` | Rotary encoder channel B | Pin 4 |
| 7 | **Red** | `BAK` | Back button input | Pin 7 (with INPUT_PULLUP) |
| 8 | **Brown** | `GND` | Ground | GND |
| 9 (Bottom) | **Black** | `3V3-5V` | Power (3.3V or 5V) | 5V or 3.3V |

### Quick Wire Color Reference

- **Grey** → CONFIRM button
- **Purple** → OLED_SDA (I2C Data)
- **Blue** → OLED_SCL (I2C Clock)
- **Green** → ENCODER_PUSH (⚠️ Non-functional on this variant)
- **Yellow** → ENCODER_TRA (Channel A)
- **Orange** → ENCODER_TRB (Channel B)
- **Red** → BAK button
- **Brown** → GND
- **Black** → Power (3.3V or 5V)

## Voltage Levels

- **Power Input**: 3.3V or 5V (compatible with both)
- **GPIO Logic**: Compatible with 5V Arduino boards (Uno, Mega) and 3.3V boards (ESP32, ESP8266)
- **I2C Levels**: Follows power supply voltage (3.3V or 5V)

## Hardware Specifications

### Rotary Encoder (EC11)
- **Type**: EC11 mechanical rotary encoder
- **Resolution**: 20 detents per full rotation
- **Rotation**: 360° continuous rotation
- **Channels**: Quadrature output (TRA/TRB)
- **Push Button**: ⚠️ **Non-functional on this module variant** - only rotation works

### OLED Display
- **Size**: 1.3-inch (3.5 cm diagonal)
- **Resolution**: 128x64 pixels
- **Interface**: I2C
- **Driver**: SSD1306 (despite being labeled as SH1106)
- **I2C Address**: Typically 0x3C (verify with I2C scanner)
- **Library**: **Must use Adafruit SSD1306 library**

### Buttons
- **Logic**: All buttons are **active-low** (LOW when pressed, HIGH when released)
- **Pull-up**: Requires internal or external pull-up resistors
- **Working Buttons**:
  - ✅ CONFIRM (Grey wire) - Dedicated confirm/select button
  - ✅ BAK (Red wire) - Dedicated back/cancel button
- **Non-functional**:
  - ❌ ENCODER_PUSH (Green wire) - Does not work on this module variant

## Required Libraries

### Core Libraries
1. **Encoder** by Paul Stoffregen
   - Install: Arduino Library Manager → Search "Encoder"
   - GitHub: https://github.com/PaulStoffregen/Encoder

2. **Adafruit SSD1306** ⚠️ (NOT SH1106!)
   - Install: Arduino Library Manager → Search "Adafruit SSD1306"
   - GitHub: https://github.com/adafruit/Adafruit_SSD1306
   - **Critical**: Module may be mislabeled as SH1106, but SSD1306 library is required

3. **Adafruit GFX Library** (dependency for SSD1306)
   - Install: Arduino Library Manager → Search "Adafruit GFX"
   - Automatically installed with SSD1306 library

4. **Wire** (built-in Arduino library)
   - No installation needed - part of Arduino core

## Code Patterns

### Basic Encoder Setup

```cpp
#include <Encoder.h>

#define ENC_A 3      // Yellow wire - Channel A
#define ENC_B 4      // Orange wire - Channel B

Encoder encoder(ENC_A, ENC_B);

void setup() {
  Serial.begin(9600);
  // Encoder library handles pin configuration automatically
}

void loop() {
  long pos = encoder.read() / 4;  // Divide by 4 to smooth readings (20 detents)
  // Use position value
}
```

### Button Reading (Active-Low)

```cpp
#define BTN_CONFIRM 6  // Grey wire
#define BTN_BAK 7      // Red wire

void setup() {
  pinMode(BTN_CONFIRM, INPUT_PULLUP);
  pinMode(BTN_BAK, INPUT_PULLUP);
}

void loop() {
  // Active-low: LOW when pressed, HIGH when released
  bool confirmPressed = !digitalRead(BTN_CONFIRM);
  bool bakPressed = !digitalRead(BTN_BAK);
  
  if (confirmPressed) {
    // Handle confirm action
  }
  
  if (bakPressed) {
    // Handle back action
  }
}
```

### OLED Display Setup (SSD1306)

```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Wire.begin();
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Hello!"));
  display.display();
}
```

### I2C Scanner (Troubleshooting)

```cpp
void scanI2C() {
  Serial.println(F("Scanning I2C bus..."));
  byte error, address;
  int nDevices = 0;
  
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print(F("I2C device found at 0x"));
      if (address < 16) Serial.print(F("0"));
      Serial.println(address, HEX);
      nDevices++;
    }
  }
  
  if (nDevices == 0) {
    Serial.println(F("No I2C devices found!"));
  }
}
```

### Complete Integration Example

```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Encoder.h>

// OLED Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Encoder Configuration
#define ENC_A 3
#define ENC_B 4
Encoder encoder(ENC_A, ENC_B);

// Button Configuration
#define BTN_CONFIRM 6
#define BTN_BAK 7

long lastPos = 0;

void setup() {
  Serial.begin(9600);
  
  // Configure buttons with pull-up
  pinMode(BTN_CONFIRM, INPUT_PULLUP);
  pinMode(BTN_BAK, INPUT_PULLUP);
  
  // Initialize I2C and OLED
  Wire.begin();
  if (display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Ready"));
    display.display();
  }
}

void loop() {
  // Read encoder position (divide by 4 to smooth)
  long pos = encoder.read() / 4;
  
  // Read buttons (active-low)
  bool confirmPressed = !digitalRead(BTN_CONFIRM);
  bool bakPressed = !digitalRead(BTN_BAK);
  
  // Update display on changes
  if (pos != lastPos || confirmPressed || bakPressed) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(F("Pos: "));
    display.println(pos);
    display.print(F("Confirm: "));
    display.println(confirmPressed ? F("ON") : F("OFF"));
    display.print(F("Back: "));
    display.println(bakPressed ? F("ON") : F("OFF"));
    display.display();
    lastPos = pos;
  }
  
  delay(10);
}
```

## Best Practices

### Memory Optimization
- Use `F()` macro for string literals to save RAM: `Serial.println(F("Text"));`
- Keep Serial output minimal to avoid memory issues
- Update display only when values change, not every loop iteration

### Encoder Handling
- Divide encoder position by 4 to smooth readings (20 detents per rotation)
- Use `long` type for encoder position (can accumulate large values)
- Check for position changes before updating display/actions

### Button Handling
- Always use `INPUT_PULLUP` mode for buttons (active-low logic)
- Use `!digitalRead()` to invert logic (LOW = pressed)
- Implement debouncing if needed (though hardware may have some debouncing)

### OLED Display
- Always verify I2C address with scanner before troubleshooting
- Use `display.display()` after drawing to update screen
- Clear display with `display.clearDisplay()` before redrawing
- Use `F()` macro for text strings to save RAM

### Wiring Verification
- ⚠️ **Important**: Wiring harness may differ from official spec - always verify connections
- Use I2C scanner to verify OLED connection
- Test encoder rotation in both directions
- Verify button connections with Serial output

## Troubleshooting

### OLED Display Not Working
1. **Verify I2C connection**: Run I2C scanner to check if device is detected
2. **Check library**: Ensure you're using **SSD1306 library**, not SH1106
3. **Verify address**: Try both 0x3C and 0x3D addresses
4. **Check wiring**: Verify SDA (Purple) and SCL (Blue) connections
5. **Power check**: Ensure module has power (Black wire to 5V/3.3V, Brown to GND)

### Encoder Not Responding
1. **Check wiring**: Verify Yellow (TRA) and Orange (TRB) connections
2. **Test pins**: Try different interrupt-capable pins if available
3. **Verify rotation**: Test rotation in both directions
4. **Position smoothing**: Remember to divide by 4 for smooth readings

### Buttons Not Working
1. **Verify pull-up**: Ensure `INPUT_PULLUP` mode is set
2. **Check logic**: Remember buttons are active-low (use `!digitalRead()`)
3. **Test wiring**: Verify Grey (CONFIRM) and Red (BAK) connections
4. **Note**: Encoder push button (Green) does NOT work on this variant

### I2C Communication Issues
1. **Run I2C scanner**: Verify device is detected on bus
2. **Check pull-up resistors**: I2C requires pull-up resistors (usually on module)
3. **Verify connections**: SDA to SDA, SCL to SCL (not swapped)
4. **Power supply**: Ensure stable power supply (3.3V or 5V)

## CLI Tool Configuration

### Arduino CLI

**General Usage** (see `cli-tool-arduino-cli.md` for full details):
```bash
arduino-cli compile --fqbn <fqbn> <sketch>
arduino-cli upload -p <port> --fqbn <fqbn> <sketch>
```

**Hardware-Specific Configuration**:
- This module works with any Arduino-compatible board
- FQBN depends on your Arduino board, not the rotary module
- Common FQBNs:
  - Arduino Uno: `arduino:avr:uno`
  - Arduino Nano: `arduino:avr:nano`
  - ESP32: `esp32:esp32:esp32`
  - ESP8266: `esp8266:esp8266:nodemcuv2`

**Port Detection**:
```bash
arduino-cli board list
```

**Example Compile/Upload** (Arduino Uno):
```bash
arduino-cli compile --fqbn arduino:avr:uno rotary-display.ino
arduino-cli upload -p COM3 --fqbn arduino:avr:uno rotary-display.ino
```

## Code Examples Reference

See project files:
- `rotary-display.ino` - Current working implementation with OLED
- `docs/rotary-module-specifications.md` - Complete hardware documentation
- `README.md` - Project overview and pin mappings

## Important Notes

1. **OLED Library**: Always use **SSD1306 library**, not SH1106 (despite labeling)
2. **Encoder Push Button**: Green wire (ENCODER_PUSH) does NOT work on this module variant
3. **Button Logic**: All buttons are active-low (LOW when pressed)
4. **Position Smoothing**: Divide encoder position by 4 for smooth readings
5. **Wiring Verification**: Always verify connections - harness may differ from spec
6. **I2C Address**: Typically 0x3C, but verify with I2C scanner
7. **Power**: Module accepts 3.3V or 5V (compatible with most Arduino boards)
