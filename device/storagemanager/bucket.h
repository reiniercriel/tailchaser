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
    FrameBucket( const Aws::String &bucketName,
            const Aws::String &region = "us-west-2");
    ~FrameBucket(){};
    bool addFrame(const cv::Mat &frame);

private:
    bool bucketExists();
    bool createBucket();

    /**
     * Generate the next (unique) object name to insert in the buckets
     */
    Aws::String nextObjectName();

    /**
     * Generate the meta data to be inserted in the put request.
     */
    Aws::Map<Aws::String, Aws::String> buildMetaData();
private:
    Aws::String m_bucketName;
    bool m_isValid;
    int m_objectCount;
};
