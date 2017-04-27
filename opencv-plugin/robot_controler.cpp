#include "robot_controler.h"
#include "processor.hpp"

#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

Robot_controler::Robot_controler(){
    Robot_controler(80, 3.5, 6, 1);
}

Robot_controler::Robot_controler(float bl, float f, float ss, float d)
    : baseline(bl), focal(f), sensorSize(ss), distance(d) {
    initProp();
}

void Robot_controler::initProp() {
    prop.minDisparity = 1;
    prop.numDisparity = 64;
    prop.SadWindowSize = 21;
    prop.P1 = 0;
    prop.P2 = 0;
    prop.dispMaxDiff = -1;
    prop.preFilterCap = 0;
    prop.uniquenessRatio = 0;
    prop.speckleWindowSize = 79;
    prop.fullDP = false;
}

void Robot_controler::process(const cv::Mat& left_img, const cv::Mat& right_img, float* vx, float* vy, float* omega){
    cv::Mat disp;
    disparityMap(left_img, right_img, disp, this->prop);
    //cv::imshow("Disp", disp);
    cv::Mat depth;
    depthMap(disp, depth, baseline, focal, sensorSize);

    //On découpe la carte en trois verticalement
    int t = depth.cols/3;
    cv::Range third(0, t);
    cv::Mat left = depth(cv::Range::all(), third);
    third.start += t; third.end += t;
    cv::Mat center = depth(cv::Range::all(), third);
    third.start += t; third.end += t;    
    cv::Mat right = depth(cv::Range::all(), third);

    /*
    cv::imshow("left", left);
    cv::imshow("center", center);
    cv::imshow("right", right);
    //*/

    double mleft = cv::mean(left).val[0];
    double mcenter = cv::mean(center).val[0];
    double mright = cv::mean(right).val[0];

    //*
    if (mleft < mcenter && mleft < mright && mleft < distance) {
        //On tourne vers la gauche
        *omega = 0.5f;
        *vx = 0.f;
    } else if (mright < mcenter && mright < mleft && mright < distance) {
        //On tourne vers la droite
        *omega = -0.5f;
        *vx = 0.f;
    } else if (mcenter < mleft && mcenter < mright && mcenter < distance) {
        //EN AVANT !
        *omega = 0.f;
        *vx = 1.f;
    }
    //*/
}