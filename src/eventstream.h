#pragma once

#include "event1.h"

class EventOutStream {
public:
  virtual void PushEvent(event const &ev) = 0;

  virtual ~EventOutStream(){};
};

class EventInStream {
public:
  virtual bool ReadEvent(event &ev) = 0;

  virtual ~EventInStream(){};
};