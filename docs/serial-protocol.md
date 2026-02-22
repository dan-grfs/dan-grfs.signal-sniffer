# Serial Communication Protocol

## Overview

Text-based command protocol for communication between ESP32 and Android phone/web app via USB serial.

- **Baud Rate**: 115200
- **Format**: ASCII text, newline-terminated (`\n`)
- **Model**: Request/Response (phone initiates, ESP32 responds)
- **Encoding**: UTF-8

## Command Format

### General Structure

```
COMMAND [PARAMETERS]\n
```

- Commands are case-insensitive
- Parameters separated by spaces or commas
- Responses end with newline
- Empty lines are ignored

## Commands

### 1. Set Time

Set the current time on ESP32.

**Request:**
```
SETTIME <timestamp>\n
```

**Parameters:**
- `<timestamp>`: Unix timestamp (seconds since epoch)

**Example:**
```
SETTIME 1704067200\n
```

**Response:**
```
OK TIME SET\n
```

**Error Response:**
```
ERROR INVALID_TIMESTAMP\n
```

---

### 2. Get Time

Get current time from ESP32.

**Request:**
```
GETTIME\n
```

**Response:**
```
TIME <timestamp>\n
```

**Example:**
```
TIME 1704067200\n
```

---

### 3. Get Status

Get device status and statistics.

**Request:**
```
STATUS\n
```

**Response:**
```
STATUS
CHANNEL:<channel>
MAC_COUNT:<count>
LOG_SIZE:<bytes>
FREE_SPACE:<bytes>
UPTIME:<seconds>
\n
```

**Example:**
```
STATUS
CHANNEL:6
MAC_COUNT:15
LOG_SIZE:12345
FREE_SPACE:1500000
UPTIME:3600
\n
```

---

### 4. Download Log

Download log file contents.

**Request:**
```
GETLOG [<start_line>] [<end_line>]\n
```

**Parameters:**
- `<start_line>`: Optional, line number to start from (1-indexed)
- `<end_line>`: Optional, line number to end at (1-indexed)
- If omitted, returns entire log

**Response:**
```
LOG_START
<line 1>
<line 2>
...
LOG_END <total_lines>\n
```

**Example:**
```
GETLOG\n
LOG_START
1704067200,aa:bb:cc:dd:ee:ff,-45,23
1704067210,11:22:33:44:55:66,-67,5
LOG_END 2
\n
```

**Error Response:**
```
ERROR LOG_NOT_FOUND\n
ERROR INVALID_RANGE\n
```

---

### 5. Clear Log

Clear/delete log file.

**Request:**
```
CLEARLOG\n
```

**Response:**
```
OK LOG CLEARED\n
```

**Error Response:**
```
ERROR CLEAR_FAILED\n
```

---

### 6. Get Summary

Get current summary (same as 10-second output).

**Request:**
```
SUMMARY\n
```

**Response:**
```
SUMMARY_START
<mac_address> <avg_rssi> <count>
<mac_address> <avg_rssi> <count>
...
SUMMARY_END <count>\n
```

**Example:**
```
SUMMARY\n
SUMMARY_START
aa:bb:cc:dd:ee:ff -45 23
11:22:33:44:55:66 -67 5
SUMMARY_END 2
\n
```

---

### 7. Set Channel

Manually set WiFi channel (1-11).

**Request:**
```
SETCHANNEL <channel>\n
```

**Parameters:**
- `<channel>`: Channel number (1-11)

**Response:**
```
OK CHANNEL <channel>\n
```

**Error Response:**
```
ERROR INVALID_CHANNEL\n
```

---

### 8. Get Channel

Get current WiFi channel.

**Request:**
```
GETCHANNEL\n
```

**Response:**
```
CHANNEL <channel>\n
```

---

### 9. Enable/Disable Logging

Control whether data is logged to flash.

**Request:**
```
SETLOGGING <on|off>\n
```

**Response:**
```
OK LOGGING <on|off>\n
```

---

### 10. Get Logging Status

Check if logging is enabled.

**Request:**
```
GETLOGGING\n
```

**Response:**
```
LOGGING <on|off>\n
```

---

## Data Formats

### Log Entry Format (CSV)

```
<timestamp>,<mac_address>,<avg_rssi>,<count>
```

**Fields:**
- `timestamp`: Unix timestamp (seconds)
- `mac_address`: MAC address in format `aa:bb:cc:dd:ee:ff`
- `avg_rssi`: Average RSSI in dBm (integer)
- `count`: Number of detections

**Example:**
```
1704067200,aa:bb:cc:dd:ee:ff,-45,23
```

### Summary Format

```
<mac_address> <avg_rssi> <count>
```

**Fields:**
- `mac_address`: MAC address in format `aa:bb:cc:dd:ee:ff`
- `avg_rssi`: Average RSSI in dBm (integer, can be negative)
- `count`: Number of detections (integer)

**Example:**
```
aa:bb:cc:dd:ee:ff -45 23
```

## Error Handling

### Error Response Format

```
ERROR <error_code>\n
```

**Common Error Codes:**
- `INVALID_COMMAND` - Unknown command
- `INVALID_PARAMETER` - Invalid parameter format
- `INVALID_TIMESTAMP` - Invalid timestamp value
- `INVALID_CHANNEL` - Channel out of range (1-11)
- `LOG_NOT_FOUND` - Log file doesn't exist
- `INVALID_RANGE` - Invalid line range for GETLOG
- `CLEAR_FAILED` - Failed to clear log
- `STORAGE_ERROR` - Flash storage error
- `NOT_READY` - Device not ready (e.g., during initialization)

## Implementation Notes

### ESP32 Side
- Parse commands in `loop()` or dedicated task
- Use `Serial.available()` to check for incoming data
- Read line-by-line using `Serial.readStringUntil('\n')`
- Trim whitespace from commands
- Case-insensitive command matching
- Validate all parameters before execution

### Phone/App Side
- Send commands with newline terminator
- Wait for response (with timeout)
- Parse multi-line responses
- Handle errors gracefully
- Implement retry logic for critical operations

## Example Communication Flow

### Setting Time

```
Phone:  SETTIME 1704067200\n
ESP32:  OK TIME SET\n
```

### Getting Summary

```
Phone:  SUMMARY\n
ESP32:  SUMMARY_START\n
ESP32:  aa:bb:cc:dd:ee:ff -45 23\n
ESP32:  11:22:33:44:55:66 -67 5\n
ESP32:  SUMMARY_END 2\n
```

### Downloading Log

```
Phone:  GETLOG\n
ESP32:  LOG_START\n
ESP32:  1704067200,aa:bb:cc:dd:ee:ff,-45,23\n
ESP32:  1704067210,11:22:33:44:55:66,-67,5\n
ESP32:  LOG_END 2\n
```

## Future Extensions

Potential future commands:
- `SETINTERVAL <seconds>` - Change summary interval
- `GETCONFIG` - Get all configuration
- `SETCONFIG <key> <value>` - Set configuration
- `STREAM <on|off>` - Enable/disable real-time streaming
- `RESET` - Reset device

## Testing

Test each command:
1. Send command via serial terminal
2. Verify response format
3. Test error cases (invalid parameters)
4. Test edge cases (empty log, full storage, etc.)
