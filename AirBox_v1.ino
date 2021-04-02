/*
  AirBox
  ------
  Read the values of a DSM501A air quality sensor via an arduino and match the color of the neoleds with the waring level
  
  version: 1.0
  author: Olivier Decru
  date: 02/04/2021
*/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

#define PIN 12
#define NUMPIXELS 5
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

int pin2 = 3;
int pin1 = 2;
unsigned long duration1;
unsigned long duration2;

unsigned long starttime;
unsigned long sampletime_ms = 3000;
unsigned long lowpulseoccupancy1 = 0;
unsigned long lowpulseoccupancy2 = 0;
float ratio1 = 0;
float ratio2 = 0;
float concentration1 = 0;
float concentration2 = 0;

void setup() {
  Serial.begin(9600);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(12,OUTPUT);
  starttime = millis();//get the current time;
  pixels.begin();
  
  ledtest(); //test leds during startup
}

void loop() {
  duration1 = pulseIn(pin1, LOW);
  //duration2 = pulseIn(pin2, LOW);
  lowpulseoccupancy1 = lowpulseoccupancy1+duration1;
  //lowpulseoccupancy2 = lowpulseoccupancy2+duration2;

  if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
  {
    getConcentration(lowpulseoccupancy1,sampletime_ms);
    setLedWarning(concentration1);
      
    lowpulseoccupancy1 = 0;
    lowpulseoccupancy2 = 0;
    starttime = millis();
  }
}

float getConcentration(float lowpulseoccupancy, long sampletime){
  float ratio = lowpulseoccupancy/(sampletime*10.0);  // Integer percentage 0=>100
  return 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
}

void setLedWarning(int concentration){
  pixels.clear();
  if (concentration < 1000) {
    setLedColor(255,255,255); //clean: white
  }
  else if (concentration > 1000 && concentration1 < 10000) {
    setLedColor(0,255,0); //good: green
  }
  else if (concentration > 10000 && concentration1 < 20000) {
    setLedColor(255,255,0); //acceptable: yellow
  }
  else if (concentration > 20000 && concentration1 < 50000) {
    setLedColor(255,0,0); //heavy: red
  }
  else if (concentration > 50000 ) {
    setLedColor(0,0,255);  //hazard: blue
  }
  pixels.show();
}

void setLedColor(int R, int G, int B){
  for(int i=0; i<NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(R,G,B));
  }
}

void ledtest(){
  pixels.clear();
  pixels.show();
  delay(500);
  setLedColor(255,255,255); //white
  pixels.show();
  delay(500);
  setLedColor(0,255,0); //green
  pixels.show();
  delay(500);
  setLedColor(255,255,0); //yellow
  pixels.show();
  delay(500);
  setLedColor(255,0,0); //red
  pixels.show();
  delay(500);
  setLedColor(0,0,255); //blue
  pixels.show();
  delay(500);
  pixels.clear();
  pixels.show();
  delay(500);
}
