# Project Plan - WiFi Signal Sniffer

## Project Goals

Create a portable WiFi monitoring device that:
1. Passively monitors all WiFi signals in range
2. Logs MAC addresses, signal strength, and timestamps
3. Connects to Android phone via USB for configuration and data retrieval
4. Provides a mobile app interface for time sync and log download

## Architecture Overview

```
┌─────────────┐         USB Serial           ┌───────────────┐
│   ESP32     │◄────────────────────────────►│ Android Phone │
│  Firmware   │         (OTG Cable)          │  Web/App      │
│             │                              │               │
│ - WiFi      │                              │ - Time Sync   │
│   Sniffer   │                              │ - Log Viewer  │
│ - Data      │                              │ - Download    │
│   Logger    │                              │   Logs        │
│ - Serial    │                              │               │
│   Protocol  │                              │               │
└─────────────┘                              └───────────────┘
```

## Development Phases

### Phase 1: Core Firmware ✅ (Complete)
- [x] Promiscuous mode WiFi capture
- [x] MAC address extraction
- [x] RSSI measurement
- [x] Summary output every 10 seconds
- [x] Channel scanning

### Phase 2: Data Logging 🚧 (In Progress)
- [ ] Initialize SPIFFS/LittleFS
- [ ] Timestamp implementation (manual set or NTP)
- [ ] CSV log file format
- [ ] Append log entries
- [ ] Log rotation/management

### Phase 3: Serial Protocol 🚧 (Next)
- [ ] Command parser
- [ ] Response format
- [ ] Time sync command
- [ ] Log download command
- [ ] Status/configuration commands

### Phase 4: Mobile App 📋 (Planned)
- [ ] Web app or native Android app decision
- [ ] Serial communication library
- [ ] Time sync interface
- [ ] Log viewer/parser
- [ ] CSV download functionality
- [ ] Real-time data display

## Technical Decisions

### Serial Communication
- **Format**: Text-based commands (human-readable, easy to debug)
- **Trigger**: Phone-initiated (request/response model)
- **Baud Rate**: 115200 (standard for ESP32)
- **Protocol**: See [Serial Protocol](serial-protocol.md)

### Data Storage
- **Format**: CSV files in SPIFFS/LittleFS
- **Structure**: `timestamp,mac_address,avg_rssi,count`
- **Capacity**: ~37K-50K entries (~4-5 days at 10s intervals)
- **File Naming**: `log_YYYYMMDD.csv` or single `log.csv` with rotation

### Time Management
- **Initial**: Manual set via serial command from phone
- **Future**: NTP sync if WiFi connection available
- **Fallback**: Relative time (millis since boot) if no time set

### App Technology
- **Decision**: Web app (PWA) vs Native Android
- **Pros Web**: Cross-platform, easier development, no app store
- **Pros Native**: Better serial access, offline capability
- **Recommendation**: Start with web app, consider native if needed

## Milestones

### Milestone 1: Data Logging (Current)
**Target**: End of current sprint
- Flash storage working
- Timestamps in logs
- CSV format implemented

### Milestone 2: Serial Protocol
**Target**: Next sprint
- All commands implemented
- Time sync working
- Log download functional

### Milestone 3: Basic App
**Target**: Following sprint
- Web app connects via serial
- Can set time
- Can download logs
- Basic log viewer

### Milestone 4: Enhanced Features
**Target**: Future
- Real-time data streaming
- Log filtering/search
- Statistics/charts
- Export options

## Risk Assessment

### Technical Risks
1. **Serial Communication Reliability**
   - Mitigation: Robust error handling, checksums
   
2. **Flash Wear**
   - Mitigation: Log rotation, wear leveling in SPIFFS

3. **Phone OTG Compatibility**
   - Mitigation: Tested and working, fallback to Bluetooth if needed

4. **Web Serial API Browser Support**
   - Mitigation: Check compatibility, consider native app if needed

### Project Risks
1. **Scope Creep**
   - Mitigation: Focus on core features first, iterate

2. **Time Management**
   - Mitigation: Phased approach, MVP first

## Success Criteria

### MVP (Minimum Viable Product)
- [ ] ESP32 logs data to flash
- [ ] Phone can set time via serial
- [ ] Phone can download logs via serial
- [ ] Basic log viewer/parser

### Full Product
- [ ] All MVP features
- [ ] Real-time data display
- [ ] Log statistics and charts
- [ ] Export to various formats
- [ ] User-friendly interface

## Next Actions

1. **Immediate**: Implement SPIFFS and timestamp logging
2. **Short-term**: Design and implement serial protocol
3. **Medium-term**: Build web app prototype
4. **Long-term**: Enhance features and polish

## Notes

- All code should use `F()` macro for strings to save RAM
- Serial communication should be robust with error handling
- Log format should be easily parseable (CSV)
- Consider power consumption for battery operation (future)
