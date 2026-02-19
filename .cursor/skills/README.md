# Skills Directory

This directory contains **reusable AI-friendly documentation (skills)** that are part of the boilerplate knowledge base. These skills can be used by any project that uses this boilerplate.

## Skill Types

### Hardware Skills (`hardware-*.md`)
Hardware-specific knowledge, pinouts, specifications, and code patterns for various boards and modules. These are **reusable across projects** - they're part of the boilerplate, not project-specific.

**Naming Convention**: `hardware-{hardware-name}.md`

**Example Files**:
- `hardware-esp32-devkit.md`
- `hardware-arduino-uno.md`
- `hardware-sensor-module.md`

**Content Should Include**:
- Hardware identification and specifications
- Pin configurations and capabilities
- Voltage levels and power requirements
- Code patterns specific to that hardware
- Code examples
- CLI tool configuration (FQBN, port settings, etc.)
- Troubleshooting notes

### CLI Tool Skills (`cli-tool-*.md`)
Command-line tool usage, installation, and workflows.

**Naming Convention**: `cli-tool-{tool-name}.md`

**Example Files**:
- `cli-tool-arduino-cli.md`
- `cli-tool-platformio.md`

**Content Should Include**:
- Tool installation instructions
- General commands and syntax
- Common workflows
- General troubleshooting

## Getting Started

1. **Check existing skills** - the boilerplate includes skills for common hardware
2. **Use existing skills** if your hardware is already documented
3. **Create new hardware skills** for hardware not yet covered
4. **Create CLI tool skills** for build tools not yet documented
5. **Keep skills updated** as you learn more about hardware (skills are shared across projects)

## Rules

- All skills MUST follow the naming convention (prefix + name)
- Hardware-specific CLI configuration goes in hardware skills, not CLI tool skills
- One hardware = one skill file
- Skills should be self-contained and comprehensive
- **Skills are reusable** - they're part of the boilerplate knowledge base, not project-specific

## Current Skills in Boilerplate

The boilerplate includes skills for:
- ESP32-WROOM-32U DevKitC (`hardware-esp32-wroom-32u.md`)
- SAMIROB OLED Rotary Encoder Module (`hardware-samirob-rotary-encoder.md`)
- Arduino CLI (`cli-tool-arduino-cli.md`)

Add more skills as you encounter new hardware or tools!

See `.cursorrules` for detailed guidelines.
