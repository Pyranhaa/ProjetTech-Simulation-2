#pragma once

#include <opencv2/core/core.hpp>

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

typedef struct StereoProperties{
	int minDisparity;
	int numDisparity;
	int SadWindowSize;
	int P1;
	int P2;
	int dispMaxDiff;
	int preFilterCap;
	int uniquenessRatio;
	int speckleWindowSize;
	bool fullDP;
}StereoProperties;

/*
Displays img in a window named title, waits for keystroke then closes the window
*/
void display_img(cv::Mat img, const std::string& title);

/*
Splits input in half verticaly and puts the result in output*
*/
void split(const cv::Mat& input, cv::Mat& outputLeft, cv::Mat& outputRight);

/*
Pastes left and right together to form output
*/
void merge(const cv::Mat& left, const cv::Mat& right, cv::Mat& output);

/*
Returns a disparity map from two images
*/
void disparityMap(const cv::Mat& leftImage, const cv::Mat& rightImage, cv::Mat& output, StereoProperties properties);


/*
Creates depthmap from disparity map and cameras parameters
*/
void depthMap(const cv::Mat& disparityMap,  cv::Mat& out, float baseline, float focal, float sensorSize);
