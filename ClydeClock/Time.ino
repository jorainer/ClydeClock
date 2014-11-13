// contains Time and TimeAlarms related functionality.
#ifdef ENABLE_CLOCK

#include <Time.h>
#include <TimeAlarms.h>

void initializeClock(){
  // set the time manually...
  setTime( 20, 3, 0, 1, 1, 11 );
  // start after 20 sec.
  //Alarm.timerOnce( 20, sunrise );
  // set the sunrise to start at hour, minute, second.
  Alarm.alarmRepeat( 6, 15, 0, sunrise );
}

void updateClock(){
  Alarm.serviceAlarms();
}


#endif
