#include <string>
#include <opencv2/opencv.hpp>
#include <unistd.h>

class MotionDetector {

public:
    MotionDetector();
    ~MotionDetector(){};

    void operator()(void(*motionCallback)(const cv::Mat&, const std::string&));

private:
    /**
     * @brief Generates the current timestamp as milliseconds since EPOCH
     */
    unsigned long long generateCurrentTimeStamp();

    /**
     * @param currentTimeStamp is the time in MS since EPOCH
     * @param startTimeStamp is the time with which we will be comparing the
     * current time
     * @offset is the difference in time we are looking for
     * @return true if the currentTimeStamp is within offset of the startTimeStamp
     */
    bool isPartOfPreviousMotion(const unsigned long long &currentTimeStamp,
        const unsigned long long &startTimeStamp,
        const unsigned long long &offset);

    /**
    * @brief This function will pre-process the frame before running
    * computations on the frames. This should reduce the noise and very
    * slight differences between images that should not be captured by
    * the motion detector
    */
    void preprocess(cv::Mat &);
};
