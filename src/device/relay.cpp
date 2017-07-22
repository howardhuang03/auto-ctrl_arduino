#include "relay.h"

Relay::Relay(byte pin, String name, int defcount):pin(pin), name(name),
        defcount(defcount), count(0), status(false) {
}

void Relay::setByCmd(String msg) {
  int idx = msg.indexOf(',');
  int count = this -> defcount * 60;
  String action = msg;
  if (idx > 0) {
    action = action.substring(0, idx);
    count = msg.substring(idx + 1).toInt() * 60;
  }

  if (action == "ON") {
    this -> set(ON, count);
  } else if (action == "OFF") {
    this -> set(OFF, 0);
  } else {
    this -> log(String("Unknown cmd ") + String(action));
  }
}

void Relay::set(bool action, int count) {
  // ON  -> LOW  (false)
  // OFF -> HIGH (true)
  digitalWrite(this -> pin, action);
  this -> count = count;
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

int Relay::getCount() {
  return this -> count;
}

bool Relay::check() {
  if (this -> status && this -> count != 0) {
    --(this -> count);
    if (this -> count == 0) {
      this -> set(OFF, 0);
      return true;
    }
  }
  return false;
}
