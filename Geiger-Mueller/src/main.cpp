#include <Arduino.h>
#include <SPI.h>
#include <ESP8266TimerInterrupt.h>
#include <U8g2lib.h>
#include <gmCounter.h>

#define VERSION "1.0"
#define GM_PULSE_GPIO 2

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
ESP8266Timer timer;
gmCounter counter(GM_TUBE_J305_CI);

IRAM_ATTR void gmPulse() {
  counter.tubePulse();
}

IRAM_ATTR void timerHandler() {
  counter.bucketSwitch();
}

void setup() {

  Serial.begin(9600);
  u8g2.begin();
  delay(500);

  Serial.println(F("\n"));
  Serial.print(F("Geiger-Mueller counter V")); Serial.println(VERSION);

  attachInterrupt(GM_PULSE_GPIO, gmPulse, FALLING);
  Serial.println(F("Counter started."));

  if (timer.attachInterruptInterval(BUCKET_TIME_MS * 1000, timerHandler)) {
    Serial.println(F("Bucket timer started."));
  } else {
    Serial.println(F("Failed to start bucket timer."));
  }

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_smart_patrol_nbp_tf);
  u8g2.drawStr(10, 10, "Geiger-M\xFCller");
  u8g2.setFont(u8g2_font_profont11_tr);
  u8g2.drawStr(25, 21, "Version");
  u8g2.drawStr(80, 21, VERSION);
  u8g2.sendBuffer();
}

void loop() {
  delay(1000);
  Serial.printf("count: %ld\tdose: %lf uSv", counter.getCounts(), counter.calcDose());
  if (counter.getRemainingInit()) {
    Serial.print(F("\t.INIT."));

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_smart_patrol_nbp_tf);
    u8g2.drawStr(10,10,"Geiger-M\xFCller");
    u8g2.setFont(u8g2_font_profont11_tr);
    u8g2.drawStr(25, 21, "Version");
    u8g2.drawStr(80, 21, VERSION);
    u8g2.drawFrame(8,25,114,5);
    u8g2.drawHLine(10, 27, 110 * (COUNTER_BUCKETS - counter.getRemainingInit() + 1) / COUNTER_BUCKETS);
    u8g2.sendBuffer();
  }
  else {
    Serial.printf("\tcpm: %ld\texposure: %lf uSv/h", counter.getCPM(), counter.calcExposure());

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenR18_tf);
    u8g2.setCursor(0, 20);
    u8g2.print(counter.calcExposure());
    u8g2.print(F(" \xB5Sv/h"));

    u8g2.setFont(u8g2_font_5x8_tf);
    u8g2.setCursor(0, 31);
    u8g2.print(counter.calcDose()); u8g2.print(F(" \xB5Sv"));

    u8g2.setCursor(75, 31);
    u8g2.print(F("CPM: ")); u8g2.print(counter.getCPM());
    u8g2.sendBuffer();  
  }
  Serial.println();
}
