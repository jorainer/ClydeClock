// Code and stuff related to the microphone.
// thus far we use only the max signal in the sampling window, but it might be better to use the difference
// max - min in the sampling window, since for a peak we expect this to be large.

#ifdef ENABLE_MIKE

uint8_t mike_the_mic = A3;
uint16_t signal_max = 0;
uint16_t signal_min = 1024;
uint8_t sample_time = 20;              // sample time in ms.
uint16_t NOISE_THRESHOLD = 500;
uint16_t NOISE_THRESHOLD_DIFF = 180;   // the difference between max and min signal to trigger
bool is_loud = false;                  // is true as long as the noise is above threshold.
uint32_t peak_start_millis;            // the time (ms) when the start of a new peak has been detected.
uint32_t last_peak_millis;             // the time when the last peak was detected.
uint16_t PEAK_DURATION = 100;          // the maximal duration (in ms) of a short noise peak (clap).
uint8_t clap_count = 0;                // count claps.
uint16_t PEAK_COUNT_WINDOW = 1000;     // the time window in which we count claps.
uint16_t TRIGGER_DELAY = 500;          // the delay after the last clap that we wait for any potentially other noise peak.
unsigned int sample;

// this function is from adafruit
// https://learn.adafruit.com/adafruit-microphone-amplifier-breakout/measuring-sound-levels
// the question is whether a clap might be too short for the sampling time...
void micSample(){
  signal_max = 0;
  signal_min = 1024;
  uint32_t start_sample = millis();
  while( millis() - start_sample < sample_time ){
    sample = analogRead( mike_the_mic );
    if( sample < 1024 ){
      if( sample > signal_max )
	signal_max = sample;
      if( sample < signal_min )
	signal_min = sample;
    }
  }
}

/*
 * Listen for claps and trigger an event if (more than) one was detected.
 */
void listenForClaps(){
  micSample();
  detectPeakStart();
  detectPeakEnd();
  // ok, now we have counted claps and have recorded the start of the last peak.
  if( clap_count > 1 ){
    // wait for some time before triggering the event.
    if( ( millis() - last_peak_millis ) >= TRIGGER_DELAY ){
#ifdef MIKE_DEBUG
      Serial << "Mike: got more than one peak, triggering an event." << endl;
#endif
      // OK, now do something!
      switch( clap_count ){
      case 2:
	switchLights();
	break;
      case 3:
	sunset();
	break;
      case 4:
	sunrise();
	break;
      }
      clap_count = 0;  // reset clap count.
    }
  }
}


/* how to detect a clap: -> check the ClydeDev TouchyFeely...
 * - measure noise level, keep record of previous level.
 * - if previous noise level was below thresh and current is above -> record clap_start
 * - if previous noise level was above
 */
void detectPeakStart(){
  // use the signal_min and signal_max...
  //  if( signal_max > NOISE_THRESHOLD && !is_loud ){
  if( signal_max - signal_min > NOISE_THRESHOLD_DIFF && !is_loud ){
#ifdef MIKE_DEBUG
    Serial << "Mike: detected peak start. signal: " << signal_max-signal_min << endl;
#endif
    is_loud = true;
    peak_start_millis=millis();
  }
}

// if noise was below threshold
void detectPeakEnd(){
  //  if( signal_max < NOISE_THRESHOLD && is_loud ){
  if( signal_max - signal_min < NOISE_THRESHOLD && is_loud ){
#ifdef MIKE_DEBUG
      Serial << "Mike: detected noise decrease." << endl;
#endif
    is_loud = false;
    if( ( millis() - peak_start_millis ) < PEAK_DURATION ){
#ifdef MIKE_DEBUG
      Serial << "Mike: looks like a peak." << endl;
#endif
      uint32_t current_time = millis();
      // OK, that fits with what we expect from a short noise peak aka clap.
      // now check:
      // - if its the first clap, OK.
      // - if the time between the last peak and this one is
      //   within the defined time window.
      if( clap_count==0 || (current_time - last_peak_millis ) > PEAK_COUNT_WINDOW ){
	clap_count = 1;
	last_peak_millis = current_time;
      }else{
	clap_count++;
	last_peak_millis = current_time;
      }
#ifdef MIKE_DEBUG
      Serial << "Mike: got in total " << clap_count << " peaks." << endl;
#endif
    }
  }
}

#endif
