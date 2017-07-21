#ifndef DEVICE_H
#define DEVICE_H

#include "arduino.h"

class device {
public:
    void setDebugStream(Stream *stream);
    template<typename T> void log(T s);

protected:
    Stream *debugStream;
};

#endif
