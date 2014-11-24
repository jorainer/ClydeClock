// Now that's the code related to the HC-SR04 sensor.
#ifdef ENABLE_PROXYSENS

uint8_t proxy_sens_trig = ;  // the pin to which the HC-SR04 trigger pin has been attached.
uint8_t proxy_sens_echo = ;  // the pin to which the HC-SR04 echo pin has been attached.
int proxy_sens_basal_dist = 0;  // the basal, initial distance measured by the sensor.
int PROXY_SENS_THRESH = 1;      // the threshold value, if we get closer by PROXY_SENS_THRESH cm we trigger an action.

/*
The idea is pretty simple: we just switch on the sensor in the loop (send signal), wait and receive it again.
 */

// initialize the module:
void initializeProxySens(){
  pinMode( proxy_sens_trig, OUTPUT );
  pinMode( proxy_sens_echo, INPUT );
  proxy_sens_basal_dist = getDistance();
}

int getDistance(){
  digitalWrite( trigPin, LOW );
  delayMicroseconds( 2 );
  digitalWrite( trigPin, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( trigPin, LOW );
  int distance = pulseIn( proxy_sens_echo, HIGH , 100 ) / 58;
  return( distance );
}

// we're reading the current distance, if that's larger than the initial one, than we kind of
// re-calibrate the measurement and return. If it is smaller (i.e. closer) by a certain
// threshold value we trigger an action.
// we may want to wait 50ms between readings, since the waves may echo around for a while...
void updateProxySens(){
  int current_distance = getDistance();
#ifdef PROXYSENS_DEBUG
  Serial << "ProxySens: distance is: " << current_distance << endl;
#endif
  if( current_distance > proxy_sens_basal_dist ){
    proxy_sens_basal_dist = current_distance;
    return;
  }
  if( ( proxy_sens_basal_dist - current_distance ) > PROXY_SENS_THRESH ){
    // OK, now we really got close...
  }
}

#endif
