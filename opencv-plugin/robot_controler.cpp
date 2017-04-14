#include "robot_controler.h"
#include "processor.hpp"

#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


void Robot_controler::process(const cv::Mat & left_img, const cv::Mat & right_img, float * vx, float * vy, float * omega){
    
    std::string cpt = std::to_string(compteur);
    compteur++;
    
    cv::Mat img;
    merge(left_img, right_img, img);
    
    std::string img_nom = "img_" + cpt + ".png";
    cv::imwrite(img_nom, img);
    
    std::string disp_nom = "disp_" + cpt + ".png";
    
    cv::Mat disp;
    disp = disparityMap(left_img, right_img, prop);
    cv::imwrite(disp_nom, disp);
    
    std::string dist_nom = "dist" + cpt + "png";
    
    cv::Mat dist;
    depthMap(disp, dist, 80, 3.5, 6);
    
    cv::imwrite(dist_nom, dist);
}

Robot_controler::Robot_controler(){
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
