#include "gmCounter.hpp"

gmCounter::gmCounter(int conversionIndex = 0, double deadTime_us = 0.0) {
  counts = 0;
  for (int i = 0; i < COUNTER_BUCKETS; i++)
    bucket[i] = 0;
  current_bucket = 0;
  init = COUNTER_BUCKETS;
  convIdx = conversionIndex;
  deadTime = deadTime_us;
}

void gmCounter::bucketSwitch() {
  if (init > 0) init--;

  current_bucket++;
  if (current_bucket >= COUNTER_BUCKETS)
    current_bucket = 0;

  bucket[current_bucket] = 0;
}

unsigned long gmCounter::getCPM() {
  unsigned long cpm = 0;
  for (int i = 0; i < COUNTER_BUCKETS; i++) {
    if (i == current_bucket)
      continue;
    
    cpm += bucket[i];
  }
  return cpm;
}

double gmCounter::compensateDeadTime(unsigned long countbase) {
  return countbase * (1.0 - (double)countbase * deadTime / 1000000.0);
}

void gmCounter::tubePulse() {
  counts++;
  bucket[current_bucket]++;
}

unsigned long gmCounter::getCounts() {
  return counts;
}

double gmCounter::calcDose() {
  return (double)counts / (double)(convIdx * 60);
}

double gmCounter::calcDoseRate() {
  return compensateDeadTime(getCPM()) / (double)convIdx;
}
