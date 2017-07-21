#include "device.h"

void device::setDebugStream(Stream *stream) {
    this->debugStream = stream;
}

template<typename T>
void device::log(T s) {
    if (this->debugStream != NULL) {
        this->debugStream->println(s);
    }
}

template void device::log<>(StringSumHelper);
template void device::log<>(String);
template void device::log<>(const char*);
