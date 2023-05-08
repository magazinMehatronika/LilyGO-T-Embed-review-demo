#include <FastLED.h>
#include <TFT_eSPI.h> 
#include "fonts.h"
#include <RotaryEncoder.h>
#include "Seeed_BME280.h"
#include <Wire.h>

#define DATA_PIN 42
#define CLOCK_PIN 45
#define NUM_LEDS 7
#define PIN_IN1 2
#define PIN_IN2 1

int angle=270;
RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);
TFT_eSPI tft = TFT_eSPI(); 
TFT_eSprite img = TFT_eSprite(&tft);
BME280 bme280;
CRGB leds[NUM_LEDS];

void read_encoder() {

  static int pos = 0;
  encoder.tick();
  int newPos = encoder.getPosition();
  if (pos != newPos) {
    if(newPos>pos)
      angle=angle+20;
    if(newPos<pos)
      angle=angle-20;
    pos = newPos;
  } 
  if(angle>=360)
    angle=360;
  if(angle<300)
    angle=300;
}

void setup() {

  delay(3000); 
  
  pinMode(0,INPUT_PULLUP);
  pinMode(46, OUTPUT);
  digitalWrite(46, HIGH);
  
  Wire.begin(43,44);
  
  Serial.begin(9600);
  
  if(!bme280.init()){
    Serial.println("Device error!");
  }
  
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
 
  tft.begin();
  tft.writecommand(0x11);
  tft.setRotation(3);
  tft.fillScreen(TFT_ORANGE);
  tft.setTextDatum(4);
  screenInit(); 
}

void loop() {

  measure();
  if(digitalRead(0)==0){
    if(410-angle==50){
      printPressure();screenInit(); 
    }else if(410-angle==70){
      printTemperature();screenInit(); 
    }else if(410-angle==90){
      printAltitude(); screenInit(); 
    }else if(410-angle==110){
      printHumidity();screenInit(); 
    }
  }
}

void screenInit(){

  leds[0].setRGB(0,165,255); 
  leds[1].setRGB(0,165,255);
  leds[2].setRGB(0,165,255);
  leds[3].setRGB(0,165,255);
  leds[4].setRGB(0,165,255);
  leds[5].setRGB(0,165,255);
  leds[6].setRGB(0,165,255);
  leds[7].setRGB(0,165,255);
  FastLED.show();

  img.createSprite(320, 170);
  img.setTextSize (1);     
  img.setTextDatum(4);   
  img.fillSprite(TFT_ORANGE);   
  img.setTextColor(TFT_BLACK,TFT_ORANGE);
  img.setFreeFont(&FreeSans9pt7b);
  img.drawString("measurement BME280",100,7);

  img.setTextColor(TFT_WHITE,TFT_ORANGE);  
  img.drawString("Pressure: ",50,50);
  img.drawString("Pa",250,50);
  img.drawString("Temp: ",50,70);
  img.drawString("C",250,70);
  img.drawString("Altitude: ",50,90);
  img.drawString("m",250,90);
  img.drawString("Humidity: ",50,110);
  img.drawString("%",250,110);
 
  img.setTextColor(TFT_ORANGE,0xFED1);
  img.drawString("                          by magazinMehatronika    .",150,150);
  img.pushSprite(0, 0); delay(100);
}

void measure()  {

  img.setTextColor(TFT_BLACK,TFT_ORANGE);
  img.setFreeFont(&DSEG7_Classic_Bold_18);  
  img.setTextDatum(5);
  img.drawString(String(bme280.getPressure()),230,53);
  img.drawString(String(bme280.getTemperature()),230,73);
  img.drawString(String(bme280.calcAltitude(bme280.getPressure())),230,93);
  img.drawString(String(bme280.getHumidity()),230,113);
  img.pushSprite(0, 0); 
        
  read_encoder();

  img.setFreeFont(&FreeSans9pt7b);
  img.fillRect(265,40,340,90,TFT_ORANGE);
  img.fillCircle(300,410-angle,6,TFT_BLACK);
  img.pushSprite(0, 0); 
}

void printPressure()  {
    
  leds[0].setRGB(255,0,0); 
  leds[1].setRGB(255,0,0);
  leds[2].setRGB(255,0,0);
  leds[3].setRGB(255,0,0);
  leds[4].setRGB(255,0,0);
  leds[5].setRGB(255,0,0);
  leds[6].setRGB(255,0,0);
  leds[7].setRGB(255,0,0);
  FastLED.show();
  delay(300);
    
  img.deleteSprite();      
  img.createSprite(320, 170);
  img.fillSprite(TFT_BLUE);  
  img.setTextFont(2);
  img.setTextDatum(0);
  img.setTextColor(TFT_BLACK,TFT_BLUE);  
  img.setFreeFont(&FreeSans9pt7b);
  img.drawString("Pressure",10,7);
  img.drawString("Pa",290,90);

  img.setTextColor(TFT_BLUE,0x051F);
  img.setTextDatum(1);
  img.drawString("                          by magazinMehatronika    .",150,150);
  img.pushSprite(0, 0); 
  
  img.setTextSize (3);
  img.setTextColor(TFT_WHITE,TFT_BLUE);
  img.setFreeFont(&DSEG7_Classic_Bold_18);  
  img.setTextDatum(5);     
    
  while(digitalRead(0)!=0){
    img.drawString(String(bme280.getPressure()),280,80);
    read_encoder();delay(100); 
    img.pushSprite(0, 0); 
  }
}

