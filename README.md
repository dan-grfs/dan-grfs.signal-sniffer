# WiFi Signal Sniffer - ESP32 Project

A WiFi monitoring device built on ESP32-WROOM-32U that passively monitors all WiFi traffic in promiscuous mode, collecting MAC addresses and signal strength data.

## Project Overview

This project consists of:
- **ESP32 Firmware**: Promiscuous mode WiFi sniffer that collects MAC addresses and RSSI data
- **Android/Web App**: Mobile application to configure device, set time, and download logs via USB serial
- **Data Logging**: Stores timestamped signal data to flash storage (SPIFFS/LittleFS)

## Hardware

- **Board**: ESP32-WROOM-32U DevKitC
- **Module**: ESP32-WROOM-32U-N4 (4 MB flash, external antenna)
- **Connection**: USB-C to phone via OTG adapter
- **Storage**: 4 MB flash (1.5-2 MB available for data logging)

See `.cursor/skills/hardware-esp32-wroom-32u.md` for detailed hardware specifications.

## Current Status

✅ **Working Features:**
- Promiscuous mode WiFi packet capture
- MAC address and RSSI collection
- Summary output every 10 seconds
- Channel scanning (1-11, 2.4 GHz)
- USB serial communication

🚧 **In Progress:**
- Timestamp logging
- Flash storage (SPIFFS/LittleFS)
- Serial command protocol
- Android/web app development

## Quick Start

### Build, Upload, and Monitor (One Command)

**PowerShell:**
```bash
arduino-cli compile --fqbn esp32:esp32:esp32 .; arduino-cli upload -p COM4 --fqbn esp32:esp32:esp32 .; arduino-cli monitor -p COM4 -c baudrate=115200
```

**Note:** Replace `COM4` with your actual port (check with `arduino-cli board list`)

This single command will:
1. Compile the sketch
2. Upload to ESP32
3. Open serial monitor to view output

### Connect to Phone

1. Connect ESP32 to Android phone via USB OTG cable
2. Open serial terminal app (e.g., "Serial USB Terminal")
3. Select ESP32 serial port, baud rate 115200
4. View real-time WiFi signal data

## Project Structure

```
.
├── dan-grfs.signal-sniffer.ino  # Main ESP32 firmware
├── test.txt                      # Root-level text file for testing
├── sketch.yaml                   # Arduino CLI configuration
├── docs/                         # Project documentation
│   ├── project-plan.md           # Overall project plan
│   ├── serial-protocol.md        # Serial communication format
│   └── app-architecture.md       # Mobile app design
└── README.md                     # This file
```

## Documentation

- **[Development Guide](docs/development-guide.md)** ⭐ - Arduino CLI commands and build process
- **[Project Plan](docs/project-plan.md)** - Overall project roadmap and milestones
- **[Serial Protocol](docs/serial-protocol.md)** - ESP32 ↔ Phone communication format
- **[App Architecture](docs/app-architecture.md)** - Mobile app design and features

## Development

### Quick Reference

**All-in-One Command (Recommended):**
```bash
arduino-cli compile --fqbn esp32:esp32:esp32 .; arduino-cli upload -p COM4 --fqbn esp32:esp32:esp32 .; arduino-cli monitor -p COM4 -c baudrate=115200
```

**Individual Commands:**
```bash
# List connected boards (find your port)
arduino-cli board list

# Compile only
arduino-cli compile --fqbn esp32:esp32:esp32 .

# Upload only
arduino-cli upload -p COM4 --fqbn esp32:esp32:esp32 .

# Monitor serial only
arduino-cli monitor -p COM4 -c baudrate=115200
```

### Configuration

- **FQBN**: `esp32:esp32:esp32`
- **Port**: `COM4` (Windows) - check with `arduino-cli board list`
- **Baud Rate**: 115200
- **Sketch File**: `dan-grfs.signal-sniffer.ino` (must match directory name)

### Finding Your Port

```bash
arduino-cli board list
```

Output example:
```
Port Protocol Type              Board Name FQBN Core
COM4 serial   Serial Port (USB) Unknown
```

Use the port shown (e.g., `COM4`, `/dev/ttyUSB0` on Linux, `/dev/cu.usbserial-*` on Mac)

## Features

### WiFi Monitoring
- Promiscuous mode packet capture
- All 2.4 GHz channels (1-11)
- MAC address detection
- Signal strength (RSSI) measurement
- Probe request detection (devices searching for networks)

### Data Collection
- Summary every 10 seconds
- Average RSSI calculation
- Detection count per device
- Timestamp logging (planned)

### Storage
- Flash-based logging (SPIFFS/LittleFS)
- ~37,000-50,000 log entries capacity
- CSV format for easy parsing

## Next Steps

See [Project Plan](docs/project-plan.md) for detailed roadmap.

## Resources

- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- [Arduino CLI Documentation](https://arduino.github.io/arduino-cli/)
- Hardware specs: `.cursor/skills/hardware-esp32-wroom-32u.md`
