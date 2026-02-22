# Development Guide

## Arduino CLI Quick Reference

### One-Command Build, Upload, and Monitor

**PowerShell (Windows):**
```bash
arduino-cli compile --fqbn esp32:esp32:esp32 .; arduino-cli upload -p COM4 --fqbn esp32:esp32:esp32 .; arduino-cli monitor -p COM4 -c baudrate=115200
```

**Bash/Linux/Mac:**
```bash
arduino-cli compile --fqbn esp32:esp32:esp32 . && arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 . && arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=115200
```

**What it does:**
1. Compiles the sketch
2. Uploads to ESP32
3. Opens serial monitor to view output

**Note:** Replace `COM4` (Windows) or `/dev/ttyUSB0` (Linux) with your actual port.

---

## Finding Your Port

### List Connected Boards

```bash
arduino-cli board list
```

**Windows Output:**
```
Port Protocol Type              Board Name FQBN Core
COM4 serial   Serial Port (USB) Unknown
```

**Linux Output:**
```
Port         Protocol Type              Board Name FQBN Core
/dev/ttyUSB0 serial   Serial Port (USB) Unknown
```

**Mac Output:**
```
Port                    Protocol Type              Board Name FQBN Core
/dev/cu.usbserial-1410 serial   Serial Port (USB) Unknown
```

Use the port shown in the output.

---

## Individual Commands

### Compile Only

```bash
arduino-cli compile --fqbn esp32:esp32:esp32 .
```

**What it does:**
- Compiles the sketch
- Shows memory usage
- Creates binary files
- Does NOT upload

**Use when:**
- Checking for compilation errors
- Verifying code changes
- Before uploading

---

### Upload Only

```bash
arduino-cli upload -p COM4 --fqbn esp32:esp32:esp32 .
```

**What it does:**
- Uploads compiled binary to ESP32
- Erases flash as needed
- Verifies upload

**Use when:**
- Code already compiled
- Just need to update firmware
- Testing upload process

**Note:** Must compile first or use the all-in-one command.

---

### Monitor Serial

```bash
arduino-cli monitor -p COM4 -c baudrate=115200
```

**What it does:**
- Opens serial monitor
- Shows serial output from ESP32
- Allows sending commands (if implemented)

**Use when:**
- Viewing debug output
- Testing serial communication
- Monitoring device activity

**Exit:** Press `Ctrl+C` to stop monitoring.

---

## Configuration

### Board Configuration (sketch.yaml)

The project includes `sketch.yaml` with default settings:

```yaml
default_fqbn: esp32:esp32:esp32
default_port: COM4
default_protocol: serial

monitor:
  baudrate: 115200
```

**Settings:**
- **FQBN**: `esp32:esp32:esp32` (ESP32 DevKit)
- **Port**: `COM4` (update to match your port)
- **Baud Rate**: 115200 (standard for ESP32)

---

## Common Workflows

### First Time Setup

1. **Find your port:**
   ```bash
   arduino-cli board list
   ```

2. **Update sketch.yaml** with your port (optional)

3. **Build and upload:**
   ```bash
   arduino-cli compile --fqbn esp32:esp32:esp32 .; arduino-cli upload -p COM4 --fqbn esp32:esp32:esp32 .; arduino-cli monitor -p COM4 -c baudrate=115200
   ```

### Development Cycle

1. **Edit code** in `dan-grfs.signal-sniffer.ino`

2. **Build, upload, monitor:**
   ```bash
   arduino-cli compile --fqbn esp32:esp32:esp32 .; arduino-cli upload -p COM4 --fqbn esp32:esp32:esp32 .; arduino-cli monitor -p COM4 -c baudrate=115200
   ```

3. **Test and iterate**

### Quick Compile Check

```bash
arduino-cli compile --fqbn esp32:esp32:esp32 .
```

Use this to check for errors without uploading.

### Monitor Only (Device Already Running)

```bash
arduino-cli monitor -p COM4 -c baudrate=115200
```

Use this when device is already running and you just want to see output.

---

## Troubleshooting

### Port Not Found

**Error:** `Error getting port metadata: port not found: COM4`

**Solution:**
1. Check port with `arduino-cli board list`
2. Update command with correct port
3. Ensure ESP32 is connected via USB
4. Try unplugging and replugging USB cable

### Upload Failed

**Error:** `Failed to connect to ESP32`

**Solution:**
1. Hold BOOT button on ESP32
2. Press and release RESET button
3. Release BOOT button
4. Try upload again immediately

### Compilation Errors

**Error:** Various compilation errors

**Solution:**
1. Check Arduino CLI is installed: `arduino-cli version`
2. Check ESP32 board package is installed: `arduino-cli core list`
3. Install if needed: `arduino-cli core install esp32:esp32`
4. Check sketch file name matches directory name

### Serial Monitor Shows Garbage

**Error:** Random characters in serial output

**Solution:**
1. Check baud rate is 115200
2. Ensure ESP32 code uses `Serial.begin(115200)`
3. Try disconnecting and reconnecting

---

## Platform-Specific Notes

### Windows

- Ports are `COM1`, `COM2`, `COM3`, etc.
- May need CP2102 drivers for ESP32
- Use PowerShell for `;` command separator

### Linux

- Ports are `/dev/ttyUSB0`, `/dev/ttyUSB1`, etc.
- May need to add user to `dialout` group:
  ```bash
  sudo usermod -a -G dialout $USER
  ```
- Log out and back in for group change to take effect
- Use `&&` for command chaining

### Mac

- Ports are `/dev/cu.usbserial-*` or `/dev/tty.usbserial-*`
- Use `/dev/cu.*` for serial communication
- May need CP2102 drivers
- Use `&&` for command chaining

---

## Advanced Usage

### Verbose Compilation

```bash
arduino-cli compile --fqbn esp32:esp32:esp32 --verbose .
```

Shows detailed compilation output.

### Custom Build Options

```bash
arduino-cli compile --fqbn esp32:esp32:esp32 --build-property "build.flash_size=4MB" .
```

### Upload with Verification

```bash
arduino-cli upload -p COM4 --fqbn esp32:esp32:esp32 --verify .
```

Verifies upload after writing.

---

## Tips

1. **Use the all-in-one command** for quick iteration
2. **Check port first** if having connection issues
3. **Monitor separately** if device is already running
4. **Compile first** to catch errors before upload
5. **Keep serial monitor open** to see debug output

---

## Related Documentation

- [Project Plan](project-plan.md)
- [Serial Protocol](serial-protocol.md)
- [App Architecture](app-architecture.md)
- Main [README.md](../README.md)
