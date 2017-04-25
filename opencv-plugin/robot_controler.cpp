#include "robot_controler.h"
#include "processor.hpp"

#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

Robot_controler::Robot_controler(){
    Robot_controler(80, 3.5, 6);
}

Robot_controler::Robot_controler(float bl, float f, float ss)
    : baseline(bl), focal(f), sensorSize(ss) {
    initProp();
}

void Robot_controler::initProp() {
    prop.minDisparity = -3;
    prop.numDisparity = 64;
    prop.SadWindowSize = 21;
    prop.P1 = 0;
    prop.P2 = 0;
    prop.dispMaxDiff = -1;
    prop.preFilterCap = 0;
    prop.uniquenessRatio = 0;
    prop.speckleWindowSize = 79;
}

void Robot_controler::process(const Mat & left_img, const Mat & right_img, float * vx, float * vy, float * omega){
    Mat disp = disparityMap(left_img, right_img, prop);
    /*
    Mat depth;
    depthMap(disp, depth, baseline, focal, sensorSize);
    //*/
    //moyenne distance 
}