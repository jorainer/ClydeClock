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
  Serial << "Speak: playSong: start playing song with " << number_notes, " notes." << endl;
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

void playTestSong(){
  int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_GS3, NOTE_G3,0, NOTE_B3, NOTE_C4};
  uint8_t noteDurations[] = {4, 8, 8, 4,4,4,4,4 };
  playSong( noteDurations, melody, 8 );
}

#endif
