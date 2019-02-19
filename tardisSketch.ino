// BY8X01-16P-Arduino Indexed Playback Example
// In this example, folders are named "00" through "99" and files inside them are named
// "001.mp3" (or .wav) through "255.mp3" (or .wav). The folder and file index passed into
// playFolderFileIndex will play that specific file. Note that combination play here is
// not supported so waiting between track plays is required. Having a busy pin connected
// here will allow for tighter timing control between file playbacks.

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "BY8X01-16P.h"


const byte busyPin = 12;
const byte clapPin = 2;

#define NUMPIXELS      1
#define LEDPIN            7

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

BY8X0116P audioController(Serial, busyPin); // Library using Serial1 UART and busy pin input D12

void setup() {
//    Serial.begin(115200);

    Serial.begin(9600);            // Serial1 must be started first - only supported UART baud rate is 9600

    audioController.init();         // Initializes module

    audioController.setVolume(10);  // Sets player volume to 20 (out of 30 max)



    pinMode(clapPin,INPUT_PULLUP); //clap indicator
    randomSeed(analogRead(0));
    pixels.begin();

    pinMode(LED_BUILTIN, OUTPUT); //For testing - blinker pulse

/*
    audioController.playFolderFileIndex(0, 1); // Plays "00\001.mp3"

    int numTracks = 4; //getNumberOfTracksInCurrentFolder(); // Gets number of tracks in current folder
  //  Serial.println(numTracks);      // Should display number of tracks in the "00" folder

    char buffer[12];
    audioController.getCurrentTrackFilename(buffer); // Gets current filename (in 8.3 format) and places it into buffer

    //Serial.println(buffer);         // Should display "001.mp3"

    audioController.waitBusy();     // Blocking call that waits until all songs have completed

    // Plays all the remaining songs in the entire folder, printing out their file name upon playback
    for (int i = 2; i <= numTracks; ++i) {
        audioController.playFolderFileIndex(0, i);

        audioController.getCurrentTrackFilename(buffer);
      //  Serial.println(buffer);

        audioController.waitBusy();
    }

  //  Serial.println("All done!");
  */
}

void loop() {

  while (digitalRead(clapPin) == LOW); //wait for clap

  pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  pixels.show();

  
  long randNumber = random(4) + 1;
  audioController.playFileIndex(randNumber); // Queues first file on MicroSD card for playback

  int hue = 0;

  while (audioController.isBusy()) {

//     digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//     delay(1000);                       // wait for a second
//     digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//     delay(1000);                       // wait for a second

  if (hue > 360) {
    hue = 0;
  }
      // saturation and value = 1
      setLedColorHSV(hue++, 1, 1);
      delay(5);
  }

//turn off led
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
 
  audioController.waitPlaybackFinished(); // Blocking call that waits until all songs have completed
  
}

// thanks to http://forum.arduino.cc/index.php?topic=307655.5
void setLedColorHSV(int h, double s, double v) {

  double r=0;
  double g=0;
  double b=0;
  double hf=h/60.0;

  int i=(int)floor(h/60.0);
  double f = h/60.0 - i;
  double pv = v * (1 - s);
  double qv = v * (1 - s*f);
  double tv = v * (1 - s * (1 - f));

  switch (i)
  {
  case 0:
    r = v;
    g = tv;
    b = pv;
    break;
  case 1:
    r = qv;
    g = v;
    b = pv;
    break;
  case 2:
    r = pv;
    g = v;
    b = tv;
    break;
  case 3:
    r = pv;
    g = qv;
    b = v;
    break;
  case 4:
    r = tv;
    g = pv;
    b = v;
    break;
  case 5:
    r = v;
    g = pv;
    b = qv;
    break;
  }

  //set each component to a integer value between 0 and 255
  int red = constrain((int)255*r,0,255);
  int green = constrain((int)255*g,0,255);
  int blue = constrain((int)255*b,0,255);

  pixels.setPixelColor(0, pixels.Color(red, green, blue));
  pixels.show();
}

