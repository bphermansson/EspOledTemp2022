#include <Arduino.h>
#include "internetTime.h"
#include "../../settings.h"

void getInternetTime(mytime_t& mytime)
{
    time_t now;                         // this is the epoch
    tm tm;                              // the structure tm holds time information in a more convenient way

    configTime(TZ, NTPSERVER); 
    delay(500);

    time(&now);                       // read the current time
    localtime_r(&now, &tm);           // update the structure tm with the current time
    int year = tm.tm_year + 1900;
    int month = tm.tm_mon + 1;
    int day = tm.tm_mday;
    int hour = tm.tm_hour;
    int min = tm.tm_min;
    int seconds = tm.tm_sec;
    char timedatebuf[50];
    char curTime[20];
    char curDate[20];
    sprintf(timedatebuf, "%02d-%02d-%02dT%02d:%02d:%02d", year, month, day, hour, min, seconds); 
    sprintf(curTime, "%02d:%02d:%02d", hour, min, seconds);
    sprintf(curDate, "%02d:%02d:%02d", year, month, day);

    strcpy(mytime.readable_date, timedatebuf);
    strcpy(mytime.time, curTime);
    strcpy(mytime.date, curDate);
    
    mytime.cur_timestamp = millis();
    mytime.raw_time = now;

}
