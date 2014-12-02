// That's everything related to sound output.
// Bases on the core tone library: http://arduino.cc/en/Tutorial/Tone
// Alternatively we could have used:
// https://code.google.com/p/rogue-code/wiki/ToneLibraryDocumentation
// and https://itp.nyu.edu/physcomp/labs/labs-arduino-digital-and-analog/tone-output-using-an-arduino/

// again, in order to allow other stuff to happen, we will initialize a song and update that
// song from the main loop... with a delay in between.

#ifdef ENABLE_SPEAK

//#include <notes.h>
//#include <pitches.h>  // for some odd reasons that thing won't compile as it doesn't find the NOTE variables...
//#include <Tone.h> // check if we could do that without Tone.


uint32_t speak_note_start_millis=0;  // use this to keep track of the time the last note was played. actually this represents each change in note playing, thus each start and end of a note!
uint8_t speak_number_notes=0;        // the number of notes in the song.
uint8_t speak_current_note=0;        // this is used to loop through the array.
static const uint8_t speak_max_notes=20;          // the maximal number of notes.
uint8_t speak_note_durations[speak_max_notes]; // 4=quarter, 8=eighth note etc.
int speak_notes[speak_max_notes];    // the notes, defined in pitches.h.
uint32_t speak_between_note_pause=30;// number of ms to pause between notes.
int speak_speed=1000;           // the playing speed. set to 1 second.
boolean speak_do_play=false;         // boolean specifying whether or not we play something
boolean speak_is_playing=false;      // wouldn't need that if we used the Tone lib and isPlaying.
uint8_t speak_out = 4;               // thats the Tone thing that we use to play.

// oh man, defining the notes in here...
static const int NOTE_B0 = 31;
static const int NOTE_C1 = 33;
static const int NOTE_CS1 = 35;
static const int NOTE_D1 = 37;
static const int NOTE_DS1 = 39;
static const int NOTE_E1 = 41;
static const int NOTE_F1 = 44;
static const int NOTE_FS1 = 46;
static const int NOTE_G1 = 49;
static const int NOTE_GS1 = 52;
static const int NOTE_A1 = 55;
static const int NOTE_AS1 = 58;
static const int NOTE_B1 = 62;
static const int NOTE_C2 = 65;
static const int NOTE_CS2 = 69;
static const int NOTE_D2 = 73;
static const int NOTE_DS2 = 78;
static const int NOTE_E2 = 82;
static const int NOTE_F2 = 87;
static const int NOTE_FS2 = 93;
static const int NOTE_G2 = 98;
static const int NOTE_GS2 = 104;
static const int NOTE_A2 = 110;
static const int NOTE_AS2 = 117;
static const int NOTE_B2 = 123;
static const int NOTE_C3 = 131;
static const int NOTE_CS3 = 139;
static const int NOTE_D3 = 147;
static const int NOTE_DS3 = 156;
static const int NOTE_E3 = 165;
static const int NOTE_F3 = 175;
static const int NOTE_FS3 = 185;
static const int NOTE_G3 = 196;
static const int NOTE_GS3 = 208;
static const int NOTE_A3 = 220;
static const int NOTE_AS3 = 233;
static const int NOTE_B3 = 247;
static const int NOTE_C4 = 262;
static const int NOTE_CS4 = 277;
static const int NOTE_D4 = 294;
static const int NOTE_DS4 = 311;
static const int NOTE_E4 = 330;
static const int NOTE_F4 = 349;
static const int NOTE_FS4 = 370;
static const int NOTE_G4 = 392;
static const int NOTE_GS4 = 415;
static const int NOTE_A4 = 440;
static const int NOTE_AS4 = 466;
static const int NOTE_B4 = 494;
static const int NOTE_C5 = 523;
static const int NOTE_CS5 = 554;
static const int NOTE_D5 = 587;
static const int NOTE_DS5 = 622;
static const int NOTE_E5 = 659;
static const int NOTE_F5 = 698;
static const int NOTE_FS5 = 740;
static const int NOTE_G5 = 784;
static const int NOTE_GS5 = 831;
static const int NOTE_A5 = 880;
static const int NOTE_AS5 = 932;
static const int NOTE_B5 = 988;
static const int NOTE_C6 = 1047;
static const int NOTE_CS6 = 1109;
static const int NOTE_D6 = 1175;
static const int NOTE_DS6 = 1245;
static const int NOTE_E6 = 1319;
static const int NOTE_F6 = 1397;
static const int NOTE_FS6 = 1480;
static const int NOTE_G6 = 1568;
static const int NOTE_GS6 = 1661;
static const int NOTE_A6 = 1760;
static const int NOTE_AS6 = 1865;
static const int NOTE_B6 = 1976;
static const int NOTE_C7 = 2093;
static const int NOTE_CS7 = 2217;
static const int NOTE_D7 = 2349;
static const int NOTE_DS7 = 2489;
static const int NOTE_E7 = 2637;
static const int NOTE_F7 = 2794;
static const int NOTE_FS7 = 2960;
static const int NOTE_G7 = 3136;
static const int NOTE_GS7 = 3322;
static const int NOTE_A7 = 3520;
static const int NOTE_AS7 = 3729;
static const int NOTE_B7 = 3951;
static const int NOTE_C8 = 4186;
static const int NOTE_CS8 = 4435;
static const int NOTE_D8 = 4699;
static const int NOTE_DS8 = 4978;



