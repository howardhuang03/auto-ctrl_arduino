#include <Wire.h>
#include <EEPROM.h>
#include <UnoWiFiDevEd.h>

#include "device/relay.h"

#define version   "0.0.0"
// Define relay control digital pin
#define RELAY1  8
#define RELAY2  9

// Relay ON/OFF definition
#define ON  LOW   // ON  -> LOW  (false)
#define OFF HIGH  // OFF -> HIGH (true)

#define debugPort Serial

#define cmdTopic "channels/local/cmd"
#define testTopic "channels/local/test"
#define responseTopic "channels/local/response"
#define MQTT "mqtt"

String devName = "EMC";
int update = 30;
bool enable = false;

Relay relay1(RELAY1, "relay1", 0);
Relay relay2(RELAY2, "relay2", 0);

const int arrsize = 2;

void readMqttCmd() {
  CiaoData data = Ciao.read(MQTT, cmdTopic);
  String msg = "";

  while (!data.isEmpty()) {
    msg = data.get(2);
    data = Ciao.read(MQTT, cmdTopic);
  }

  if (msg.length() == 0) return;

  // Parsing string
  int idx1 = msg.indexOf(',');
  //  Search for the next comma just after the first
  int idx2 = msg.indexOf(',', idx1 + 1);

  String name = msg.substring(0, idx1);
  String relay = msg.substring(idx1 + 1, idx2);
  String action = msg.substring(idx2 + 1); // To the end of the string

  if (name != devName) {
    debugPort.println("I'm " + devName + ", not " + name);
    return;
  }

  // Check relay content
  if (!IsNumeric(relay)) {
    debugPort.println("Unknown relay: " + relay);
  }

  // Execute command
  triggerRelay(relay.toInt(), action);
}

boolean IsNumeric(String str) {
  unsigned int stringLength = str.length();
  boolean seenDecimal = false;

  if (stringLength == 0) return false;
  for(unsigned int i = 0; i < stringLength; ++i) {
    if (isDigit(str.charAt(i))) continue;

    if (str.charAt(i) == '.') {
      if (seenDecimal) return false;
      seenDecimal = true;
      continue;
    }
    return false;
  }
  return true;
}

void triggerRelay(int num, String action) {
  if (num > arrsize || num <= 0) {
    debugPort.println("relay number is out of range");
    return;
  }

  /*Relay relay = array[num - 1];
  if (action != "status") {
    relay.setByCmd(action);
  }
  sendMqttMsg(relay);*/
  if (num == 1) {
    relay1.setByCmd(action);
    sendMqttMsg(relay1);
  } else if (num == 2) {
    relay2.setByCmd(action);
    sendMqttMsg(relay2);
  }

}


void sendMqttMsg(Relay relay) {
  String msg = devName + "," + relay.getDevice() + ",";
  if (relay.getStatus()) msg += "ON";
  else msg += "OFF";
  Ciao.write(MQTT, responseTopic, msg);
}

check() {
  if (relay1.check(update)) {
    debugPort.println("Disable " + relay1.getDevice());
    sendMqttMsg(relay1);
  }
  if (relay2.check(update)) {
    debugPort.println("Disable " + relay2.getDevice());
    sendMqttMsg(relay2);
  }
}

// Write flash only
/*void writeDevNum() {
  int addr = 0;
  // Clear EEPROM
  for (addr = 0; addr <= EEPROM.length() ; addr++) {
    EEPROM.write(addr, 0);
  }
  // Write EEPROM
  addr = 0;
  EEPROM.write(addr, 5);
  EEPROM.write(addr + 1, 0);
}*/

String getDevName() {
  byte num = 0;
  String name = devName;
  for (int addr = 0; addr < 2; addr++) {
    num = EEPROM.read(addr);
    name += String(num, 10);
  }
  return name;
}

void setup() {
  debugPort.begin(115200);
  devName = getDevName();
  debugPort.println(devName + " " + version);

  // Init control pins & relay
  relay1.setDebugStream(&debugPort);
  relay2.setDebugStream(&debugPort);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  relay1.set(OFF);
  relay2.set(OFF);

  Ciao.begin();
  Ciao.write(MQTT, testTopic, devName + ", " + version);
}

void loop() {
  readMqttCmd();
  delay(1000);
  check();
}
