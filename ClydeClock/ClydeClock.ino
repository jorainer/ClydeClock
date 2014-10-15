/*
 * ClydeClock
 * ----------
 *
 * Provides some basic Clyde usage and allows to use Clyde as an
 * "alarm clock"
 *
 * Bases on the ClydeDev from Erin RobotGrrl (http://robotgrrl.com)
 * Library requirements:
 * - ClydeDev
 * - Streaming
 * - MPR121 & I2Cdev
 * - Time
 * - TimeAlarms
 * October, 2014
 *
 */

#include <Wire.h>
#include <MPR121.h>
#include <Streaming.h>

#include <ClydeDev.h>
#include <ClydeTouchyFeely.h>
#include <ClydeAfraidDark.h>

// enable debugging of the Basic.ino
#define BASIC_DEBUG

// to enable the "clock" stuff...
#define ENABLE_CLOCK
#ifdef ENABLE_CLOCK
#include <Time.h>
#include <TimeAlarms.h>
#endif

// -- clyde dev
ClydeDev clyde = ClydeDev();

// -- touchy feely module in module port 1
ClydeTouchyFeely touchyfeely = ClydeTouchyFeely(1);
boolean tf_enabled = false;

// afraid of the dark module in module port 2
ClydeAfraidDark afraiddark = ClydeAfraidDark(2);
boolean ad_enabled = false;
boolean currently_pressed = false;

// that's my global variables.
uint32_t fadetime [1] = {1000};         // default time for color fading.
static const uint8_t max_steps = 12;    // max number of allowed steps; need this to pre-allocate the memory.
// for the white light:
uint8_t wl_current_step = 0;            // current brightness step for the white light.
uint8_t wl_number_steps = 0;            // the number of steps of the cycle.
uint32_t wl_step_start = millis();      // when was the step started?
uint8_t wl_step_start_intensity = 0;    // the intensity at the start of a cycle step.
uint8_t wl_intensity = 0;               // the current brightness of the white light.
uint8_t wl_step_intensities[max_steps]; // the array with the brightness values to cycle through.
uint32_t wl_step_durations[max_steps];  // the duration (ms) for each brightness step.
// for the RGB light:
uint8_t current_step = 0;               // current step for the cycle through RGB values.
uint8_t number_steps = 0;               // the number of steps.
uint32_t step_start = millis();         // the time when the cycle step started.
uint8_t step_start_r = 0;               // the red intensity at the beginning of a cycle step.
uint8_t step_start_g = 0;               // the green intensity at the beginning of a cycle step.
uint8_t step_start_b = 0;               // the blue intensity at the beginning of a cycle step.
uint8_t step_colors[max_steps*3];       // the array with the RGB values to cycle through.
uint32_t step_durations[max_steps];     // the array with the durations for each cycle step.

// TouchyFeely related:
uint8_t leg_switch = 2;  // set this to a value from 0-5 if you want to use a leg as the light switch instead of the eye... e.g. if the eye is buggy, like the one of my Clyde.
// Some pre-defined colors (from R's RColorBrewer package).
uint8_t COLORBREWER_RED [ 3 ] = { 228, 26, 28 };
uint8_t COLORBREWER_BLUE [ 3 ] = { 55, 126, 184 };
uint8_t COLORBREWER_GREEN [ 3 ] = { 77, 175, 74 };
uint8_t COLORBREWER_PURPLE [ 3 ] = { 152, 78, 163 };
uint8_t COLORBREWER_ORANGE [ 3 ] = { 255, 127, 0 };
uint8_t COLORBREWER_PINK [ 3 ] = { 240, 2, 127 };  // not really in RColorBrewer...

void setup() {

  // inistialize variables:
  memset((void*)&wl_step_intensities[0], 0, sizeof( uint8_t )*max_steps );
  memset((void*)&wl_step_durations[0], 0, sizeof( uint32_t )*max_steps );
  memset((void*)&step_colors[0], 0, sizeof( uint8_t )*max_steps*3 );
  memset((void*)&step_durations[0], 0, sizeof( uint32_t )*max_steps );

  Serial.begin(9600);
  // comment the line below if you don't want serial communication.
  while(!Serial);
  // waits for 5 seconds to properly start all modules.
  delay(5000);

  // -- clyde!
  //clyde.setDebugStream(&Serial, ClydeDev::DEBUG); // uncomment if you want to see debug text
  clyde.init();

  clyde.setEyePressedHandler(clydeEyePressed);
  clyde.setEyeReleasedHandler(clydeEyeReleased);


  // -- touchy feely module
  touchyfeely.setDebugStream(&Serial, ClydeDev::DEBUG); // uncomment if you want to see debug text
  tf_enabled = touchyfeely.init();

  if(tf_enabled) {
    // these handlers are defined in TouchyFeely.ino
    touchyfeely.setTouchedHandler(clydeTouched);
    touchyfeely.setReleasedHandler(clydeReleased);
    touchyfeely.setDetectedHandler(clydeDetected);
  }


  // -- afraid of the dark module
  //afraiddark.setDebugStream(&Serial, ClydeDev::DEBUG); // uncomment if you want to see debug text
  ad_enabled = afraiddark.init();

  Serial << "Hello! :3" << endl;

  // initialize lights.
  clyde.setLight( wl_intensity );
  clyde.setEyeRGB( step_start_r, step_start_g, step_start_b );
}

// the main loop of clyde...
void loop() {

  // check if we are in some color cycle and if so update the colors accordingly
  //cycleThroughRGBColors();
  // the same for the white light
  fadeWhiteLight();

  clyde.update();

  if(tf_enabled) touchyfeely.update();

  if(ad_enabled) {
    afraiddark.update();

    // uncomment this if you want the light sensor to control the intensity
    // of the rgb light... the result is a little glitchy though, as it is
    // using the raw sensor values.
    /*
    uint16_t light_level = afraiddark.getLight();
    intensity = ((float)light_level)/1000.0;

    Serial << "light level: " << light_level << " intensity: " << intensity << endl;
    */

  }

}


