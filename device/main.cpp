#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <unistd.h>
#include <string>
#include "application/tailchaser.h"

using namespace std;

int main(int argc, char **argv) {
    TailChaser *chaser = new TailChaser();
    return chaser->start();
}
