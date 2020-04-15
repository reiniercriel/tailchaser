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

FrameBucket::FrameBucket( const Aws::String &bucketName, const Aws::String &region)
: m_bucketName(bucketName)
, m_isValid(bucketName.size() != 0)
, m_objectCount(0)
{}


bool FrameBucket::addFrame(const cv::Mat &frame) {
    std::string file_name = "/home/../projects/tailchaser/device/storagemanager/lena.jpg";
    bool success = true;
    if (!bucketExists()) {
        success = createBucket();
    }
    if (success) {
        Aws::SDKOptions options;
        Aws::InitAPI(options);
        {
            Aws::Client::ClientConfiguration client_configuration;
            Aws::S3::S3Client s3_client(client_configuration);
            Aws::S3::Model::PutObjectRequest object_request;

            Aws::Map<Aws::String, Aws::String> metaData = buildMetaData();

            object_request.SetMetadata(metaData);
            object_request.SetBucket(m_bucketName);
            object_request.SetKey(nextObjectName());
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
        Aws::ShutdownAPI(options);
    }
    return success;
}

bool FrameBucket::bucketExists() {
    bool retVal = false;
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    {
        Aws::Client::ClientConfiguration client_configuration;
        Aws::S3::S3Client s3_client(client_configuration);
        auto outcome = s3_client.ListBuckets();

        if (outcome.IsSuccess()) {
            Aws::Vector<Aws::S3::Model::Bucket> bucket_list =
            outcome.GetResult().GetBuckets();

            for (auto const &bucket: bucket_list) {
                if (bucket.GetName() == m_bucketName) {
                    retVal = true;
                    break;
                }
            }
        }
    }
    Aws::ShutdownAPI(options);
    return retVal;
}

bool FrameBucket::createBucket() {
    bool retVal = true;

    Aws::S3::Model::CreateBucketRequest request;
    request.SetBucket(m_bucketName);

    Aws::SDKOptions options;
    Aws::InitAPI(options);
    {
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
    }

    Aws::ShutdownAPI(options);
    return retVal;
}

Aws::String FrameBucket::nextObjectName() {
    std::string s = "frame_" + std::to_string(m_objectCount);
    Aws::String aws_s(s.c_str(), s.size());
    return aws_s;
}

Aws::Map<Aws::String, Aws::String> FrameBucket::buildMetaData() {
    Aws::Map<Aws::String, Aws::String> retVal;
    retVal.insert( std::pair<Aws::String, Aws::String>("Content-Type", "application/octet-stream") );
    retVal.insert( std::pair<Aws::String, Aws::String>("x-amz-meta-fieldname", "image") );
    return retVal;
}
