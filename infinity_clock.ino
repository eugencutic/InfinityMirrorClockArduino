#include "FastLED.h"
#include <DS3231.h>
#include <Wire.h>

#define NUM_LEDS 60
#define DATA_PIN 5

#define BUTTON_1_PIN 9  //set time
#define BUTTON_2_PIN 11 //move minutes and hours or set alarm

#define PIEZO_PIN 7

CRGB leds[NUM_LEDS];
CRGB RED, GREEN, BLUE, BLACK; //used for comparison 

DS3231 rtc(SDA, SCL);

Time currentTime;
int oldSec = 0, oldMin = 0, oldHour = 0;

bool showingTime = true, settingTime = false, settingAlarm = false, alarmRinging = false;

int button1State, button2State;
int lastButton1State, lastButton2State;
int lastDebounceTime1 = 0, lastDebounceTime2 = 0;
int debounceDelay = 50;

int minOrHour = 0; //deciding which led to move

Time alarm;
int alarmStartTime, alarmDuration = 10000, toneDelay = 500, lastToneTime = 0;

void setup() {
  Serial.begin(9600);
  
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  rtc.begin();

  pinMode(BUTTON_1_PIN, INPUT);
  pinMode(BUTTON_2_PIN, INPUT);

  RED = CRGB::Red;
  GREEN = CRGB::Green;
  BLUE = CRGB::Blue; 
  BLACK = CRGB::Black;
}

void loop() {

  if (showingTime) {
    showTime();

    if (alarmRinging){

      if (millis() - lastToneTime > toneDelay) {
        lastToneTime = millis();
        tone(PIEZO_PIN, 440, 250);
      }
      if (millis() - alarmStartTime > alarmDuration) {
        alarmRinging = false;
      }
      
      int reading = digitalRead(BUTTON_1_PIN);
  
      if (reading != lastButton1State)       
      {
      lastDebounceTime1 = millis();        
      }
    
      if ((millis() - lastDebounceTime1) > debounceDelay)  
      {
        if (reading != button1State)     
        {
              button1State = reading;        
      
              if (button1State == HIGH)      
              {
                alarmRinging = false;
              }
        }
      }
      lastButton1State = reading;

    } else {
      int reading = digitalRead(BUTTON_1_PIN);
  
      if (reading != lastButton1State)       
      {
      lastDebounceTime1 = millis();        
      }
    
      if ((millis() - lastDebounceTime1) > debounceDelay)  
      {
        if (reading != button1State)     
        {
              button1State = reading;        
      
              if (button1State == HIGH)      
              {
                showingTime = false;
                settingTime = true;
                settingAlarm = false;
              }
        }
      }
      lastButton1State = reading;
  
      reading = digitalRead(BUTTON_2_PIN);
      
      if (reading != lastButton2State)       
      {
      lastDebounceTime2 = millis();        
      }
    
      if ((millis() - lastDebounceTime2) > debounceDelay)  
      {
        if (reading != button2State)     
        {
              button2State = reading;        
      
              if (button2State == HIGH)      
              {
                showingTime = false;
                settingTime = false;
                settingAlarm = true;
              }
        }
      }
      lastButton2State = reading;
    }
  }
  if(settingTime)
    getTimeInput();
  if(settingAlarm)
    getAlarmInput();
}


void showTime() {

  currentTime = rtc.getTime();

  if (currentTime.sec == oldSec &&
      currentTime.min == oldMin &&
      currentTime.hour == oldHour)
      return;

  if (currentTime.sec == alarm.sec &&
      currentTime.min == alarm.min &&
      currentTime.hour == alarm.hour) {
        alarmRinging = true;
        alarmStartTime = millis();
      }
      

  oldSec = currentTime.sec;
  oldMin = currentTime.min;
  oldHour = currentTime.hour;

  
  
  for (int i = 59; i >= 59 - currentTime.sec; i--) {
      leds[i] = CRGB::Green;
  }

  for (int i = 59 - currentTime.sec - 1; i >= 0; i--) {
      leds[i] = CRGB::Black;
  }

  leds[59 - currentTime.min] = CRGB::Blue; //if overlapping, hour is shown
  leds[59 - (currentTime.hour % 12) * 5] = CRGB::Red;

  FastLED.show();
  
}

