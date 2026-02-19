# Arduino CLI Tool Reference

## Purpose
Command-line interface for Arduino development. Use this when compiling, uploading, or managing Arduino projects from the terminal instead of the Arduino IDE.

## Installation

### Windows
1. **Chocolatey** (if installed):
   ```powershell
   choco install arduino-cli
   ```

2. **Scoop** (if installed):
   ```powershell
   scoop install arduino-cli
   ```

3. **Manual Installation**:
   - Download from: https://github.com/arduino/arduino-cli/releases/latest
   - Download: `arduino-cli_X.X.X_Windows_64bit.zip`
   - Extract and add to PATH

4. **Using Go** (if installed):
   ```powershell
   go install github.com/arduino/arduino-cli@latest
   ```

### Verification
```bash
arduino-cli version
```

## Initial Setup

### Initialize Configuration
```bash
arduino-cli config init
```
Creates config file at: `%LOCALAPPDATA%\Arduino15\arduino-cli.yaml` (Windows)

### Add Board Manager URLs
```bash
arduino-cli config add board_manager.additional_urls <URL>
```

### Update Board Index
```bash
arduino-cli core update-index
```

## Core Management

### List Installed Cores
```bash
arduino-cli core list
```

### Install Core
```bash
arduino-cli core install <core-id>
```

### Update Core
```bash
arduino-cli core update-index
arduino-cli core upgrade <core-id>
```

### Uninstall Core
```bash
arduino-cli core uninstall <core-id>
```

## Board Management

### List Connected Boards
```bash
arduino-cli board list
```
Shows connected boards with their ports and FQBN (Fully Qualified Board Name).

### List All Available Boards
```bash
arduino-cli board listall
```

### List Boards by Core
```bash
arduino-cli board listall <core-id>
```

### Get Board Details
```bash
arduino-cli board details -b <fqbn>
```

## Compilation

### Basic Compile
```bash
arduino-cli compile --fqbn <fqbn> <sketch-path>
```

### Compile with Options
```bash
arduino-cli compile --fqbn <fqbn> --build-property <key>=<value> <sketch-path>
```

### Compile Examples
```bash
# Compile with verbose output
arduino-cli compile --fqbn <fqbn> --verbose <sketch-path>

# Compile and show build path
arduino-cli compile --fqbn <fqbn> --build-path <path> <sketch-path>

# Compile with warnings
arduino-cli compile --fqbn <fqbn> --warnings all <sketch-path>
```

## Upload

### Basic Upload
```bash
arduino-cli upload -p <port> --fqbn <fqbn> <sketch-path>
```

### Upload with Options
```bash
# Upload with verbose output
arduino-cli upload -p <port> --fqbn <fqbn> --verbose <sketch-path>

# Upload with specific programmer
arduino-cli upload -p <port> --fqbn <fqbn> --programmer <programmer> <sketch-path>
```

### Upload Pre-compiled Binary
```bash
arduino-cli upload -p <port> --fqbn <fqbn> --input-file <binary-file>
```

## Serial Monitor

### Basic Monitor
```bash
arduino-cli monitor -p <port>
```

### Monitor with Configuration
```bash
# Set baud rate
arduino-cli monitor -p <port> -c baudrate=115200

# Set line ending
arduino-cli monitor -p <port> -c line_ending=LF

# Enable echo
arduino-cli monitor -p <port> -c echo=on

# Enable timestamps
arduino-cli monitor -p <port> -c timestamp=on

# Combined options
arduino-cli monitor -p <port> -c baudrate=115200 -c line_ending=LF -c echo=on -c timestamp=on
```

### Monitor Configuration Options
- `baudrate`: Serial baud rate (e.g., 9600, 115200)
- `line_ending`: Line ending (LF, CR, CRLF, NL)
- `echo`: Enable/disable echo (on/off)
- `timestamp`: Enable/disable timestamps (on/off)

## Library Management

### Search Libraries
```bash
arduino-cli lib search <library-name>
```

### Install Library
```bash
arduino-cli lib install <library-name>
```

### Install Specific Version
```bash
arduino-cli lib install <library-name>@<version>
```

### List Installed Libraries
```bash
arduino-cli lib list
```

### Update Libraries
```bash
arduino-cli lib update-index
arduino-cli lib upgrade
```

### Uninstall Library
```bash
arduino-cli lib uninstall <library-name>
```

## Sketch Management

### Create New Sketch
```bash
arduino-cli sketch new <sketch-name>
```

### Archive Sketch
```bash
arduino-cli sketch archive <sketch-path>
```

### Check Sketch
```bash
arduino-cli sketch check <sketch-path>
```

## Configuration