// initialize the "module"
void initializeSpeak(){
  memset((void*)&speak_note_durations[0], 0, sizeof( uint8_t )*speak_max_notes );
  memset((void*)&speak_notes[0], 0, sizeof( int )*speak_max_notes );
  // initialize the output.
  //speak_out.begin( 4 );  // using pin 4 as output; that's if we use the Tone library.
}

// start playing a song; set the stage to play the song.
void playSong( uint8_t durations[], int notes[], uint8_t number_notes ){
#ifdef SPEAK_DEBUG
  Serial << "Speak: playSong: start playing song with " << number_notes << " notes." << endl;
#endif
  speak_do_play=true;
  speak_is_playing=false;
  speak_number_notes=number_notes;
  speak_current_note=0;
  speak_note_start_millis=millis();
  // copy the content...
  for( int i=0; i < number_notes; i++ ){
    speak_note_durations[ i ] = durations[ i ];
    speak_notes[ i ] = notes[ i ];
  }
  // note: we're not starting to play the note here, that's the job
  // of updateSpeak.
}

void testSong(){
#ifdef SPEAK_DEBUG
  Serial << "Speak: testSong." << endl;
#endif
  int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_GS3, NOTE_G3,0, NOTE_B3, NOTE_C4};
  uint8_t noteDurations[] = {4, 8, 8, 4,4,4,4,4 };
  playSong( noteDurations, melody, 8 );
}

void dido(){
  int melody[] = {NOTE_C3, NOTE_C4};
  uint8_t noteDurations[] = {4, 8};
  playSong( noteDurations, melody, 2 );
}

// that's called from the main loop and in here we check
// a) are we supposed to play a song?
// b) in case, check if we should start to play a note.
void updateSpeak(){
  if( speak_do_play ){
    // evaluate which note we play, or if we should switch.
    if( speak_is_playing ){
      // check if it's time to stop the current note.
      if( ( millis() - speak_note_start_millis ) >= ( speak_speed/(int)speak_note_durations[ speak_current_note ] )){
	noTone( speak_out );              // stop playingthat note.
	speak_is_playing = false;
	//speak_out.stop();                 // stop playing that note.
	speak_current_note++;             // move to the next note
	speak_note_start_millis=millis(); // keep track of the last play change.
	if( speak_current_note >= speak_number_notes ){
	  // stop for good if we played the last note.
	  speak_current_note=0;
	  speak_do_play=false;
	}
      }
    }else{
      // check if we can start playing... if the pause between notes is over.
      if( millis() >= ( speak_between_note_pause + speak_note_start_millis ) ){
        speak_is_playing=true;
	tone( speak_out, speak_notes[ speak_current_note ] );
	//speak_out.play( speak_notes[ speak_current_note ] );
      }
    }
  }
}


#endif
