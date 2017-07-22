#ifndef RELAY_H
#define RELAY_H

#include <arduino.h>
#include <SoftwareSerial.h>
#include "device.h"

#define ON  LOW
#define OFF HIGH

class Relay : public device {
public:
  Relay(byte pin, String name, int defcount);
  void set(bool action, int count);
  void setByCmd(String action);
  bool getStatus();
  String getDevice();
  int getCount();
  bool check();

private:
  byte pin;
  String name;
  int defcount;
  int count;
  bool status;
  void setStatus(bool status);
};

#endif
