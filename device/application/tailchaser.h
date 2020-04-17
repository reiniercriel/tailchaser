#include "../motiondetector/motiondetector.h"
#include "../storagemanager/bucket.h"

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

class TailChaser {
public:
    TailChaser();
    ~TailChaser();

    int start();
    void stop();

public:
    static void on_motionDetected(const cv::Mat&, const std::string&);

private:
    void motionDetected(const cv::Mat& frame, const std::string& frameName);

private:
    MotionDetector *m_motionDetector;
    FrameBucket *m_frameBucket;
};
