#include "relay.h"

Relay::Relay(byte pin, String name, int count):pin(pin), name(name),
        count(count), status(false) {
}

void Relay::setByCmd(String action) {
  if (action == "ON") {
    this -> set(ON);
  } else if (action == "OFF") {
    this -> set(OFF);
  } else {
    this -> log(String("Unknown cmd ") + String(action));
  }
}

void Relay::set(bool action) {
  // ON  -> LOW  (false)
  // OFF -> HIGH (true)
  digitalWrite(this -> pin, action);
  this -> setStatus(action);
};

void Relay::setStatus(bool action) {
  this -> status = !action;
  this -> log("Set " + this -> name + " to Status: " + status);
}

bool Relay::getStatus() {
  this -> log(String(this -> name) + String(" Status: ") + String(status));
  return status;
}

String Relay::getDevice() {
  return this -> name;
}

bool Relay::check(int update) {
  if (this -> status) (this -> count)++;
  if (this -> count == update) {
    this -> set(OFF);
    this -> count = 0;
    this -> status = false;
    return true;
  }
  return false;
}