### View Configuration
```bash
arduino-cli config dump
```

### Set Configuration Value
```bash
arduino-cli config set <key> <value>
```

### Remove Configuration Value
```bash
arduino-cli config remove <key>
```

### Sketch Configuration File (sketch.yaml)

Arduino CLI supports a `sketch.yaml` file in the sketch directory to store default settings. This allows you to use simplified commands without specifying FQBN and port each time.

**Create `sketch.yaml` in your sketch directory:**
```yaml
# Arduino CLI Sketch Configuration
# This file stores default settings for this sketch
# You can now use simple commands like:
#   arduino-cli compile <sketch-name>.ino
#   arduino-cli upload <sketch-name>.ino

default_fqbn: <package>:<architecture>:<board>
default_port: <port>
default_protocol: serial
```

**Example:**
```yaml
default_fqbn: esp32:esp32:esp32
default_port: COM4
default_protocol: serial
```

**Benefits:**
- No need to specify `--fqbn` and `-p` flags every time
- Project-specific settings stored with the code
- Easier to share project setup with others
- Simpler commands: `arduino-cli compile sketch.ino` instead of `arduino-cli compile --fqbn esp32:esp32:esp32 sketch.ino`

**Usage:**
Once `sketch.yaml` is created, you can use simplified commands:
```bash
# Compile (uses default_fqbn from sketch.yaml)
arduino-cli compile sketch.ino

# Upload (uses default_fqbn and default_port from sketch.yaml)
arduino-cli upload sketch.ino

# Monitor (still need to specify port, or use default_port)
arduino-cli monitor -p COM4 -c baudrate=115200
```

**Note**: Hardware-specific FQBN values and port settings should be documented in the hardware skill file, not here. This section explains the general concept of sketch.yaml files.

## Common Workflows

### Complete Workflow: Compile, Upload, Monitor
```bash
# Compile
arduino-cli compile --fqbn <fqbn> <sketch-path>

# Upload
arduino-cli upload -p <port> --fqbn <fqbn> <sketch-path>

# Monitor
arduino-cli monitor -p <port> -c baudrate=115200
```

### One-Line Compile and Upload
```bash
arduino-cli compile --fqbn <fqbn> <sketch-path> && \
arduino-cli upload -p <port> --fqbn <fqbn> <sketch-path>
```

### Compile, Upload, and Monitor Chain
```bash
arduino-cli compile --fqbn <fqbn> <sketch-path> && \
arduino-cli upload -p <port> --fqbn <fqbn> <sketch-path> && \
arduino-cli monitor -p <port> -c baudrate=115200
```

## FQBN (Fully Qualified Board Name) Format

FQBN format: `<package>:<architecture>:<board>[:<options>]`

Examples:
- `arduino:avr:uno` - Arduino Uno
- `esp32:esp32:esp32` - ESP32 Dev Module
- `esp32:esp32:esp32s3` - ESP32-S3 Dev Module

## Troubleshooting

### Board Not Detected
1. Check USB connection
2. Verify drivers are installed
3. Check port with `arduino-cli board list`
4. Try different USB port/cable

### Compilation Errors
1. Verify FQBN is correct: `arduino-cli board listall`
2. Check core is installed: `arduino-cli core list`
3. Verify libraries are installed: `arduino-cli lib list`
4. Use `--verbose` flag for detailed error messages

### Upload Failures
1. Verify port is correct: `arduino-cli board list`
2. Check board is in bootloader mode (may need to hold Boot button)
3. Try different upload speed
4. Check USB cable (data cable, not charge-only)

### Serial Monitor Issues
1. Verify baud rate matches sketch (`Serial.begin()`)
2. Check port is not in use by another program
3. Close other serial monitors/IDEs

## Useful Commands Reference

```bash
# Check version
arduino-cli version

# Get help for any command
arduino-cli <command> --help

# List all commands
arduino-cli help

# Check configuration
arduino-cli config dump

# Update everything
arduino-cli core update-index
arduino-cli lib update-index
```

## When to Use This Skill

Use this skill when:
- Compiling Arduino sketches from command line
- Uploading code to boards via terminal
- Monitoring serial output from terminal
- Managing Arduino cores and libraries via CLI
- Automating Arduino builds (CI/CD)
- Working without Arduino IDE GUI
- Creating build scripts for Arduino projects
- Setting up development workflows

## Integration with Other Tools

- **Makefiles**: Use Arduino CLI commands in Makefile targets
- **CI/CD**: Integrate into GitHub Actions, GitLab CI, etc.
- **Scripts**: Create batch/shell scripts for common workflows
- **IDEs**: Some IDEs can use Arduino CLI as backend
