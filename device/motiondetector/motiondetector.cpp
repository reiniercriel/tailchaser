#include "motiondetector.h"
#include <string>
#include <stdio.h>
#include <chrono>
#include <ctime>

using namespace cv;
using namespace std;

void MotionDetector::operator()(void(*motionCallback)(const cv::Mat&)) {
    Mat frame, gray, frameDelta, thresh, firstFrame;
    vector<vector<Point> > cnts;
    
    VideoCapture camera;

    // We should only have one camera connected
    int deviceId = 0;
    // Any API
    int apiId = cv::CAP_ANY;
    
    camera.open(deviceId + apiId);
    
    if (!camera.isOpened()) {
	printf("Error: Unable to open camera!");
	return;
    }

    camera.read(frame);
    
    // At startup, the first frame is the ground truth
    // We will want this to change, in case something moves into the
    // frame and stays there.
    cv::Mat groundTruth = frame;    
    preprocess(groundTruth);

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
            if(contourArea(cnts[i]) >= 500) {
		groundTruth = candidate;
		preprocess(groundTruth);
		motionCallback(candidate);
		break;
	    }
        }
        
        if(waitKey(1) == 27){
            //exit if ESC is pressed
            break;
        }    
    }
}

void MotionDetector::preprocess(cv::Mat &frame) {
    // Make into a grayscale image
    cvtColor(frame, frame, COLOR_BGR2GRAY);
    
    // Blur to reduce fine details
    // Use a fairly large kernel
    GaussianBlur(frame, frame, Size(21, 21), 0);
}

