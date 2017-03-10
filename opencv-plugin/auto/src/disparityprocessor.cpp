#include "disparityprocessor.hpp"

using namespace cv;

DisparityProcessor::DisparityProcessor(bool useSGBM) {
    mode = (useSGBM) ? StereoMode::STEREO_MODE_SGBM : StereoMode::STEREO_MODE_BM;
}

void DisparityProcessor::process(Mat& img, const std::string& fileName) {
    Mat left, right;
    split(img, left, right);
    img = disparityMap(left, right, mode);
}