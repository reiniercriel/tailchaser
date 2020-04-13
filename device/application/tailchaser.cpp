#include "tailchaser.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <unistd.h>
#include <thread>

using namespace cv;
using namespace std;

TailChaser *caller = nullptr;
void TailChaser::on_motionDetected(const cv::Mat& frame) {
    if (caller) {
        caller->motionDetected(frame);
    }
}

TailChaser::TailChaser() {
    // Init
}

TailChaser::~TailChaser() {
    // Make sure all threads are properly closed
}

int TailChaser::start() {
    caller = this;
    std::thread mainThread(MotionDetector(),TailChaser::on_motionDetected);
    mainThread.join();
    return 1;
}

void TailChaser::stop() {
    // Stop MotionDetection from different thread
}


void TailChaser::motionDetected(const cv::Mat &frame) {
    putText(frame, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
    imshow("Movement detected", frame);
}
