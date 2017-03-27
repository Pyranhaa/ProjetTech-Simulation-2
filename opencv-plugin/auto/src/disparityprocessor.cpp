#include "disparityprocessor.hpp"

#include <iostream>

using namespace cv;

DisparityProcessor::DisparityProcessor() {
    prop.minDisparity = 0;
    prop.numDisparity = 64;
    prop.SadWindowSize = 21;
    prop.P1 = 8 * prop.SadWindowSize * prop.SadWindowSize;
    prop.P2 = 32 * prop.SadWindowSize * prop.SadWindowSize;
    prop.disp12MaxDiff = -1;
    prop.preFilterCap = 9;
    prop.uniquenessRatio = 15;
    prop.speckleWindowSize = 0;
    prop.fullDP = false;
}

void DisparityProcessor::process(Mat& img, const std::string& fileName) {
    Mat left, right;
    split(img, left, right);
    img = disparityMap(left, right, prop);
}