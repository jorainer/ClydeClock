// Now that's the code related to the HC-SR04 sensor.
// now we're using NewPing:
// https://code.google.com/p/arduino-new-ping/
// connection notes:
// Has to be connected to digital pins (NOT A...)
// Gnd -> GND
// Echo -> 10
// Trig -> 12
// Vcc -> +5V

#ifdef ENABLE_PROXYSENS

#include <NewPing.h>

#define proxy_sens_echo 10           // the pin to which the HC-SR04 echo pin has been attached.
#define proxy_sens_trig 12          // the pin to which the HC-SR04 trigger pin has been attached.
#define proxy_sens_max_distance 300 // the maximum distance.

int proxy_sens_basal_dist = 0;  // the basal, initial distance measured by the sensor.
uint32_t proxy_sens_last_action_triggered = 0;  // keep track of the last triggered action.
//int PROXY_SENS_THRESH = 1;      // the threshold value, if we get closer by PROXY_SENS_THRESH cm we trigger an action.
int PROXY_SENS_TRIGGER_DISTANCE = 4;    // trigger distance in cm. if we get closer we trigger an action.
uint32_t PROXY_SENS_TRIGGER_SLEEP = 1000;  // time in ms we don't trigger another action if one was triggered.
uint32_t PROXY_SENS_PING_SLEEP = 70;        // time in ms we wait between pings.
uint32_t proxy_sens_last_ping = 0;          // last time we sent a ping.

NewPing sonar( proxy_sens_trig, proxy_sens_echo, proxy_sens_max_distance );

// initialize the module:
void initializeProxySens(){
  //pinMode( proxy_sens_trig, OUTPUT );
  //pinMode( proxy_sens_echo, INPUT );
  //proxy_sens_basal_dist = getDistance();
}

int getDistance(){
  digitalWrite( proxy_sens_trig, LOW );
  delayMicroseconds( 2 );
  digitalWrite( proxy_sens_trig, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( proxy_sens_trig, LOW );
  int distance = pulseIn( proxy_sens_echo, HIGH , 100 ) / 58;
  return( distance );
}

// we're reading the current distance, if that's larger than the initial one, than we kind of
// re-calibrate the measurement and return. If it is smaller (i.e. closer) by a certain
// threshold value we trigger an action.
// we may want to wait 50ms between readings, since the waves may echo around for a while...
void updateProxySens(){
  uint32_t ps_current_time = millis();
  if( ( ps_current_time - proxy_sens_last_ping ) < PROXY_SENS_PING_SLEEP ){
    // we return, since we want to have some time between pings.
    return;
  }
  if( ( ps_current_time - proxy_sens_last_action_triggered ) < PROXY_SENS_TRIGGER_SLEEP ){
    // means we will sleep for some time and don't trigger another action.
    return;
  }
  //unsigned int uS = sonar.ping();
  unsigned int uS = sonar.ping_median( 5 );
  proxy_sens_last_ping = ps_current_time;
  int current_distance = (int)( uS / US_ROUNDTRIP_CM );
#ifdef PROXYSENS_DEBUG
  Serial << "ProxySens: distance is: " << current_distance << endl;
#endif
  // simple stuff, if we are closer than 4cm trigger an action.
  if( current_distance < PROXY_SENS_TRIGGER_DISTANCE ){
    switchLights();
    proxy_sens_last_action_triggered = ps_current_time;
  }
}

#endif
