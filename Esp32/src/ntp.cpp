#include "ntp.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600; // CEST time
const int   daylightOffset_sec = 3600;

void configTime() {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

bool now(struct tm* timeInfo) {
    return getLocalTime(timeInfo);
}

bool nowEpoch(time_t* epoch) {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return false;
    }
    time(epoch);
    
    return true;
}