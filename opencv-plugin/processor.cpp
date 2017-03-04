#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/stitching/stitcher.hpp>


#include <string>
#ifndef UNITY
#include <iostream>
#endif

#include "processor.hpp"

#ifdef UNITY

#define EXPORT __attribute__((visibility("default")))

static cv::Mat gauche;
static cv::Mat droite;

#include <GL/gl.h>
void tex2Mat(int tex_id, int width, int height, cv::Mat& target){
  glBindTexture(GL_TEXTURE_2D, tex_id);
  int nb_c = 3; // RGB

  unsigned char* data = (unsigned char*) malloc(sizeof(unsigned char) * width * height * nb_c);

  glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

  cv::Mat img(width, height, CV_8UC3, data);

  cv::flip(img, target, 0);
}


extern "C"{
  EXPORT void display_texture(int tex_id, int width, int height){
    cv::Mat tex;
    tex2Mat(tex_id, width, height, tex);
    cv::imshow("display", tex);
    //cv::waitKey(0);
  }

  EXPORT void load_left(int tex_id, int width, int height) {
    tex2Mat(tex_id, width, height, gauche);
  }

  EXPORT void load_right(int tex_id, int width, int height) {
    tex2Mat(tex_id, width, height, droite);
  }

  EXPORT int display_disparity() {
    if (gauche.empty())
      return 1;
    if (droite.empty())
      return 2;
    cv::Mat img = disparityMap(gauche, droite, StereoMode::STEREO_MODE_SGBM);
    if (img.empty())
      return 3;
    display_img(img, "Ma carte de disp !");
    return 0;
  }

  EXPORT int display_cams() {
    if (gauche.empty())
      return 1;
    if (droite.empty())
      return 2;
    cv::Mat img;
    merge(gauche, droite, img);
    if (img.empty())
      return 3;
    display_img(img, "Display");
    return 0;
  }
}
#endif /* UNITY */

extern "C"{
  void display_img(cv::Mat img, const std::string& title) { //tmp
    //cv::namedWindow(title, cv::WINDOW_AUTOSIZE ); //Implicite dans imshow
    cv::imshow(title, img);
  }

  void split(const cv::Mat& input, cv::Mat& outputLeft, cv::Mat& outputRight) {
    cv::Mat tmp = input(cv::Rect(0, 0, input.cols / 2, input.rows));
    tmp.copyTo(outputLeft);

    tmp = input(cv::Rect(input.cols / 2, 0, input.cols / 2, input.rows));
    tmp.copyTo(outputRight);
  }

  void merge(const cv::Mat& left, const cv::Mat& right, cv::Mat& output) {
    cv::Mat tmp(left.rows, left.cols + right.cols, CV_8UC3);
    //Oui, les coordonnées sont dans le bon sens, c'est triste
    for (int y = 0; y < tmp.rows; y++) {
      for (int x = 0; x < left.cols; x++) {
        tmp.at<cv::Vec3b>(y, x) = left.at<cv::Vec3b>(y, x);
      }
      for (int x = 0; x < right.cols; x++) {
        tmp.at<cv::Vec3b>(y, x + left.cols) = right.at<cv::Vec3b>(y, x);
        //std::cout << x + left.cols+1 << std::endl;
      }
    }

    tmp.copyTo(output);
  }

  cv::Mat disparityMap(cv::Mat& leftImage, cv::Mat& rightImage, StereoMode mode) {
    cv::Mat disp16 = cv::Mat(leftImage.rows, leftImage.cols, CV_16S);
    cv::Mat disp8 = cv::Mat(leftImage.rows, leftImage.cols, CV_8UC1);
    cv::Mat lImg, dImg;

    cv::cvtColor(leftImage, lImg, CV_BGR2GRAY);
    cv::cvtColor(rightImage, dImg, CV_BGR2GRAY);
    lImg.convertTo(lImg, CV_8UC1);
    dImg.convertTo(dImg, CV_8UC1);

    int ratio = 7;
    int numDisp = ratio * 16;
    int SADWindowSize = 21;
    
    switch(mode)
    {
        case StereoMode::STEREO_MODE_BM:
        {
            cv::StereoBM sbm = cv::StereoBM(cv::StereoBM::BASIC_PRESET, numDisp, SADWindowSize);
            sbm(lImg, dImg, disp16);
            break;
        }

        case StereoMode::STEREO_MODE_SGBM:
        {
            int minDisp = 0;
            cv::StereoSGBM sgbm = cv::StereoSGBM(minDisp, numDisp, SADWindowSize);
            sgbm(lImg, dImg, disp16);
            break;
        }
    }
    
    double minVal; double maxVal;
    cv::minMaxLoc(disp16, &minVal, &maxVal);

    disp16.convertTo(disp16, CV_8UC1, 255/(maxVal - minVal));
    cv::cvtColor(disp16, disp16, CV_GRAY2BGR);
    cv::normalize(disp16, disp8, 0, 255, CV_MINMAX, CV_8U);
    
    return disp16;
  }
}