void getTimeInput() {

  minOrHour = 0; // start by moving minute led

  while(minOrHour < 2)
    moveClock();

  int secToSet = 0, minToSet = -1, hourToSet = 0;
  for (int i = 0; i <= 59; i++) {
    if (leds[i] == BLUE)
      minToSet = i;
    if (leds[i] == RED)
      hourToSet = i;
  }
  if (minToSet == -1)
    minToSet = hourToSet;

  minToSet = 59 - minToSet;
  hourToSet = (59 - hourToSet) / 5;

  rtc.setTime(hourToSet, minToSet, secToSet);
  
  showingTime = true;
  settingTime = false;
  settingAlarm = false;
}

void getAlarmInput() {

  minOrHour = 0; // start by moving minute led

  while(minOrHour < 2)
    moveClock();

  int secToSet = 0, minToSet = -1, hourToSet = 0;
  for (int i = 0; i <= 59; i++) {
    if (leds[i] == BLUE)
      minToSet = i;
    if (leds[i] == RED)
      hourToSet = i;
  }
  if (minToSet == -1)
    minToSet = hourToSet;

  minToSet = 59 - minToSet;
  hourToSet = (59 - hourToSet) / 5;

  alarm.hour = hourToSet;
  alarm.min = minToSet;
  alarm.sec = secToSet;

  showingTime = true;
  settingAlarm = false;
  settingTime = false;
}

void moveClock() {
  
  int reading2 = digitalRead(BUTTON_2_PIN);
    
  if (reading2 != lastButton2State)       
  {
  lastDebounceTime2 = millis();        
  }

  if ((millis() - lastDebounceTime2) > debounceDelay)  
  {
    if (reading2 != button2State)     
    {
          button2State = reading2;        
  
          if (button2State == HIGH)      
          {
            
            moveLed();
          }
    }
  }
  lastButton2State = reading2;

  int reading1 = digitalRead(BUTTON_1_PIN);

  if (reading1 != lastButton1State)       
  {
  lastDebounceTime1 = millis();        
  }

  if ((millis() - lastDebounceTime1) > debounceDelay)  
  {
    if (reading1 != button1State)     
    {
          button1State = reading1;        
  
          if (button1State == HIGH)      
          {
              minOrHour++;
          }
    }
  }
  lastButton1State = reading1;
}

void moveLed() {
  bool overlapping = true;

  for (int i = 0; i <= 59; i++) {
    if (leds[i] == BLUE)
      overlapping = false;
    if (leds[i] == GREEN)
      leds[i] = CRGB::Black;
  }
    
  for (int i = 0; i <= 59; i++) {
    int nextMinLed = i - 1 == -1 ? 59 : i - 1;
    int nextHourLed = i - 5 ==  -1 ? 59 : i - 5;
    if (overlapping) {
      
      if (minOrHour == 0) {
        if (leds[i] == RED) {
          leds[nextMinLed] = CRGB::Blue;
          FastLED.show();
          return;
        }
      } else {
        if (leds[i] == RED) {
          leds[i] = CRGB::Blue;
          leds[nextHourLed] = CRGB::Red;
          FastLED.show();
          return;
        }
      }
    } else {
      if (minOrHour == 0) {
        if (leds[i] == BLUE) {
          leds[i] = CRGB::Black;
          if (leds[nextMinLed] != RED)
            leds[nextMinLed] = CRGB::Blue;
            FastLED.show();
          return;
        }
      } else {
        if (leds[i] == RED) {
          leds[i] = CRGB::Black;
          leds[nextHourLed] = CRGB::Red;
          FastLED.show();
          return;
        }
      }
    }
  }
  
}
