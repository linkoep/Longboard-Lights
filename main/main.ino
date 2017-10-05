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
uint32_t orange = strip.Color(127,60,0);
uint32_t teal = strip.Color(0,50,45);
uint32_t chillColor = teal;

//Board Locations
int startF = 0;
int startR = 18;
int startB = 67;
int startL = 82;
int endL = 128;

int chillLength = 10;
int chillSpeed = 7; //Milliseconds before update

int stepOnLength = 12;
int stepOnSpeed = 7;

int signalSpeed = 500;

//accelerometer


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
  mma.read();
  Serial.print(mma.x); Serial.print("\t"); Serial.println(mma.y);
  if(mma.x < -2000 || mma.x > 2000)
  {
    off();
  }
  else
  {
    if(mma.y < -3500)
    {
      off();
    }
    else if(mma.y < -450)
    {
      forward();
      if(mma.x > 600)
     {
      signalRight();
     }
     else if(mma.x < -460)
     {
      signalLeft();
     }
     else
     {
      chill();
     }
    }
    else if(mma.y < -350)
    {
      delay(500);
      if(mma.y > -450 && mma.y < -350)
      {
        stepOn();
      }
    }
    else if(mma.y < -130)
    {
      off();
    }
    else if(mma.y < 3500)
    {
      forward();
      if(mma.x > 600)
     {
      signalRight();
     }
     else if(mma.x < -460)
     {
      signalLeft();
     }
     else
     {
      chill();
     }
    }
    else
    {
      off();
    }
  }
  delay(10);
}

void forward(){
  for(int i = startF; i < startR; i ++)
  {
  strip.setPixelColor(i, white);
  }

  for(int i = startR; i < startB; i++)
  {
    strip.setPixelColor(i, 0);
  }

  for(int i = startB; i < startL; i ++)
  {
  strip.setPixelColor(i, red);
  }

  for(int i = startL; i <= endL; i++)
  {
    strip.setPixelColor(i, 0);
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
  for(int i = startR; i < startB; i++)
  {
    strip.setPixelColor(i, orange);
  }
  strip.show();
  delay(signalSpeed);
  for(int i = startR; i < startB; i++)
  {
    strip.setPixelColor(i, 0);
  }
  strip.show();
  delay(signalSpeed);
}

void signalLeft(){
  for(int i = startL; i <= endL; i++)
  {
    strip.setPixelColor(i, orange);
  }
  strip.show();
  delay(signalSpeed);
  for(int i = startL; i <= endL; i++)
  {
    strip.setPixelColor(i, 0);
  }
  strip.show();
  delay(signalSpeed);
}

void chill(){
  for(int leader = startR; leader < startB + chillLength; leader ++)
  {
    if(leader < startB)
    {
    strip.setPixelColor(leader, chillColor);
    }
    if((endL + startR) - leader <=endL && (endL + startR) - leader >= startL)
    {
      strip.setPixelColor((endL + startR) - leader, chillColor);
    }
    
    if(leader - chillLength >= startR)
    {
      strip.setPixelColor(leader - chillLength, 0); 
    }

    if(endL + startR - leader + chillLength <= endL)
    {
      strip.setPixelColor(endL + startR - leader + chillLength, 0);
    }
    strip.show();
    delay(chillSpeed);
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


void stepOn(){
    int follow;
    for(int leader = 0; leader <= endL; leader ++)
    {
      if(leader - stepOnLength < 0)
      {
        follow = leader - stepOnLength + 128;
      }
      else
      {
        follow = leader - stepOnLength;
      }
      strip.setPixelColor(leader, teal);
      strip.setPixelColor(follow, 0);
      strip.show();
      delay(stepOnSpeed);
    }
}

void off(){
  for(int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}




