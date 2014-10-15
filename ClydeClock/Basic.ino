// this file contains some basic functions.


// cycles through a pre-defined set of colors in a pre-defined time.
// this should be called by the "update" function.
// global variables I depend on:
// + current_step   ... in order to know which color we want to reach.
// + step_start     ... the time when the current step started.
// + step_start_r, step_start_g, step_start_b the colors of the (last) step.
// + step_colors    ... the array of rgb values.
// + step_durations ... the array of durations for each step.
void cycleThroughRGBColors( ){

  // if the step if 0 we don't do anything. everything > 0 means we're in a cycle.
  // we also return if the current step is larger than we have colors/steps.
  // btw... sizeof( step_durations ) / sizeof( step_durations[ 0 ] ) is the C style of getting the length of an array.
  if( current_step==0 | current_step > ( sizeof( step_durations ) / sizeof( step_durations[ 0 ] ) ) ){
    return;
  }

  uint32_t now = millis();
  uint32_t time_in_cycle_step = now - step_start;  // needs that step_start is defined...
  // check if we're still in this step or if we have to move to the next one.
  if( time_in_cycle_step > step_durations[ ( current_step - 1 ) ] ){
    // move on to the next step.
    current_step++;
    // just make sure we still have some steps...
    if( current_step > sizeof( step_durations ) / sizeof( step_durations[ 0 ] ) ){
      Serial << "Basic- cycleThroughRGBColors done with the cycle b" << endl;
      return;
    }
    step_start = now;
    step_start_r = clyde.current_colour[ 0 ];
    step_start_g = clyde.current_colour[ 1 ];
    step_start_b = clyde.current_colour[ 2 ];
    time_in_cycle_step = 1;  // set the time in step to 1 ms
  }
  // calculate the color difference, i.e. the difference between the step_start color and the
  // color we want to get to.
  int16_t diff_color_r = step_colors[ (current_step-1) * 3 ] - step_start_r;
  int16_t diff_color_g = step_colors[ ( (current_step-1) * 3 ) + 1 ] - step_start_g;
  int16_t diff_color_b = step_colors[ ( (current_step-1) * 3 ) + 2 ] - step_start_b;
  // the color is the start color + the time we spent in the step * with the difference
  // between the r color value we want to reach and the r color value at the beginning
  // divided by the time duration of the step.
  int16_t r = step_start_r + ( time_in_cycle_step / step_durations[ ( current_step - 1 )] * diff_color_r );
  if( r > 255 )
    r = 255;
  if( r < 0 )
    r = 0;
  // now the same for g and b...
  int16_t g = step_start_g + ( time_in_cycle_step / step_durations[ ( current_step - 1 )] * diff_color_g );
  if( g > 255 )
    g = 255;
  if( g < 0 )
    g = 0;
  int16_t b = step_start_b + ( time_in_cycle_step / step_durations[ ( current_step - 1 )] * diff_color_b );
  if( b > 255 )
    b = 255;
  if( b < 0 )
    b = 0;
  // finally set Clyde's eye color.
  clyde.setEyeRGB( r, g, b );
}

// similar as cycleRGB, just for the white light.
// require global parameters:
void fadeWhiteLight(){
  if( wl_current_step == 0 | wl_current_step > ( sizeof( wl_step_durations ) / sizeof( wl_step_durations[ 0 ] ) ) ){
    wl_current_step = 0;
    return;   // return if we did not yet start or are finished with fading.
  }
    
  uint32_t now = millis();
  uint32_t time_in_cycle_step = now - wl_step_start;
  if( time_in_cycle_step==0 )
    time_in_cycle_step = 1;
  // check if we're still in this step or if we have to move to the next one.
  if( time_in_cycle_step > wl_step_durations[ ( wl_current_step - 1 ) ] ){
    // move on to the next step.
    wl_current_step++;
    // just make sure we still have some steps...
    if( wl_current_step > ( sizeof( wl_step_durations ) / sizeof( wl_step_durations[ 0 ] ) ) ){
      Serial << "Basic- fadeWhiteLight done with the cycle b" << endl;
      wl_current_step=0;
      return;
    }
    wl_step_start = now;
    time_in_cycle_step = 1;  // set the time in step to 1 ms
  }
  // calculate the color difference, i.e. the difference between the step_start color and the
  // color we want to get to.
  Serial << "Basic- fadeWhiteLight wl_intensity a: " << wl_intensity << " end: " << wl_step_intensities[ (wl_current_step-1) ] << " step " << wl_current_step << " end[0]: " << wl_step_intensities[0] << endl;
  //int16_t
  float diff_intensity = wl_step_intensities[ (wl_current_step-1) ] - wl_intensity;
  Serial << "diff_intensity: " << diff_intensity << endl;
  float to_add = ( time_in_cycle_step / wl_step_durations[ ( wl_current_step -1 ) ] ) * diff_intensity;
  Serial << "to_add: " << to_add << endl;
  uint8_t wl_intensity=wl_intensity + (uint8_t)to_add;
  Serial << "Basic- fadeWhiteLight wl_intensity b: " << wl_intensity << endl;
  if( wl_intensity > 255 ){
    wl_intensity = 255;
  }
  if( wl_intensity < 0 ){
    wl_intensity = 0;
  }
  // change the light if we got up to here.
  Serial << "Basic- fadeWhiteLight wl_intensity c: " << wl_intensity << endl;
  clyde.setLight( wl_intensity );
}

