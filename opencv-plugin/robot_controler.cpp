#include "robot_controler.h"
#include "processor.hpp"
#include "string"
void Robot_controler::process(const cv::Mat & left_img, const cv::Mat & right_img, float * vx, float * vy, float * omega){
    
    std::string compteur = std::to_string(compteur++);
    
    cv::Mat img;
    merge(left_img, right_img, img);
    
    std::string img_nom = "img_" + compteur + ".png";
    cv::imwrite(img_nom.c_str(), img);
    
    std::string disp_nom = "disp_" + compteur + ".png";
    
    cv::Mat disp = disparityMap(left_img, right_img, &prop);
    cv::imwrite(disp_nom.c_str(), disp);
    
    std::string dist_name "dist" + compteur + "png";
    
    cv::Mat dist = cv::Mat(disp.rows, disp.cols);    
    depthMap(&disp, &dist, 80, 3.5, 6);
    
    cv::imwrite(dist_nom.c_str(), dist);
}

Robot_controler::Robot_controler(){
    properties.minDisparity = -3;
    properties.numDisparity = 64;
    properties.SadWindowSize = 21;
    properties.P1 = 0;
    properties.P2 = 0;
    properties.dispMaxDiff = -1;
    properties.preFilterCap = 0;
    properties.uniquenessRatio = 0;
    properties.speckleWindowSize = 79;
}
