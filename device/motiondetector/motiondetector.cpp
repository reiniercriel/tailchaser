#include "motiondetector.h"
#include <string>

using namespace cv;
using namespace std;

void MotionDetector::start(void(*motionCallback)(cv::Mat, std::string)) {
	
	Mat frame, gray, frameDelta, thresh, firstFrame;
    vector<vector<Point> > cnts;
    VideoCapture camera(0); //open camera
    
    //set the video size to 512x288 to process faster
    camera.set(3, 512);
    camera.set(4, 288);

    sleep(3);
    camera.read(frame);

    //convert to grayscale and set the first frame
    cvtColor(frame, firstFrame, COLOR_BGR2GRAY);
    GaussianBlur(firstFrame, firstFrame, Size(21, 21), 0);

	int j(0);
    while(camera.read(frame)) {
	
        //convert to grayscale
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, gray, Size(21, 21), 0);

        //compute difference between first frame and current frame
        absdiff(firstFrame, gray, frameDelta);
        threshold(frameDelta, thresh, 25, 255, THRESH_BINARY);
        
        dilate(thresh, thresh, Mat(), Point(-1,-1), 2);
        findContours(thresh, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for(int i = 0; i< cnts.size(); i++) {
            if(contourArea(cnts[i]) < 500) {
                continue;
            }
			std::string title = std::to_string(j++);
			motionCallback(frame, title);

			//convert to grayscale and set the first frame
			cvtColor(frame, firstFrame, COLOR_BGR2GRAY);
			GaussianBlur(firstFrame, firstFrame, Size(21, 21), 0);
        }
        
        if(waitKey(1) == 27){
            //exit if ESC is pressed
            break;
        }
    
    }
	
}
