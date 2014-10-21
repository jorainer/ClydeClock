// functionality related to the light sensor
// problem is that the measured light is very low...

uint16_t ITS_DARK = 1;         // that seems pretty low, but somehow my Clyde measures low intensities...
uint16_t last_light = 0;
uint32_t just_got_dark=0;
boolean sunset_triggered=false;

void checkForDarkness(){
  unsigned int light_level = afraiddark.getLight();
  //#ifdef AFRAID_DARK_DEBUG
    //Serial << "checkForSunset: light_level: " << light_level << endl;
  //#endif  
  // disable if white light is on, or its not dark...
  if( wl_intensity > 0 || light_level > ITS_DARK ){
    last_light = light_level;
    sunset_triggered = false;
    return;
  }
  // if it's dark:
  if( light_level <= ITS_DARK ){
    // but it wasn't dark before...
    if( last_light > ITS_DARK ){
      just_got_dark = millis();
#ifdef AFRAID_DARK_DEBUG
    Serial << "checkForSunset: got dark: " << light_level << "wasn't dark before: " << last_light << endl;
#endif      
    }else{
      // wait for some time...
      if( (millis() > ( just_got_dark + 1000 )) && !sunset_triggered ){
#ifdef AFRAID_DARK_DEBUG
	Serial << "checkForSunset: trigger sunset " << endl;
#endif      
	// we waited for some time, and it's still dark... so trigger.
	sunset();
	sunset_triggered = true;
      }
    }
  }
  last_light = light_level;
}


