#include <WiFi.h>
#include "time.h"

void configTime();

bool now(struct tm* timeInfo);

bool nowEpoch(time_t* epoch);