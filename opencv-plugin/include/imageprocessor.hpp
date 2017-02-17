#ifndef IMAGEPROCESSOR_HPP
#define IMAGEPROCESSOR_HPP

#include <opencv2/core/core.hpp>

enum {STEREO_BM=0, STEREO_SGBM=1};

//TODO Document
cv::Mat disparityMap(cv::Mat& img_droite, cv::Mat& img_gauche, int mode);

//TODO Autres fonctions genre depthmap, trouver quelqu'un etc

#endif