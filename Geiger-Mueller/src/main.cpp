#include <Arduino.h>
#include <SPI.h>
#include <ESP8266TimerInterrupt.h>

#include <gmCounter.h>
#include <version.h>
#include "display.cpp"

#define GM_PULSE_GPIO 2

ESP8266Timer timer;
gmCounter counter(GM_TUBE_J305_CI, 190);
Display display(VERSION_SHORT);

IRAM_ATTR void gmPulse() {
  counter.tubePulse();
}

IRAM_ATTR void timerHandler() {
  counter.bucketSwitch();
}

void setup() {

  Serial.begin(9600);
  display.splash();
  delay(500);

  Serial.println(F("\n"));
  Serial.print(F("Geiger-Mueller counter ")); Serial.println(VERSION);

  attachInterrupt(GM_PULSE_GPIO, gmPulse, FALLING);
  Serial.println(F("Counter started."));

  if (timer.attachInterruptInterval(BUCKET_TIME_MS * 1000, timerHandler)) {
    Serial.println(F("Bucket timer started."));
  } else {
    Serial.println(F("Failed to start bucket timer."));
  }
}

void loop() {
  delay(1000);
  Serial.printf("%ld: counts: %ld\tdose: %lf uSv", millis(), counter.getCounts(), counter.calcDose());
  if (counter.getRemainingInit()) {
    Serial.print(F("\t.INIT."));

    display.initScreen(counter.getRemainingInit(), COUNTER_BUCKETS);
  }
  else {
    Serial.printf("\tcpm: %ld\tdoseRate: %lf uSv/h", counter.getCPM(), counter.calcDoseRate());
    display.runDisplay(counter.calcDoseRate(), counter.calcDose(), counter.getCPM());
  }
  Serial.println();
}
