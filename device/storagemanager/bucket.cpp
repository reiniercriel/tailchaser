#include "bucket.h"
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/Bucket.h>
#include <aws/s3/model/CreateBucketConfiguration.h>
#include <aws/s3/model/CreateBucketRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <time.h>
#include <chrono>
#include <sys/time.h>

const unsigned long long MinimumTimeBetweenWritesMs = 2 *1000; // 2 seconds

FrameBucket::FrameBucket()
    : m_previousWriteTime(generateCurrentTimeStamp())
{}

bool FrameBucket::addFrame(const cv::Mat& frame, const std::string& frameName) {
    if (!hasEnoughTimePassedSinceLastWrite()) return false;
    bool success = true;

    std::string file_name = "temp.jpg";
    imwrite( file_name, frame );

    Aws::String awsBucketName("tailchaser");
    Aws::String awsFrameName(frameName.c_str(), frameName.size());

    cout << "***********************" << endl;
    cout << "* Writing frame to S3.." << endl;
    cout << "* - BucketName = " << awsBucketName << endl;
    cout << "* - Frame = " << frameName << endl;
    cout << "***********************" << endl << endl;

    Aws::SDKOptions options;
    Aws::InitAPI(options);
    {
        if (!bucketExists(awsBucketName)) {
            success = createBucket(awsBucketName);
        }

        if (success) {
            Aws::Client::ClientConfiguration client_configuration;
            Aws::S3::S3Client s3_client(client_configuration);
            Aws::S3::Model::PutObjectRequest object_request;
            Aws::Map<Aws::String, Aws::String> metaData = buildMetaData();

            object_request.SetMetadata(metaData);
            object_request.SetBucket(awsBucketName);
            object_request.SetKey(awsFrameName);

            const std::shared_ptr<Aws::IOStream> input_data =
            Aws::MakeShared<Aws::FStream>("TailChaser", file_name.c_str(),
            std::ios_base::in | std::ios_base::binary);
            object_request.SetBody(input_data);

            // Put the object
            auto put_object_outcome = s3_client.PutObject(object_request);
            if (!put_object_outcome.IsSuccess()) {
                auto error = put_object_outcome.GetError();
                std::cout << "ERROR: " << error.GetExceptionName() << ": "
                << error.GetMessage() << std::endl;
                success = false;
            }
        }
    }
    Aws::ShutdownAPI(options);
    m_previousWriteTime = generateCurrentTimeStamp();

    cout << "****************************" << endl;
    cout << "* Done writing frame to S3.." << endl;
    cout << "* - Status = " << (success ? "SUCCESS" : "FAILED")  << endl;
    cout << "****************************" << endl << endl;

    return success;
}

bool FrameBucket::bucketExists(const Aws::String &bucketName) {
    bool retVal = false;
    Aws::Client::ClientConfiguration client_configuration;
    Aws::S3::S3Client s3_client(client_configuration);
    auto outcome = s3_client.ListBuckets();

    if (outcome.IsSuccess()) {
        Aws::Vector<Aws::S3::Model::Bucket> bucket_list =
        outcome.GetResult().GetBuckets();

        for (auto const &bucket: bucket_list) {
            if (bucket.GetName() == bucketName) {
                retVal = true;
                break;
            }
        }
    }
    return retVal;
}

bool FrameBucket::createBucket(const Aws::String &bucketName) {
    bool retVal = true;

    Aws::S3::Model::CreateBucketRequest request;
    request.SetBucket(bucketName);

    // Create the bucket
    Aws::S3::S3Client s3_client;
    auto outcome = s3_client.CreateBucket(request);
    if (!outcome.IsSuccess())
    {
        retVal = false;
        auto err = outcome.GetError();
        std::cout << "ERROR: CreateBucket: " <<
        err.GetExceptionName() << ": " << err.GetMessage() << std::endl;
    }
    return retVal;
}

Aws::Map<Aws::String, Aws::String> FrameBucket::buildMetaData() {
    Aws::Map<Aws::String, Aws::String> retVal;
    retVal.insert( std::pair<Aws::String, Aws::String>("Content-Type", "application/octet-stream") );
    retVal.insert( std::pair<Aws::String, Aws::String>("x-amz-meta-fieldname", "image") );
    return retVal;
}

unsigned long long FrameBucket::generateCurrentTimeStamp() {
    struct timeval time;

    gettimeofday(&time, NULL);
    unsigned long long millisecondsSinceEpoch = (unsigned long long)(time.tv_sec) * 1000 +
                                                (unsigned long long)(time.tv_usec) / 1000;
    return millisecondsSinceEpoch;
}

bool FrameBucket::hasEnoughTimePassedSinceLastWrite() {
    unsigned long long currentTime = generateCurrentTimeStamp();
    return ( (currentTime - m_previousWriteTime) >= MinimumTimeBetweenWritesMs );
}
