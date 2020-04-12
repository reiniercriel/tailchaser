#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <unistd.h>

using namespace cv;
using namespace std;

class MotionDetector {

public:
	MotionDetector(){};
	~MotionDetector(){};
	
	void start(void(*motionCallback)(cv::Mat,std::string));	

};
