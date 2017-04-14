#include "distanceprocessor.hpp"
#include "disparityprocessor.hpp"
#include "processor.hpp"

#include <iostream>

#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

Mat depth2;

DistanceProcessor::DistanceProcessor(float bl, float f, float ss)
                :   baseline(bl),
                    focal(f),
                    sensorSize(ss) {}

void DistanceProcessor::process(cv::Mat& img, const std::string& fileName) {
    proc.process(img, fileName);
    depthMap(img, img, baseline, focal, sensorSize);
}

