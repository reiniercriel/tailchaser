#include <aws/core/Aws.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>

using namespace cv;
using namespace std;

/**
 * Is the local counter part of a bucket on S3
 * This class is responsible for connectingor creating the bucket and adding
 * objects to the buckets*/
class FrameBucket {
public:
    FrameBucket();
    ~FrameBucket(){};
    bool addFrame(const cv::Mat &frame, const std::string &frameName);

private:
    bool bucketExists(const Aws::String &bucketName);
    bool createBucket(const Aws::String &bucketName);

    /**
     * Generate the meta data to be inserted in the put request.
     */
    Aws::Map<Aws::String, Aws::String> buildMetaData();

    /**
     * @brief Generates the current timestamp as milliseconds since EPOCH
     */
    unsigned long long generateCurrentTimeStamp();

    /**
     * @brief This function will check whether enough time has passed since the last write to S3.
     * The only purpose of this function is to make sure we are not flooding S3 and get some
     * unexpected high bills.
     *
     */
    bool hasEnoughTimePassedSinceLastWrite();

private:
    unsigned long long m_previousWriteTime;
};
