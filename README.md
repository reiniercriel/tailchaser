# tailchase
A complete project for tracking your pets movements during the day and uploading, analysing and showing these movements on the AWS cloud. Please find in this repository the following layers:
* Device layer
* Simple WebbApp (TODO)
* WebApp (TODO)
You will find more detailed explanations about every layer below.

## Component: Device

### Description
The device layer of this project is responsible for interacting with the machine on which the application is installed. The code was made for and tested on Rasberry Pi 4 but can also run on similar devices. As long as it is a Unix machine with a camera connected, changes are tyou can get it working fairly easily.

### Pre-requisites for building the device application code (Linux only)
1. GNU Compiler Collection (GCC) 4.9 or later 
2. AWS-SDK: Build and install from source. See https://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/setup.html for detailed instructions. This package is needed to interact with the S3 bucket on AWS (For uploading images)
3. CMake: Please install the latest versoin of cmake
4. OpenCV 4: Build and install from source: Seee https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html for detailed instructions
5. Git

### Building the device component
1. Create a build folder
2. Run CMAKE from the build folder followed by the path to the source directory. e.g. 'CMAKE <path/to/source>'
3. Run MAKE from the build folder. e.g. 'MAKE .'

## Simple WebApp
Ongoing

## WebApp
Ongoing
