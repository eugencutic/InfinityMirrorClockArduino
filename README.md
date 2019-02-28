# InfinityMirrorClockArduino
Components:
  Arduino Nano
  WS2812B led strip
  DS3231 real-time clock module
  2 buttons
  1 piezo buzzer
Power:
  5V 3A for arduino and led strip
  3V battery for RTC module
  
Implementation:
  Takes the output from the module and maps it to leds accordingly, as so: everytime the second changes all leds up until the led corresponding to the second (except the one corresponding to hour and minute) turn green, then the minute led turns blue, and in the end the hour led turns red so that on overlapping the hour led is shown. Hours are mapped so that every time 60 minutes pass, the hour led goes 5 positions ahead.
  

  
