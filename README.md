# TailChaser
A complete project for tracking your pet's movements during the day and uploading, analyzing and showing these movements on the AWS cloud. Please find in this repository the following layers:
* device
* test_staticwebapp
* webapp (TODO)
You will find more detailed explanations about every layer below.

## Component: device

### Description
The device layer of this project is responsible for interacting with the machine on which the application is installed. The code was made for and tested on Rasberry Pi 4 but can also run on similar devices. As long as it is a Unix machine with a camera connected, chances are you can get it working fairly easily.

### Pre-requisites for building (and running) the device application code (Linux only)
1. GNU Compiler Collection (GCC) 4.9 or later
2. AWS-SDK: Build and install from source. See https://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/setup.html for detailed instructions. This package is needed to interact with the S3 bucket on AWS (For uploading images).
Small note about building the SDK. When building the module "aws-c-event-stream", you must manually pass the CMAKE_PREFIX_PATH variable to the CMake Generator. Otherwise you will not be able to successfully generate the files. For example: cmake .. -DCMAKE_PREFIX_PATH="/usr/local/"

3. CMake: Please install the latest version of cmake
4. OpenCV 4: Build and install from source: See https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html for detailed instructions
5. Git
6. libcanberra-gtk-module

### Building the device component
1. Create a build folder.
2. Run CMAKE from the build folder followed by the path to the source directory. e.g. 'CMAKE <path/to/source>'
3. Run MAKE from the build folder. e.g. 'MAKE .'

### Enabling uploads to AWS S3
This code requires an S3 instance to be up and running. Make sure to set your credentials a profile file, as
explained in https://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/credentials.html

## Component: test_staticwebapp
The simple webapp is meant for testing purposes mainly. The idea is that this static web page can
show the images which have been uploaded to the AWS S3 service by the device. It does no analysis
or even ordering, but it allows you to double check the data. Please follow a few simple steps as
listed here: https://docs.aws.amazon.com/sdk-for-javascript/v2/developer-guide/s3-example-photos-view.html

## Component: webapp
Ongoing
