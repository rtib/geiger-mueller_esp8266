#include <Arduino.h>
#include <unity.h>
#include <gmCounter.h>

void test_gmCounter_singlePulse() {
  gmCounter counter(GM_TUBE_J305_CI);
  counter.tubePulse();
  TEST_ASSERT_EQUAL(1, counter.getCounts());
}

void test_gmCounter_multiplePulse() {
  gmCounter counter(GM_TUBE_J305_CI);
  for (int i = 0; i < 10; i++) {
    counter.tubePulse();
  }
  TEST_ASSERT_EQUAL(10, counter.getCounts());
}

void test_gmCounter_bucketSwitch() {
  gmCounter counter(GM_TUBE_J305_CI);
  TEST_ASSERT_EQUAL(COUNTER_BUCKETS, counter.getRemainingInit());
  for (int i = 0; i < COUNTER_BUCKETS; i++) {
    counter.bucketSwitch();
  }
  TEST_ASSERT_EQUAL(0, counter.getRemainingInit());
}

void setup() {
  delay(2000);
  Serial.begin(9600);
  UNITY_BEGIN();
}

void loop() {
  RUN_TEST(test_gmCounter_singlePulse);
  RUN_TEST(test_gmCounter_multiplePulse);
  RUN_TEST(test_gmCounter_bucketSwitch);
  UNITY_END();
}
