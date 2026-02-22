/*
 * ESP32 WiFi Sniffer - Promiscuous Mode
 * 
 * Collects MAC addresses and signal strength, prints summary every 10 seconds
 */

#include <WiFi.h>
#include <esp_wifi.h>

// WiFi channel to monitor (1-11 for 2.4GHz)
int currentChannel = 1;
const int maxChannel = 11;

// MAC address tracking structure
struct MacEntry {
  uint8_t mac[6];
  long rssiSum;      // Sum of all RSSI values (signed)
  unsigned int count; // Number of times seen
  bool initialized;  // Track if entry is valid
};

// Maximum number of unique MAC addresses to track
const int MAX_MACS = 100;
MacEntry macList[MAX_MACS];
int macCount = 0;

// Check if two MAC addresses are equal
bool macEqual(uint8_t *mac1, uint8_t *mac2) {
  for (int i = 0; i < 6; i++) {
    if (mac1[i] != mac2[i]) return false;
  }
  return true;
}

// Find or add MAC address to the list
void addMacAddress(uint8_t *mac, int8_t rssi) {
  // Validate RSSI before adding
  if (rssi > -20 || rssi < -100) {
    return;  // Skip invalid RSSI values
  }
  
  // Check if MAC already exists
  for (int i = 0; i < macCount; i++) {
    if (macList[i].initialized && macEqual(macList[i].mac, mac)) {
      // Update existing entry - properly handle signed RSSI
      // Cast to long to ensure signed arithmetic
      macList[i].rssiSum += (long)rssi;
      macList[i].count++;
      return;
    }
  }
  
  // Add new MAC if we have space
  if (macCount < MAX_MACS) {
    memcpy(macList[macCount].mac, mac, 6);
    // Initialize with signed long to ensure proper arithmetic
    macList[macCount].rssiSum = (long)rssi;
    macList[macCount].count = 1;
    macList[macCount].initialized = true;
    macCount++;
  }
}

// Promiscuous mode callback function
void promiscuous_rx_cb(void *buf, wifi_promiscuous_pkt_type_t type) {
  wifi_promiscuous_pkt_t *pkt = (wifi_promiscuous_pkt_t *)buf;
  wifi_pkt_rx_ctrl_t *ctrl = &pkt->rx_ctrl;
  
  // Determine packet type
  const uint8_t *frame = pkt->payload;
  uint8_t frameType = (frame[0] & 0x0F);
  uint8_t frameSubtype = (frame[0] >> 4) & 0x0F;
  
  // Extract MAC addresses (positions vary by frame type)
  uint8_t *macAddr1 = (uint8_t *)&frame[10];  // Source or BSSID
  uint8_t *macAddr2 = (uint8_t *)&frame[16];  // Destination or TA
  uint8_t *macAddr3 = (uint8_t *)&frame[4];   // BSSID for some frames
  
  // RSSI (signal strength) - extract and validate
  // The rssi field in wifi_pkt_rx_ctrl_t is already int8_t
  // RSSI for WiFi is typically in range -30 to -90 dBm
  int8_t rssi = ctrl->rssi;
  
  // Validate RSSI is in reasonable range
  // Skip packets with invalid RSSI to prevent bad data accumulation
  if (rssi > -20 || rssi < -100) {
    return;  // Invalid RSSI, skip this packet
  }
  
  // Process different frame types and extract MAC addresses
  if (frameType == 0x08) {  // Management frame
    if (frameSubtype == 0x08) {  // Beacon frame
      // BSSID is at macAddr3
      addMacAddress(macAddr3, rssi);
      
    } else if (frameSubtype == 0x04) {  // Probe Request
      // Source MAC is at macAddr2
      addMacAddress(macAddr2, rssi);
      
    } else {
      // Show source MAC
      addMacAddress(macAddr2, rssi);
    }
    
  } else if (frameType == 0x02) {  // Data frame
    // Show source MAC
    addMacAddress(macAddr2, rssi);
    
  } else if (frameType == 0x01) {  // Control frame
    addMacAddress(macAddr2, rssi);
  }
}

// Helper function to print MAC address
void printMac(uint8_t *mac) {
  for (int i = 0; i < 6; i++) {
    if (i > 0) Serial.print(F(":"));
    if (mac[i] < 16) Serial.print(F("0"));
    Serial.print(mac[i], HEX);
  }
}

// Print summary of all collected MAC addresses
void printSummary() {
  if (macCount == 0) {
    Serial.println(F("No devices detected"));
    return;
  }
  
  Serial.println();
  Serial.println(F("=== Summary ==="));
  Serial.println(F("MAC Address        Avg RSSI  Count"));
  Serial.println(F("-----------------------------------"));
  
  for (int i = 0; i < macCount; i++) {
    printMac(macList[i].mac);
    Serial.print(F("  "));
    
    // Calculate average RSSI - ensure proper signed division
    long avgRssiLong = macList[i].rssiSum / (long)macList[i].count;
    int avgRssi = (int)avgRssiLong;
    
    // Validate average RSSI is in reasonable range
    if (avgRssi < -100 || avgRssi > -20) {
      // Skip invalid entries
      continue;
    }
    
    // Formatting for alignment
    if (avgRssi > -10) Serial.print(F(" "));
    if (avgRssi > -100) Serial.print(F(" "));
    Serial.print(avgRssi);
    Serial.print(F(" dBm  "));
    Serial.println(macList[i].count);
  }
  
  Serial.println(F("==================================="));
  Serial.println();
  
  // Clear the list for next period
  for (int i = 0; i < macCount; i++) {
    macList[i].initialized = false;
    macList[i].rssiSum = 0;
    macList[i].count = 0;
  }
  macCount = 0;
}


void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println(F("WiFi Sniffer - MAC Address Summary"));
  Serial.println(F("Summary printed every 10 seconds"));
  Serial.println();
  
  // Initialize MAC list
  for (int i = 0; i < MAX_MACS; i++) {
    macList[i].initialized = false;
    macList[i].rssiSum = 0;
    macList[i].count = 0;
  }
  macCount = 0;
  
  // Disconnect any existing WiFi connection
  WiFi.disconnect(true);
  delay(100);
  
  // Set WiFi to station mode
  WiFi.mode(WIFI_STA);
  
  // Initialize promiscuous mode
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&promiscuous_rx_cb);
  
  // Set initial channel
  esp_wifi_set_channel(currentChannel, WIFI_SECOND_CHAN_NONE);
}

void loop() {
  static unsigned long lastSummary = 0;
  static unsigned long lastChannelChange = 0;
  unsigned long currentTime = millis();
  
  // Print summary every 10 seconds
  if (currentTime - lastSummary >= 10000) {
    lastSummary = currentTime;
    printSummary();
  }
  
  // Cycle through channels every 10 seconds (same timing as summary)
  if (currentTime - lastChannelChange >= 10000) {
    lastChannelChange = currentTime;
    
    currentChannel++;
    if (currentChannel > maxChannel) {
      currentChannel = 1;
    }
    
    esp_wifi_set_channel(currentChannel, WIFI_SECOND_CHAN_NONE);
  }
  
  delay(100);
}
