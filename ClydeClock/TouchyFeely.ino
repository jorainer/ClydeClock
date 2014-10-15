// note, cycleRGB is defined in Behaviour.ino

//void clydeTouched(uint8_t l) {
void clydeReleased(uint8_t l) {

  Serial << "Callback- Touched! " << l << ", leg switch would be " << leg_switch << endl;

  if( l==leg_switch ){
    Serial << "got the switch leg" << endl;
    //switchLights();
    switchWL();
    return;
  }
  // only if the RGB light is on...
  if( clyde.current_colour[ 0 ] > 0 || clyde.current_colour[ 1 ] > 0 || clyde.current_colour[ 2 ] > 0 ){
    switch(l) {
    case 0:
      // cycle red
      startRGBCycle( COLORBREWER_RED, fadetime );
      break;
    case 1:
      // cycle green
      startRGBCycle( COLORBREWER_GREEN, fadetime );
      break;
    case 2:
      // cycle blue
      startRGBCycle( COLORBREWER_BLUE, fadetime );
      break;
    case 3:
      startRGBCycle( COLORBREWER_PURPLE, fadetime );
      break;
    case 4:
      startRGBCycle( COLORBREWER_ORANGE, fadetime );
      break;
    case 5:
      startRGBCycle( COLORBREWER_PINK, fadetime );
      break;
    }
  }
}

//void clydeReleased(uint8_t l) {
void clydeTouched(uint8_t l) {

  Serial << "Callback- Released!" << l << endl;

}

void clydeDetected(uint8_t l) {

  Serial << "Callback- Detected!" << l << endl;

}

