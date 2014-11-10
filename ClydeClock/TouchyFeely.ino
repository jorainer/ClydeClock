// note, cycleRGB is defined in Behaviour.ino
// Update: we require also a "double-touch" to avoid false positive triggers.

// multitouch:
// - use an array to record which legs have been touched
// - use a trigger delay after which an action will be triggered; depending on the legs
//   that have been touched. Also, reset the array.

void clydeReleased( uint8_t l ){
#ifdef TOUCHY_FEELY_DEBUG
  Serial << "TouchyFeely: released! " << l << endl;
#endif
  //checkTouch( l );
  addToTouchArray( l );
  touch_counter++;  // increment this counter with each touch.
  if( touch_counter > max_touch_counter_reset ){
#ifdef TOUCHY_FEELY_DEBUG
  Serial << "TouchyFeely: Calling RESET on the touchy feely! got in total " << touch_counter << " touches." << endl;
#endif
  touchyfeely.reset( true, TOUCH_LEVEL, RELEASE_LEVEL );
  touch_counter=0;
  }
}

void clydeTouched(uint8_t l) {
#ifdef TOUCHY_FEELY_DEBUG
  Serial << "TouchyFeely: touched! " << l << endl;
#endif
  touch_array_last_touch_time=0;  // reset that to one.
}

void clydeDetected(uint8_t l) {
#ifdef TOUCHY_FEELY_DEBUG
  Serial << "TouchyFeely: Detected! " << l << endl;
#endif
}

// push the leg number to an
// array and record the last touch time. if the most recent touch
// is more than TOUCH_TRIGGER_DELAY ago, evaluate the full array,
// but require always double touches (to avoid false positives).
// inserts the leg-number at position 0 in the array and shifts all previous numbers.
void addToTouchArray( uint8_t leg ){
  // shift all values in the array
  for( int current_idx = 6; current_idx >= 0; current_idx-- ){
    touch_array_touched_leg[( current_idx + 1 )] = touch_array_touched_leg[current_idx];
  }
  // add the leg number to the stack and record the last touch time.
  touch_array_touched_leg[0] = (leg+1);
  touch_array_last_touch_time = millis();
}

/*
  Evaluate the array and trigger an event if we got double clicks. This
  function should be called from the main update function.
 */
void evalTouchTimeArray(){
  // get the most recent entry in touch_array_times
  if( touch_array_last_touch_time == 0 )
    return;
  uint32_t current_time_millis = millis();
  if( ( current_time_millis - touch_array_last_touch_time ) > TOUCH_TRIGGER_DELAY ){
    touch_array_last_touch_time = 0;   // reset, so that we only call it after a leg was really touched...
    touch_counter = 0;   // reset the touch counter.
    // evaluate the stack.
    // go through the array and report all duplets (i.e. double touches of the same leg)
    uint8_t lastleg=0;
    char touch_string[4]=""; // should be touch_array_touched_leg/2
    for( int i=0; i < 8; i++ ){
      if( lastleg!=touch_array_touched_leg[ i ] ){
	lastleg = touch_array_touched_leg[ i ];
      }else{
	// means the last record also touched this leg.
	if( lastleg!=0 ){
	  // append to the touch_string
	  char current_leg_string[2]="";
	  sprintf( current_leg_string, "%d", lastleg );
	  strcat( touch_string, current_leg_string );
	  #ifdef TOUCHY_FEELY_DEBUG
	  Serial << "evalTouchTimeArray: the touch string is now " << touch_string << endl;
	  #endif
	}
	lastleg = 0;  // set that to 0, so we're looking for a new duplet.
      }
      // reset the value...
      touch_array_touched_leg[ i ] = 0;
    }
#ifdef TOUCHY_FEELY_DEBUG
    Serial << "evalTouchTimeArray: FINAL touch string: P" << touch_string << "P" << endl;
#endif
    if( strcmp( touch_string, "" )==0 ){
#ifdef TOUCHY_FEELY_DEBUG
    Serial << "evalTouchTimeArray: dash, is an empty string!" << endl;
    return;
#endif
    }
    boolean rgb_on = clyde.current_colour[ 0 ] > 0 || clyde.current_colour[ 1 ] > 0 || clyde.current_colour[ 2 ] > 0;
    if( strcmp( touch_string, leg_switch )==0 ){
#ifdef TOUCHY_FEELY_DEBUG
      Serial << "evalTouchArray! got: leg_switch!" << touch_string << endl;
#endif
      switchLights();	  
    }else if( rgb_on && strcmp( touch_string, "1" )==0 ){
#ifdef TOUCHY_FEELY_DEBUG
      Serial << "evalTouchArray! got: " << touch_string << " fade to red." << endl;
#endif
      startRGBCycle( COLORBREWER_RED, fadetime, 1 );
    }else if( rgb_on && strcmp( touch_string, "2" )==0 ){
#ifdef TOUCHY_FEELY_DEBUG
      Serial << "evalTouchArray! got: " << touch_string << " fade to red." << endl;
#endif
      startRGBCycle( COLORBREWER_BLUE, fadetime, 1 );
    }else if( rgb_on && strcmp( touch_string, "3" )==0 ){
#ifdef TOUCHY_FEELY_DEBUG
      Serial << "evalTouchArray! got: " << touch_string << " fade to red." << endl;
#endif
      startRGBCycle( COLORBREWER_GREEN, fadetime, 1 );
    }else if( rgb_on && strcmp( touch_string, "4" )==0 ){
#ifdef TOUCHY_FEELY_DEBUG
      Serial << "evalTouchArray! got: " << touch_string << " fade to red." << endl;
#endif
      startRGBCycle( COLORBREWER_ORANGE, fadetime, 1 );
    }else if( rgb_on && strcmp( touch_string, "5" )==0 ){
#ifdef TOUCHY_FEELY_DEBUG
      Serial << "evalTouchArray! got: " << touch_string << " fade to red." << endl;
#endif
      startRGBCycle( COLORBREWER_PURPLE, fadetime, 1 );
    }else if( rgb_on && strcmp( touch_string, "6" )==0 ){
#ifdef TOUCHY_FEELY_DEBUG
      Serial << "evalTouchArray! got: " << touch_string << " fade to red." << endl;
#endif
      startRGBCycle( COLORBREWER_PINK, fadetime, 1 );
    }else if( rgb_on && strcmp( touch_string, "11" )==0 ){
#ifdef TOUCHY_FEELY_DEBUG
      Serial << "evalTouchArray! got: " << touch_string << " fade through all colors." << endl;
#endif
      uint8_t the_colors[ 18 ] = {228, 26, 28, 240, 2, 127, 255, 127, 0, 152, 78, 163, 55, 126, 184, 77, 175, 74 };
      uint32_t the_times[ 6 ] = { 1000, 1000, 1000, 1000, 1000, 1000 };
      startRGBCycle( the_colors, the_times, 6 );       
      /*    }else if( strcmp( touch_string, "22" ) ){
#ifdef TOUCHY_FEELY_DEBUG
      Serial << "evalTouchArray! got: " << touch_string << " start sunset." << endl;
#endif
sunset();*/
    }
  }
}

