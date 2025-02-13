#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

const int PIN_MP3_TX = 2;
const int PIN_MP3_RX = 3;

const int PAUSE_PLAY = 6;  //pause and resume button maps to pin d6
const int SKIP_NEXT = 7;  //skip and next button maps to pin d7

int pause_state = 0;
int skip_state = 0;

bool playing = false;

DFRobotDFPlayerMini mp3;

void setup() 
{
  pinMode(PAUSE_PLAY, INPUT_PULLUP);
  pinMode(SKIP_NEXT, INPUT_PULLUP);
  SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
  
  //intitialize USB serial port
  Serial.begin(9600);

  //initialize software serial port
  softwareSerial.begin(9600);
  
  if (!mp3.begin(softwareSerial))
  {
    Serial.println("connection failed");
  }

  //volume range [0,30]
  mp3.volume(22);

  //play 0001.mp3
  mp3.play(1);

  mp3.pause();

  mp3.start();
  playing = true;

  //debugging
  Serial.print("number of files on MicroSd : ");
  Serial.println(mp3.readFileCounts());
  Serial.print("file being played : ");
  Serial.println(mp3.readCurrentFileNumber());


}

void loop() 
{
  pause_state = digitalRead(PAUSE_PLAY);
  skip_state = digitalRead(SKIP_NEXT);
  
  //pause and play button functionality
  if (pause_state == LOW) //when the button is pressed d6 will read low
  {
    if (playing)
    {
      mp3.pause();
      playing = false;
      Serial.println("paused...");
      delay(200); //delay to keep arduino from resuming
    }
    else
    {
      mp3.start();
      playing = true;
      Serial.println("playing...");
      delay(200); //delay to keep arduino from re-pausing
    }
  }

  //skip functionality
  if (skip_state == LOW) //when the button is pressed d7 wil be low
  {
    mp3.next();
    Serial.print("file being played : ");
    Serial.println(mp3.readCurrentFileNumber());
    playing = true; //if the mp3 is paused the skip button will resume playing
    Serial.println("playing next...");
  }



}
