#define COUNTER_BUCKETS 31
#define BUCKET_TIME_MS 2000L

class gmCounter {
  public:
    gmCounter(int);
    void tubePulse();
    void bucketSwitch();
    unsigned long getCounts();
    unsigned long getCPM();
    int getRemainingInit() { return init; }

    double calcDose();
    double calcExposure();

  private:
    unsigned long counts;
    unsigned int bucket[COUNTER_BUCKETS];
    int current_bucket;
    int init;

    int convIdx;
};
