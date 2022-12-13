/*
 *   $Id: multicore_demo.ino,v 1.8 2022/12/13 10:07:18 gaijin Exp $
 *
 * Simple demo of "easy" multicore functionality with the RP2040 Pico.
 *
 * This file brings together several of the examples from the
 * Arduino-Pico libraries directory to demonstrate how easy it is
 * to run both cores on the Pico/Pico-W using the included
 * setup1() + loop1() functionality.
 *
 * The setup() and loop() functions control core-0 (the default
 * core used by the Pico if you don't specify anything else), while
 * the setup1() and loop1() functions control core-1.
 *
 * This demo uses core-1 to run the "Fader" demo on the board LED.

 * Meanwhile, core-0 runs the normal WiFi functions, as well as
 * WiFi scans for in-range networks.  The network scans are triggered
 * by the "BOOTSEL" button, which is monitored by core-1.
 *
 */

#include <Arduino.h>
#include <WiFi.h>

void fader();

const char ssid[] = "XXXXXX";	// -Your- WiFi SSID (name) goes here.
const char pass[] = "XXXXXX";	// -Your- WiFi password goes here.

#define TOGGLE 1


/*
 * Format MAC strings.
 */
const char *macToString(uint8_t mac[6]) {
  static char s[20];
  sprintf(s, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return s;
}


/*
 * Show WiFi encryption type.
 */
const char *encToString(uint8_t enc) {
  switch (enc) {
    case ENC_TYPE_NONE: return "NONE";
    case ENC_TYPE_TKIP: return "WPA";
    case ENC_TYPE_CCMP: return "WPA2";
    case ENC_TYPE_AUTO: return "AUTO";
  }
  return "UNKN";
}


/*
 * Run the WiFi-scan loop (on default, core-0).
 */
void scanloop() {
  delay(5000);
  Serial.printf("Beginning scan at %d\r\n", millis());
  auto cnt = WiFi.scanNetworks();
  if (!cnt) {
    Serial.printf("No networks found.\r\n");
  } else {
    Serial.printf("Found %d networks\n\r\n", cnt);
    Serial.printf("%32s %5s %17s %2s %4s\r\n", "SSID", "ENC", "BSSID        ", "CH", "RSSI");
    for (auto i = 0; i < cnt; i++) {
      uint8_t bssid[6];
      WiFi.BSSID(i, bssid);
      Serial.printf("%32s %5s %17s %2d %4d\r\n", WiFi.SSID(i), encToString(WiFi.encryptionType(i)), macToString(bssid), WiFi.channel(i), WiFi.RSSI(i));
    }
  }
  Serial.printf("\n---\r\n");
}


void setup() {
  Serial.begin(115200);

  // Initialize the buit-in LED pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);	// Turn the LED on to show we're alive.

  delay(5000);				// Give the user time to connect to serial.
  digitalWrite(LED_BUILTIN, LOW);	// Okay, that's enough of that.  Use serial, please.

  Serial.println("\n\n\t === Multicore Demo ===\r\n");
  Serial.println("Core-0: On-Line.\r\n");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("\r");

  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
}


/*
 * Core-0 (default) WiFi stuff.
 */
void loop() {
  if (rp2040.fifo.pop() == TOGGLE) {
    Serial.println("\r\nCore-0: Initiating WiFi scan...\r\n");

    /*
     * Request core-1 start the LED fader routine.
     */
    rp2040.fifo.push(TOGGLE);
    scanloop();
    Serial.println("\r\n ==>> Push BOOTSEL button to trigger WiFi scan.\r\n");
 }
}



/*
 * Core-1: Runs the LED "Fader" example code.
 */
void setup1() {
  delay(20000);			// Give core-0 time to connect WiFi.
  Serial.println("Core-1: On-Line.");

  // Ensure built-in LED is declared as an output pin.
  pinMode(LED_BUILTIN, OUTPUT);

  /*
   * The initial BOOTSEL prompt comes from core-1, to ensure
   * that the message is only displayed once this core is on-line.
   */
  Serial.println("\r\n ==>> Push BOOTSEL button to trigger WiFi scan.\r\n");
}

/*
 * Core-1: LED fader code.
 */
void loop1() {

  /*
   * Monitor the BOOTSEL button for presses.
   */
  if (BOOTSEL) {
    // Wait for BOOTSEL to be released
    while (BOOTSEL) {
      delay(1);
    }
    rp2040.fifo.push(TOGGLE);
    Serial.println("\r\nCore-1: Registered button press...\r\n");

    /*
     * If we receive a message from core-0, start the LED fader routine.
     */
    if (rp2040.fifo.pop() == TOGGLE) {
      Serial.println("\r\nCore-1: Received a start-fader command...\r\n");
      fader();
    }
  }
}
