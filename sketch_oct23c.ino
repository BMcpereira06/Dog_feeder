#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "lcd.h"

const byte RESET_BUTTON = 2;
const byte SENSOR_MOV = 0;
const int TIME_SHOWING_TEXT_MS = 9000
const int RESET_TIME_MS = 5000 
unsigned long startMillis{};
unsigned long elapsedMillis{};
bool started = false;

void setup() 
{
  pinMode(RESET_BUTTON, INPUT_PULLUP);
  pinMode(SENSOR_MOV, INPUT_PULLUP);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);


}

void loop() 
{

  if(!digitalRead(RESET_BUTTON))
  {
    // reset time
    display.clearDisplay();
    display.setCursor(22, 0);
    display.println("Reseted.");
    display.display(); 
    delay(TIME_SHOWING_TEXT_MS);
    started = false;
    startMillis = 0;
  }
  else
  {
      if(!started)
      {
      startTimer();
      started = true;
      }
      if(!digitalRead(SENSOR_MOV))
      {
      // Shows time
        display.setCursor(0, 0);
        display.clearDisplay();
      

        display.drawBitmap(96, 0, full_battery, 29, 16, WHITE);
        display.setCursor(0, 20);
    
        display.println("Thank you!\n");
        display.setCursor(0, 45);
        display.println(getElapsed());
        display.display(); 
        delay(RESET_TIME_MS);
      }
      else
      {
          display.clearDisplay();
          display.display(); 
      }
  }
  
  
     
 }


long readVcc() { 
  long result; // Read 1.1V reference against AVcc 
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1); 
  delay(2); // Wait for Vref to settle 
  ADCSRA |= _BV(ADSC); // Convert 
  while (bit_is_set(ADCSRA,ADSC)); 
  result = ADCL; 
  result |= ADCH<<8; 
  result = 1126400L / result;
  return result;
}


void startTimer()
{
  startMillis = millis();
}

String getElapsed()
{
  elapsedMillis = millis() - startMillis;
  return getMillisInTime();
}

String getMillisInTime()
{
    unsigned long durSS = (elapsedMillis/1000)%60;    //Seconds
    unsigned long durMM = (elapsedMillis/(60000))%60; //Minutes
    unsigned long durHH = (elapsedMillis/(3600000));  //Hours
    durHH = durHH % 24; 
    String dataTemp = "";

  if (durHH < 10)
  {
    dataTemp = dataTemp + "0" + String(durHH)+ "h:";
  }
  else{
    dataTemp = dataTemp + String(durHH)+ "h:";
  }
  
  if (durMM < 10)
  {
    dataTemp = dataTemp + "0" + String(durMM)+ "m:";
  }
  else{
    dataTemp = dataTemp + String(durMM)+ "m:";
  }
  
  if (durSS < 10)
  {
    dataTemp = dataTemp + "0" + String(durSS)+ "s";
  }
  else{
    dataTemp = dataTemp + String(durSS)+ "s";
  }
  
  return dataTemp;
}
