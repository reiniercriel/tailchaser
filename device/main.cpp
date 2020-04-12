#include <opencv2/opencv.hpp>
//#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <unistd.h>
#include "motiondetector/motiondetector.h"

using namespace cv;
using namespace std;

void motionCallback(cv::Mat frame) {
    putText(frame, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
    imshow("CALLBACK", frame);
}

int main(int argc, char **argv) {

    MotionDetector detector;
    detector.start(motionCallback);

    return 0;
}
