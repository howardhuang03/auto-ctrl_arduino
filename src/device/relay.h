#ifndef RELAY_H
#define RELAY_H

#include <arduino.h>
#include <SoftwareSerial.h>
#include "device.h"

#define ON  LOW
#define OFF HIGH

class Relay : public device {
public:
  Relay(byte pin, String name, int count);
  void set(bool action);
  void setByCmd(String action);
  bool getStatus();
  String getDevice();
  bool check(int update);

private:
  int count;
  byte pin;
  String name;
  bool status;
  void setStatus(bool status);
};

#endif
