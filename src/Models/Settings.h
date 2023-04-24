#pragma once
#include "Arduino.h"

struct Settings {
  unsigned int sleepDurationSeconds;
};

const Settings DEFAULT_SETTINGS = {
  .sleepDurationSeconds = 1200
};
