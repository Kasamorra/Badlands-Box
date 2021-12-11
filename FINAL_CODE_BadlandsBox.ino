//Music box that plays the melody of Bruce Springsteen's "Badlands" when opened, stops when closed
//Compoments: Arduino, passive buzzer, light dependent resistor (short: ldr),10k Ohm resistor 
//Pins used: A0, 7

#include "pitches.h" //file with all notes the buzzer could play

//Light Dependent Resistor
const int ldrPin = A0;
int lightLevel; //light value the ldr picks up
const int bright = 450; //guideline value for when the box is opened
const int dark = 300; //guideline value for when the box is closed

//Passive Buzzer
const int buzzPin = 7;

// Array with Notes of the melody
int16_t melody[] = {
NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_DS5, NOTE_CS5, 
NOTE_B4, NOTE_B4, NOTE_B4, NOTE_CS5, NOTE_B4, NOTE_B4, NOTE_B4, NOTE_GS4, NOTE_A4, NOTE_A4, 
NOTE_A4, NOTE_A4, NOTE_GS4, NOTE_FS4, NOTE_E4, NO_SOUND, NOTE_DS5, NOTE_E5, NOTE_E5, NOTE_E5, 
NOTE_DS5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_DS5, 
NOTE_CS5, NOTE_B4, NOTE_GS4, NOTE_B4, 
NOTE_E5, NOTE_DS5, NOTE_CS5, NOTE_B4, //woah woah woah woah 
NOTE_E5, NOTE_E5, //Bad-lands
NO_SOUND, //break
NOTE_E5, NOTE_DS5, NOTE_CS5, NOTE_B4, //woah woah woah woah
NOTE_E5, NOTE_E5 //Bad-lands
}; //57 in total

//Each tone's duration
//Full Note = 1, Half Note = 2, ...
int16_t noteDuration[] = {
4, 3, 8, 8, 8, 8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 4, 4, 8, 8, 8, 
8, 8, 8, 8, 8, 8, 8, 16, 16, 16, 
16, 8, 8, 4, 8, 8, 6, 16, 8, 8, 
6, 16, 8, 2, 
4, 4, 4, 4, //woah woah woah woah
4, 2, //Bad-lands
8, //break
4, 4, 4, 4, //woah woah woah woah
2, 1 //Bad-lands
}; //57 in total


const int songPace = 1450; //duration of full note in milliseconds
int melodyLength = 0; 
int noteLength; //how long a note is actually to be played
const int breakTime = 300;
int playState = 0; // 0 = waiting to play, 1 = Song playing, -1 = Song over
int noteCount = 0; //Counter for position in array

void setup() {
  Serial.begin(9600); //for troubleshooting
  pinMode(buzzPin, OUTPUT);

  melodyLength = sizeof(melody) / sizeof(melody[0]); //calculates length of melody
}

void loop() {
  lightLevel = analogRead(ldrPin); //ldr checks the light level
  Serial.println(lightLevel); //for troubleshooting

  if (lightLevel >= bright && playState == 0) { //checks if box opened and song not playing
    playState = 1;  //changes state to "playing"
  }
  else if (lightLevel <= dark) //checks if box closed
  {
    if ((playState == 1) || (playState == -1)) //checks if melody playing or already over
    {
      playState = 0; //changes state to "song waiting to play"
      noteCount = 0; //resets counter for array position
    }
  }

  if (playState == 1) //checks if state is "playing"
  {
    noteLength = songPace / noteDuration[noteCount]; //calculates how long next note is to be played
    
    tone(buzzPin, melody[noteCount], noteLength); //plays next note
    delay(noteLength*1.2);
    noteCount = noteCount + 1; //updates counter

    if (noteCount == melodyLength) //checks if full melody was played
    {
      noteCount = 0; //resets counter
      playState = -1; //sets state to "song over"
    }
  }
  else
  {
    digitalWrite(buzzPin, LOW); //turns buzzer off
    noteCount = 0; //resets counter
  }

}
