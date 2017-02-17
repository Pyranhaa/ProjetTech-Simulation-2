#include "imageprocessor.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/features2d/features2d.hpp"

cv::Mat disparityMap(cv::Mat& img_gauche, cv::Mat& img_droite, int mode){
    cv::Mat disp16 = cv::Mat(img_gauche.rows, img_gauche.cols, CV_16S);
    cv::Mat disp8 = cv::Mat(img_gauche.rows, img_gauche.cols, CV_8UC1);
    cv::Mat img_g, img_d;

    cv::cvtColor(img_gauche, img_g, CV_BGR2GRAY);
    cv::cvtColor(img_droite, img_d, CV_BGR2GRAY);
    img_g.convertTo(img_g, CV_8UC1);
    img_d.convertTo(img_d, CV_8UC1);

    int ratio = 7;
    int numDisp = ratio * 16;
    int SADWindowSize = 21;
    
    if (mode == 0){
      cv::StereoBM sbm = cv::StereoBM(cv::StereoBM::BASIC_PRESET, numDisp, SADWindowSize);
      sbm(img_g, img_d, disp16);

    }else if (mode == 1){
      int minDisp = 0;
      cv::StereoSGBM sgbm = cv::StereoSGBM(minDisp, numDisp, SADWindowSize);
      sgbm(img_g, img_d, disp16);
    }
    
    double minVal; double maxVal;
    cv::minMaxLoc( disp16, &minVal, &maxVal );

    disp16.convertTo(disp16, CV_8UC1, 255/(maxVal - minVal));
    cv::cvtColor(disp16, disp16, CV_GRAY2BGR);
    cv::normalize(disp16, disp8, 0, 255, CV_MINMAX, CV_8U);
    
    
    return disp16;
}