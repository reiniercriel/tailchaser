#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <unistd.h>

using namespace cv;
using namespace std;

class MotionDetector {

public:
	MotionDetector(){};
	~MotionDetector(){};
	
	void operator()(void(*motionCallback)(const cv::Mat&));
	
private:
	/**
	 * This function will pre-process the frame before running
	 * computations on the frames. This should reduce the noise and very
	 * slight differences between images that should not be captured by
	 * the motion detector
	 */
	void preprocess(cv::Mat &);
};
