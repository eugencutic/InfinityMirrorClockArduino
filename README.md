# InfinityMirrorClockArduino
Components:
  Arduino Nano
  WS2812B led strip
  DS3231 real-time clock module
  2 buttons
  1 piezo buzzer
  32x32 Mirror
  32x32 glass covered with reflective foil
  Hobbyglass
Power:
  5V 3A for arduino and led strip
  3V battery for RTC module
  
Implementation:
  Takes the output from the module and maps it to leds accordingly, as so: everytime the second changes all leds up until the led corresponding to the second (except the one corresponding to hour and minute) turn green, then the minute led turns blue, and in the end the hour led turns red so that on overlapping the hour led is shown. Hours are mapped so that every time 60 minutes pass, the hour led goes 5 positions ahead.
    On setting time or alarm, the process is inverted, moving the leds being controlled by the 2 buttons (one changes position, the other one works as a confirmation button).
    *Video : https://drive.google.com/file/d/1iHv9qPqJetNf6Eqznq9GCI2GCNZWBfAh/view?usp=drivesdk*
  

  
