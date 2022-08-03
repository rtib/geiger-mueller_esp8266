#define COUNTER_BUCKETS 31
#define BUCKET_TIME_MS 2000L

/**
 * @brief Geiger-Mueller Counter class
 * 
 */
class gmCounter {
  public:

    /**
     * @brief Construct a new gmCounter object
     * 
     * @param conversionIndex CI of the tube
     * @param deadTime_us dead time in microseconds
     */
    gmCounter(int, double);
    
    /**
     * @brief event hander of a tube pulse
     * 
     */

    void tubePulse();

    /**
     * @brief switch to the next counter bucket
     * 
     */    
    void bucketSwitch();

    /**
     * @brief get the actual counts, uncompensated
     * 
     * @return unsigned long 
     */
    unsigned long getCounts();

    /**
     * @brief compensate dead time on a count
     * 
     * @param countbase uncompensated count or rate
     * @return double dead time compensated count
     */
    double compensateDeadTime(unsigned long);

    unsigned long getCPM();
    int getRemainingInit() { return init; }

    /**
     * @brief calculate cumulated dose in microsieverts
     * 
     * @return double dose
     */
    double calcDose();

    /**
     * @brief calculate the actual dose rate over the last minute
     * 
     * @return double dose rate in microsieverts per hour
     */
    double calcDoseRate();

  private:
    unsigned long counts;
    unsigned int bucket[COUNTER_BUCKETS];
    int current_bucket;
    int init;

    int convIdx;
    double deadTime;
};
