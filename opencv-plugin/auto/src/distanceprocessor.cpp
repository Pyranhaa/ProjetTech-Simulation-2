#include "distanceprocessor.hpp"
#include "disparityprocessor.hpp"
#include <iostream>

#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

DistanceProcessor::DistanceProcessor(float bl, float f, float ss)
                :   baseline(bl),
                    focal(f),
                    sensorSize(ss) {
    
}

void DistanceProcessor::process(cv::Mat& img, const std::string& fileName) {
    proc.process(img, fileName);

    imshow("0", img);
    waitKey(0);
    Mat depth(img.rows, img.cols, CV_32FC1);

    for (int y = 0; y < depth.rows; y++) {
      for (int x = 0; x < depth.cols; x++) {
        depth.at<float>(y, x) = calculateDepth(img.at<unsigned char>(y, x), img.cols);
      }
    }

    imshow("", depth);
    waitKey(0);

    depth.copyTo(img);
}

float DistanceProcessor::calculateDepth(unsigned char disp, int imgsize) {
	//Where is the distance ?
    return ((baseline * focal) / ((disp/(float) imgsize) * sensorSize));
}
