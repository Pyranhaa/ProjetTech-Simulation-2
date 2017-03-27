#include "disparityprocessor.hpp"

#include <iostream>

using namespace cv;

DisparityProcessor::DisparityProcessor() {
    /* Paramètres testés en simulation et jugés bon:
		Pre filter clip: 0
		Sad window size: 21
		Minimum disparity: -3
		Number of disparity: 64
		Texture threshomd: 0
		Uniqueness ratio: 0
		Speckle size: 79
		Speckle range: 2
		Disparity Max difference: -1
	*/

    //* Defaut Jeremi
    prop.minDisparity = 0;
    prop.numDisparity = 64;
    prop.SadWindowSize = 21;
    prop.P1 = 8 * prop.SadWindowSize * prop.SadWindowSize;
    prop.P2 = 32 * prop.SadWindowSize * prop.SadWindowSize;
    prop.disp12MaxDiff = -1;
    prop.preFilterCap = 31;
    prop.uniquenessRatio = 0;
    prop.speckleWindowSize = 0;
    prop.fullDP = false;
    //*/

    /* Defaut Hadrien
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
    //*/
}

void DisparityProcessor::process(Mat& img, const std::string& fileName) {
    Mat left, right;
    split(img, left, right);
    img = disparityMap(left, right, prop);
}