//void startFadeWhiteLight(uint8_t *intens, uint16_t *durations ){
void startFadeWhiteLight(uint8_t intens[], uint16_t durations[] ){
  Serial << "Basic- startFadeWhiteLight called" << endl;
  wl_current_step = 1; // initialize the step, so function fadeWhiteLight will actually fade.
  wl_step_start = millis();
  // re-allocate storage depending on the size of submitted arrays.
  //wl_step_intensities = (uint8_t*)realloc( wl_step_intensities, sizeof( intens ) );
  Serial << "got before " << wl_step_intensities[0] << endl;
  wl_step_intensities = intens;
  Serial << "got after " << wl_step_intensities[0] << endl;
  //wl_step_durations = (uint16_t*)realloc( wl_step_durations, sizeof( durations ) );
  wl_step_durations = durations;
}

// just start a color cycle and initialize the parameters (i.e. global variables)
// s_colors and s_durations have to be arrays (even if they have only length 1).
void startRGBCycle( uint8_t s_colors[], uint16_t s_durations[] ){
  Serial << "Basic- startRGBCycle called" << endl;
  current_step = 1;
  step_start_r = clyde.current_colour[ 0 ]; // red value from which we start.
  step_start_g = clyde.current_colour[ 1 ]; // green value from which we start.
  step_start_b = clyde.current_colour[ 2 ]; // blue value from which we start.
  step_start = millis();
  // re-allocate storage depending on the size of submitted arrays.
  //step_colors = (uint8_t*)realloc( step_colors, sizeof( s_colors ) );
  step_colors = s_colors;
  //step_durations = (uint16_t*)realloc( step_durations, sizeof( s_durations ) );
  step_durations = s_durations;
  //cycleThroughRGBColors( ); will call that in the update call anyways
}

// stop a color cycle. Will essentially just set the number of steps to 0
void stopCycle(){
  current_step = 0;
  wl_current_step = 0;
}


// switches the light.
// if off -> switch on eye
// if eye on -> switch on white light
// if eye and white light on -> switch of eye
// if eye off and white light on -> switch off white light.
// Note: would like to get public access to the white light and to the threeway_max
void switchLights(){
  if( wl_intensity==0 ){
    // white light is off.
    if( clyde.current_colour[ 0 ] > 0 | clyde.current_colour[ 1 ] > 1 | clyde.current_colour[ 2 ] > 0 ){
      // got LED on, turn on white light.
      wl_intensity = 255;
      clyde.setLight( wl_intensity );
    }else{
      // all is off, turn on led.
      startRGBCycle( COLORBREWER_BLUE, fadetime );
    }
  }else{
    // white light is on.
    if( clyde.current_colour[ 0 ] > 0 | clyde.current_colour[ 1 ] > 1 | clyde.current_colour[ 2 ] > 0 ){
      // got LED on, turn off led.
      uint8_t lightoff [3] = {0, 0, 0};
      startRGBCycle( lightoff, fadetime );
    }else{
      // LED is off, turn off all lights
      wl_intensity = 0;
      clyde.setLight( wl_intensity );
    }
  }  
}

void switchWL(){
  if( wl_intensity==0 ){
    //wl_intensity = 255;
    //clyde.setLight( wl_intensity );
    uint8_t newwl[1]={255};
    Serial << "Basic- switchWL turn on" << endl;
    startFadeWhiteLight( newwl, fadetime );
  }else{
    Serial << "Basic- switchWL turn off" << endl;
    //wl_intensity = 0;
    uint8_t newwl[1]={0};
    //clyde.setLight( wl_intensity );
    startFadeWhiteLight( newwl, fadetime );
  }
}


/* uint32_t myabs( uint32_t a ){ */
/*   if( a < 0 ) */
/*     return -a; */
/*   return a; */
/* } */