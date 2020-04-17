#include "motiondetector.h"
#include <string>
#include <stdio.h>
#include <ctime>
#include <time.h>
#include <chrono>
#include <sys/time.h>
#include <unistd.h>

using namespace cv;
using namespace std;
using namespace std::chrono;

const static unsigned long long offset = 60 * 1000; // 60 seconds
const static int contourThresholds = 400;

MotionDetector::MotionDetector()
{}

void MotionDetector::operator()(void(*motionCallback)(const cv::Mat&, const std::string&)) {
    Mat frame, gray, frameDelta, thresh, firstFrame;
    vector<vector<Point> > cnts;

    VideoCapture camera;

    // We should only have (at least) one camera connected
    int deviceId = 0;
    // Any API
    int apiId = cv::CAP_ANY;

    camera.open(deviceId + apiId);

    if (!camera.isOpened()) {
        printf("Error: Unable to open camera!");
        return;
    }

    camera.read(frame);

    // Start time of a 'movement'
    unsigned long long currentMovementTimeStamp = generateCurrentTimeStamp();
    unsigned long long timeStampCurrent = currentMovementTimeStamp;

    // At startup, the first frame is the ground truth
    // We will want this to change, in case something moves into the
    // frame and stays there.
    cv::Mat groundTruth = frame;
    preprocess(groundTruth);

    // This index will keep track of the number of images we send down one bucket, it will be used
    // as a name for the image, so it is easy to veiw them
    int frameIndexForMovement = 0;

    while(camera.read(frame)) {
        cv::Mat candidate = frame;

        preprocess(frame);

        //Now, let's subtract the background
        absdiff(groundTruth, frame, frameDelta);

        // Make all moving pixels white
        threshold(frameDelta, thresh, 25, 255, THRESH_BINARY);
        // Dilation for 2 iterations (Add border to objects in image)
        // 3x3 default kernel
        dilate(thresh, thresh, Mat(), Point(-1,-1), 2);

        // Find the contours of different objects, returns 2D points
        findContours(thresh, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for(int i = 0; i< cnts.size(); i++) {
            // If we discover a big area, callback and return to capture
            if(contourArea(cnts[i]) >= contourThresholds) {
                timeStampCurrent = generateCurrentTimeStamp();

                // If a motion happens within 60 seconds of another motion we consider it part of
                // the same 'movement' and will put the images in the same bucket
                // If it's a new movement, change the bucket name
                if (!isPartOfPreviousMotion(timeStampCurrent, currentMovementTimeStamp, offset)) {
                    currentMovementTimeStamp = timeStampCurrent;
                    frameIndexForMovement = 0;
                }

                groundTruth = candidate;
                preprocess(groundTruth);
                std::string frameName = std::to_string(currentMovementTimeStamp)
                                + "/" + std::to_string(++frameIndexForMovement) + ".jpg";
                motionCallback(candidate, frameName);
                break;
            }
        }

        // Give the device a breather
        usleep(200);
    }
}

unsigned long long MotionDetector::generateCurrentTimeStamp(){
    struct timeval time;

    gettimeofday(&time, NULL);
    unsigned long long millisecondsSinceEpoch = (unsigned long long)(time.tv_sec) * 1000 +
                                                (unsigned long long)(time.tv_usec) / 1000;
    return millisecondsSinceEpoch;
}

bool MotionDetector::isPartOfPreviousMotion(const unsigned long long &currentTimeStamp,
                                            const unsigned long long &startTimeStamp,
                                            const unsigned long long &offset){
    return ((currentTimeStamp-startTimeStamp) < offset);
}

void MotionDetector::preprocess(cv::Mat &frame) {
    // Make into a grayscale image
    cvtColor(frame, frame, COLOR_BGR2GRAY);

    // Blur to reduce fine details
    // Use a fairly large kernel
    GaussianBlur(frame, frame, Size(21, 21), 0);
}
