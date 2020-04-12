#include <opencv2/opencv.hpp>
//#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <unistd.h>
#include <string>
#include "motiondetector/motiondetector.h"

using namespace cv;
using namespace std;

void motionCallback(cv::Mat frame, std::string title) {
    putText(frame, title, Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);

    imshow("Movement detected", frame);
}

int main(int argc, char **argv) {

    MotionDetector detector;
    detector.start(motionCallback);

    return 0;
}
