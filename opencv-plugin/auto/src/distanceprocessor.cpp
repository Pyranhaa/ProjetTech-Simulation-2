#include "distanceprocessor.hpp"
#include "disparityprocessor.hpp"
#include <iostream>

using namespace std;
using namespace cv;

DistanceProcessor::DistanceProcessor(float bl, float f, float ss)
                :   baseline(bl),
                    focal(f),
                    sensorSize(ss) {
    
}

void DistanceProcessor::process(cv::Mat& img, const std::string& fileName) {
    proc.process(img, fileName);

    Mat depth(img.rows, img.cols, CV_8UC1);

    for (int y = 0; y < depth.rows; y++) {
      for (int x = 0; x < depth.cols; x++) {
        depth.at<unsigned char>(y, x) = calculateDepth(img.at<unsigned char>(y, x));
      }
    }
    depth.copyTo(img);
}

unsigned char DistanceProcessor::calculateDepth(unsigned char disp) {
	//Where is the distance ?
    return ((baseline * focal) / (disp * sensorSize))*100;
}
