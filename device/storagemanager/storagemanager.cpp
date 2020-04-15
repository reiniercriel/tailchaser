#include "storagemanager.h"
#include "bucket.h"
#include <aws/core/Aws.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>

using namespace cv;
using namespace std;

void StorageManager::start(){
    cv::Mat frame;
    Aws::String name = "today5";
    FrameBucket bucket(name);
    bucket.addFrame(frame);
}