void printTemperature(){
      
  leds[0].setRGB(0,0,255); 
  leds[1].setRGB(0,0,255);
  leds[2].setRGB(0,0,255);
  leds[3].setRGB(0,0,255);
  leds[4].setRGB(0,0,255);
  leds[5].setRGB(0,0,255);
  leds[6].setRGB(0,0,255);
  leds[7].setRGB(0,0,255);
  FastLED.show();
  delay(300);

  img.deleteSprite();      
  img.createSprite(320, 170);
  img.setTextFont(2);
  img.setTextDatum(0); 
  img.fillSprite(TFT_RED);   
  img.setTextColor(TFT_BLACK,TFT_RED);
  img.setFreeFont(&FreeSans9pt7b);
  img.drawString("Temperature",10,7);
  img.drawString("C",290,90);

  img.setTextDatum(1);
  img.setTextColor(TFT_RED,0xFCB2);
  img.drawString("                          by magazinMehatronika    .",150,150);
  img.setTextSize (3);
  img.pushSprite(0, 0); 
  
  img.setTextColor(TFT_WHITE,TFT_RED);
  img.setFreeFont(&DSEG7_Classic_Bold_18);  
  img.setTextDatum(5);     
    
  while(digitalRead(0)!=0){
    img.drawString(String(bme280.getTemperature()),280,80);
    read_encoder();delay(100); 
    img.pushSprite(0, 0); 
  }
}

void printAltitude(){
        
  leds[0].setRGB(255,0,255); 
  leds[1].setRGB(255,0,255);
  leds[2].setRGB(255,0,255);
  leds[3].setRGB(255,0,255);
  leds[4].setRGB(255,0,255);
  leds[5].setRGB(255,0,255);
  leds[6].setRGB(255,0,255);
  leds[7].setRGB(255,0,255);
  FastLED.show();
  delay(300);
       
  img.deleteSprite();      
  img.createSprite(320, 170);
  img.setTextFont(2);
  img.setTextDatum(0);
  img.fillSprite(TFT_PURPLE);   
  img.setTextColor(TFT_BLACK,TFT_PURPLE);
  img.setFreeFont(&FreeSans9pt7b);
  img.drawString("Altitude",10,7);
  img.drawString("m",290,90);

  img.setTextDatum(1);
  img.setTextColor(TFT_GREEN, TFT_PINK);
  img.drawString("                          by magazinMehatronika    .",150,150);
  img.setTextSize (3);
  img.pushSprite(0, 0); 

  img.setTextColor(TFT_WHITE,TFT_PURPLE);
  img.setFreeFont(&DSEG7_Classic_Bold_18);  
  img.setTextDatum(5);      
    
  while(digitalRead(0)!=0){
    img.drawString(String(bme280.calcAltitude(bme280.getPressure())),280,80);
    read_encoder();delay(100); 
    img.pushSprite(0, 0); 
  }
}

void printHumidity(){
        
  leds[0].setRGB(0,255,0); 
  leds[1].setRGB(0,255,0);
  leds[2].setRGB(0,255,0);
  leds[3].setRGB(0,255,0);
  leds[4].setRGB(0,255,0);
  leds[5].setRGB(0,255,0);
  leds[6].setRGB(0,255,0);
  leds[7].setRGB(0,255,0);
  FastLED.show();
  delay(300);
      
  img.deleteSprite();      
  img.createSprite(320, 170);
  img.fillSprite(TFT_GREEN); 
  img.setTextFont(2);
  img.setTextDatum(0);
  img.setTextColor(TFT_BLACK,TFT_GREEN);    
  img.setTextColor(TFT_BLACK,TFT_GREEN);
  img.setFreeFont(&FreeSans9pt7b);
  img.drawString("Humidity",10,7);
  img.drawString("%",290,90);

  img.setTextDatum(1);
  img.setTextColor(TFT_GREEN, 0xCFF9);
  img.drawString("                          by magazinMehatronika    .",150,150);
  img.setTextSize (3);
  img.pushSprite(0, 0); 

  img.setTextColor(TFT_WHITE,TFT_GREEN);
  img.setFreeFont(&DSEG7_Classic_Bold_18);  
  img.setTextDatum(5);     
    
  while(digitalRead(0)!=0){
    img.drawString(String(bme280.getHumidity()),280,80);
    read_encoder();delay(100); 
    img.pushSprite(0, 0); 
  }
}
