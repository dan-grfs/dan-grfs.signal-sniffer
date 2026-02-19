# Tinkering Boilerplate

A boilerplate project template for ESP and Arduino tinkering projects that require AI assistance and CLI build commands.

## Purpose

This boilerplate provides a clean starting point for embedded development projects with:
- Pre-configured structure for ESP32, ESP8266, Arduino, and other compatible boards
- CLI tool support (Arduino CLI, PlatformIO, etc.)
- AI-friendly documentation and skills system
- Best practices for embedded development

## Project Structure

```
.
├── .cursor/              # Cursor IDE configuration
│   └── skills/           # AI skills for hardware and tools
├── docs/                  # Project documentation
├── src/                   # Source code (create as needed)
└── README.md             # This file
```

## Quick Start

1. **Copy this boilerplate** to start a new project
2. **Update this README** with your project-specific information
3. **Create hardware skills** in `.cursor/skills/` for your specific hardware
4. **Configure build tools** (Arduino CLI, PlatformIO, etc.)
5. **Start coding!**

## Development Tools

### Arduino CLI

This boilerplate supports Arduino CLI for command-line development. See `.cursor/skills/cli-tool-arduino-cli.md` for usage.

**Quick Commands:**
```bash
# List connected boards
arduino-cli board list

# Compile a sketch
arduino-cli compile --fqbn <fqbn> <sketch>

# Upload to board
arduino-cli upload -p <port> --fqbn <fqbn> <sketch>

# Monitor serial output
arduino-cli monitor -p <port> -c baudrate=115200
```

### PlatformIO

PlatformIO support can be added by creating a `platformio.ini` file. See PlatformIO documentation for details.

## Skills System

The `.cursor/skills/` directory contains **reusable AI-friendly documentation** that is part of the boilerplate knowledge base:
- **Hardware skills** (`hardware-*.md`): Hardware-specific knowledge, pinouts, and code patterns for various boards and modules
- **CLI tool skills** (`cli-tool-*.md`): Command-line tool usage and workflows

**The skills are part of the boilerplate** - they provide reusable knowledge that any project can leverage. When starting a new project:
1. **Use existing skills** if your hardware is already documented
2. **Create new hardware skills** for hardware not yet covered (e.g., `hardware-esp32-devkit.md`)
3. **Document pin configurations**, voltage levels, and code patterns in hardware skills
4. **Include CLI tool configuration** specific to your hardware in hardware skills

## Documentation

- **Hardware Documentation**: Create hardware-specific docs in `docs/` as needed
- **Setup Guides**: Document setup procedures for your specific hardware
- **Code Examples**: Include examples in hardware skill files

## Best Practices

### Code Guidelines

- Use `F()` macro for string literals to save RAM (ESP32/ESP8266)
- Keep Serial output reasonable for memory-constrained boards
- Always verify pin connections before troubleshooting
- Document pin mappings and configurations
- Mark test/prototype code clearly

### Memory Management

- Use `F()` macro for string literals: `Serial.println(F("Text"));`
- Minimize Serial output on memory-constrained boards
- Use PROGMEM for large constant arrays (AVR)
- Be mindful of stack usage in embedded systems

### Documentation

- Keep README.md updated with project status
- Document hardware-specific information in skills files
- Update documentation when making significant changes
- Include troubleshooting notes for common issues

## Getting Started with Your Project

1. **Identify your hardware** and check if a skill file already exists in `.cursor/skills/`
2. **If no skill exists**, create a new hardware skill file for your board
3. **Set up your development environment** (Arduino IDE, PlatformIO, etc.)
4. **Configure CLI tools** for your board (FQBN, port settings, etc.)
5. **Create your first sketch** in `src/` or project root
6. **Document your project-specific setup** in README.md (hardware skills remain in skills files)

## Next Steps

- [ ] Replace this README with your project-specific information
- [ ] Check if hardware skill files exist for your hardware (they're part of the boilerplate!)
- [ ] Create new hardware skill files if needed for hardware not yet covered
- [ ] Set up build configuration (sketch.yaml, platformio.ini, etc.)
- [ ] Document your project-specific setup in README.md
- [ ] Start coding!

## Resources

- [Arduino CLI Documentation](https://arduino.github.io/arduino-cli/)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- [Arduino Reference](https://www.arduino.cc/reference/en/)
