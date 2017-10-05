//Pin Setup
int lEDPin = 6;

//Libraries
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_NeoPixel.h>

//initialization
Adafruit_NeoPixel strip = Adafruit_NeoPixel(129, lEDPin, NEO_GRB + NEO_KHZ800);
Adafruit_MMA8451 mma = Adafruit_MMA8451();

//Color definitions
uint32_t white = strip.Color(100,100,90);
uint32_t red = strip.Color(127,0,0);
uint32_t orange = strip.Color(100,40,0);

//Board Locations
int startF = 0;
int startR = 18;
int startB = 67;
int startL = 82;
int endL = 128;

int blinkerLength = 10;
int blinkerSpeed = 7; //Milliseconds before update

//accelerometer
float v;
float v0 = 0;

void setup() {
  Serial.begin(9600);
  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }

  mma.setRange(MMA8451_RANGE_2_G);
  
  Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  Serial.println("G");
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
 
 delay(10);
 mma.read();
 
 /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("Y: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
v = v0 + event.acceleration.y * .01;
 Serial.println(v);
if(v > 0)
 {
  forward();
 }
 else if (v < 0)
 {
  reverse();
 }
 else
 {
  off();
 }

 v0 = v;

 if(mma.x > 300)
 {
  signalRight();
 }

 if(mma.x < -300)
 {
  signalLeft();
 }

 
}

void forward(){
  for(int i = startF; i < startR; i ++)
  {
  strip.setPixelColor(i, white);
  }

  for(int i = startB; i < startL; i ++)
  {
  strip.setPixelColor(i, red);
  }

  strip.show();
}

void reverse(){
  for(int i = startF; i < startR; i ++)
  {
  strip.setPixelColor(i, red);
  }

  for(int i = startB; i < startL; i ++)
  {
  strip.setPixelColor(i, white);
  }

  strip.show();
}

void signalRight(){
  for(int cycle = 0; cycle < 1; cycle++)
  {
    for(int leader = startR; leader < startB + blinkerLength; leader ++)
    {
      if(leader < startB)
      {
      strip.setPixelColor(leader, orange);
      }
      if(leader - blinkerLength >= startR)
      {
        strip.setPixelColor(leader - blinkerLength, 0); 
      }
      strip.show();
      delay(blinkerSpeed);
    }
  }
}

void signalLeft(){
  for(int cycle = 0; cycle < 1; cycle++)
  {
    for(int leader = endL; leader > startL - blinkerLength - 1; leader --)
    {
      if(leader >= startL)
      {
      strip.setPixelColor(leader, orange);
      }
      if(leader + blinkerLength <= endL)
      {
        strip.setPixelColor(leader + blinkerLength, 0); 
      }
      strip.show();
      delay(blinkerSpeed);
    }
  }
}

void party(){
  for(int cycle = 0; cycle < 20; cycle++){
    for(int i = 10; i < 30; i++){
      strip.setPixelColor(i, random(127), random(127), random(127));
    }
    for(int i = 40; i < 60; i++){
      strip.setPixelColor(i, random(127), random(127), random(127));
    }

    strip.show();
    delay(50);

    for(int i = 10; i < 30; i++){
      strip.setPixelColor(i, 0);
    }
    for(int i = 40; i < 60; i++){
      strip.setPixelColor(i, 0);
    }

    strip.show();
    delay(10);
  }
}

void off(){
  for(int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}
