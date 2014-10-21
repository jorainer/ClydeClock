// note, cycleRGB is defined in Behaviour.ino

//void clydeTouched(uint8_t l) {
void clydeReleased(uint8_t l) {
#ifdef TOUCHY_FEELY_DEBUG
  Serial << "Callback- Touched! " << l << ", leg switch would be " << leg_switch << endl;
#endif
  if( l==leg_switch ){
#ifdef TOUCHY_FEELY_DEBUG
    Serial << "got the switch leg" << endl;
#endif
    switchLights();
    //switchWL();
    return;
  }
  // only if the RGB light is on...
  if( clyde.current_colour[ 0 ] > 0 || clyde.current_colour[ 1 ] > 0 || clyde.current_colour[ 2 ] > 0 ){
    switch(l) {
    case 0:
      // cycle red
      startRGBCycle( COLORBREWER_RED, fadetime, 1 );
      break;
    case 1:
      // cycle green
      startRGBCycle( COLORBREWER_GREEN, fadetime, 1 );
      break;
    case 2:
      // cycle blue
      startRGBCycle( COLORBREWER_BLUE, fadetime, 1 );
      break;
    case 3:
      startRGBCycle( COLORBREWER_PURPLE, fadetime, 1 );
      break;
    case 4:
      startRGBCycle( COLORBREWER_ORANGE, fadetime, 1 );
      break;
    case 5:
      startRGBCycle( COLORBREWER_PINK, fadetime, 1 );
      break;
    }
  }
}

//void clydeReleased(uint8_t l) {
void clydeTouched(uint8_t l) {
#ifdef TOUCHY_FEELY_DEBUG
  Serial << "Callback- Released!" << l << endl;
#endif
}

void clydeDetected(uint8_t l) {
#ifdef TOUCHY_FEELY_DEBUG
  Serial << "Callback- Detected!" << l << endl;
#endif
}

