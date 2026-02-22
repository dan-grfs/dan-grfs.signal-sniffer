# Mobile App Architecture

## Technology Decision

### Option 1: Progressive Web App (PWA) ⭐ Recommended
**Technology**: HTML/CSS/JavaScript with Web Serial API

**Pros:**
- Cross-platform (Android, iOS, desktop)
- No app store submission needed
- Easy to update
- Can work offline (service worker)
- Web Serial API support in Chrome/Edge

**Cons:**
- Requires Chrome/Edge browser (Web Serial API)
- Less native feel
- Limited access to phone features

**Best For:** Quick development, cross-platform support

---

### Option 2: Native Android App
**Technology**: Kotlin/Java with Android SDK

**Pros:**
- Full access to phone features
- Better performance
- Native look and feel
- Can use USB Host API directly

**Cons:**
- Android only
- Requires app store or sideloading
- More complex development
- Longer development time

**Best For:** Long-term solution, Android-only deployment

---

### Recommendation: Start with PWA

Start with a Progressive Web App because:
1. Faster development
2. Works on multiple platforms
3. Easy to test and iterate
4. Can convert to native later if needed

## App Features

### Core Features (MVP)

1. **Connection Management**
   - Detect ESP32 serial port
   - Connect/disconnect
   - Connection status indicator

2. **Time Synchronization**
   - Get phone's current time
   - Send to ESP32
   - Verify sync

3. **Log Download**
   - Request log from ESP32
   - Parse CSV data
   - Display in table/list
   - Export to file

4. **Real-time Monitoring** (Optional for MVP)
   - Stream summary data
   - Live MAC address list
   - Signal strength visualization

### Enhanced Features (Future)

1. **Data Visualization**
   - Signal strength over time
   - MAC address frequency
   - Channel distribution

2. **Filtering & Search**
   - Filter by MAC address
   - Filter by signal strength
   - Search functionality

3. **Statistics**
   - Total devices detected
   - Average signal strength
   - Detection frequency

4. **Export Options**
   - CSV export
   - JSON export
   - Share functionality

## PWA Architecture

### File Structure

```
app/
├── index.html          # Main app page
├── manifest.json       # PWA manifest
├── service-worker.js   # Offline support
├── css/
│   └── style.css      # Styling
├── js/
│   ├── app.js         # Main app logic
│   ├── serial.js      # Serial communication
│   ├── parser.js      # Log parsing
│   └── ui.js          # UI updates
└── assets/
    └── icons/         # App icons
```

### Key Components

#### 1. Serial Communication Module (`serial.js`)

```javascript
class SerialConnection {
  async connect() { /* Open serial port */ }
  async disconnect() { /* Close port */ }
  async sendCommand(cmd) { /* Send command, wait for response */ }
  async readResponse() { /* Read response with timeout */ }
}
```

#### 2. Command Handler (`app.js`)

```javascript
class CommandHandler {
  async setTime() { /* Send SETTIME command */ }
  async getTime() { /* Send GETTIME command */ }
  async getSummary() { /* Send SUMMARY command */ }
  async downloadLog() { /* Send GETLOG command */ }
  async getStatus() { /* Send STATUS command */ }
}
```

#### 3. Log Parser (`parser.js`)

```javascript
class LogParser {
  parseCSV(csvText) { /* Parse CSV log entries */ }
  parseSummary(summaryText) { /* Parse summary format */ }
  formatEntry(entry) { /* Format for display */ }
}
```

#### 4. UI Manager (`ui.js`)

```javascript
class UIManager {
  updateConnectionStatus(status) { /* Update UI */ }
  displayLog(entries) { /* Display log data */ }
  displaySummary(data) { /* Display summary */ }
  showError(message) { /* Show error */ }
}
```

## User Interface Design

### Main Screen

```
┌─────────────────────────────┐
│  WiFi Signal Sniffer        │
├─────────────────────────────┤
│  [Connect] [Disconnect]    │
│  Status: Connected          │
├─────────────────────────────┤
│  [Sync Time]                │
│  Last Sync: 12:34:56        │
├─────────────────────────────┤
│  [Get Summary]              │
│  [Download Log]             │
│  [View Log]                 │
├─────────────────────────────┤
│  Log Entries: 1,234         │
│  Devices: 15                │
└─────────────────────────────┘
```

### Log Viewer Screen

```
┌─────────────────────────────┐
│  ← Back    Log Viewer       │
├─────────────────────────────┤
│  [Filter] [Export]          │
├─────────────────────────────┤
│  Time      MAC          RSSI │
│  12:00:00  aa:bb:cc...  -45  │
│  12:00:10  11:22:33...  -67  │
│  12:00:20  dd:ee:ff...  -52  │
│  ...                         │
└─────────────────────────────┘
```

## Web Serial API Usage

### Basic Connection

```javascript
// Request port
const port = await navigator.serial.requestPort();

// Open with baud rate
await port.open({ baudRate: 115200 });

// Get reader/writer
const reader = port.readable.getReader();
const writer = port.writable.getWriter();
```

### Sending Commands

```javascript
async function sendCommand(command) {
  const encoder = new TextEncoder();
  const data = encoder.encode(command + '\n');
  await writer.write(data);
}
```

### Reading Responses

```javascript
async function readResponse() {
  const decoder = new TextDecoder();
  let buffer = '';
  
  while (true) {
    const { value, done } = await reader.read();
    if (done) break;
    
    buffer += decoder.decode(value);
    if (buffer.includes('\n')) {
      return buffer.trim();
    }
  }
}
```

## Native Android Alternative

If PWA doesn't meet needs, native Android app structure:

### Technology Stack
- **Language**: Kotlin
- **UI**: Jetpack Compose or XML layouts
- **Serial**: USB Host API or Serial library
- **Storage**: Room Database or SharedPreferences

### Key Libraries
- `usb-serial-for-android` - USB serial communication
- `kotlinx-coroutines` - Async operations
- `Material Components` - UI components

## Development Roadmap

### Phase 1: Basic PWA
- [ ] Serial connection
- [ ] Time sync
- [ ] Log download
- [ ] Basic log viewer

### Phase 2: Enhanced UI
- [ ] Better styling
- [ ] Real-time updates
- [ ] Error handling
- [ ] Loading states

### Phase 3: Advanced Features
- [ ] Data visualization
- [ ] Filtering/search
- [ ] Statistics
- [ ] Export options

### Phase 4: Native App (Optional)
- [ ] Convert to native if needed
- [ ] Enhanced features
- [ ] App store submission

## Testing Strategy

1. **Unit Tests**: Command parsing, log parsing
2. **Integration Tests**: Serial communication
3. **Manual Testing**: Real device testing
4. **Browser Testing**: Chrome, Edge compatibility

## Deployment

### PWA Deployment
- Host on GitHub Pages, Netlify, or similar
- Users visit URL, install as PWA
- No app store needed

### Native App Deployment
- Build APK
- Distribute via:
  - Google Play Store
  - Direct APK download
  - F-Droid (open source)

## Notes

- Web Serial API requires HTTPS (or localhost)
- Test on real Android device (not just emulator)
- Handle connection drops gracefully
- Implement timeout for all operations
- Provide clear error messages
