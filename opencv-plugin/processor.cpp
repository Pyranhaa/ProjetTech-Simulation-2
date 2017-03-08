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

#include <GL/gl.h>

static cv::Mat left;
static cv::Mat right;


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
    tex2Mat(tex_id, width, height, left);
  }

  EXPORT void load_right(int tex_id, int width, int height) {
    tex2Mat(tex_id, width, height, right);
  }

  EXPORT int display_disparity() {
    if (left.empty())
      return 1;
    if (right.empty())
      return 2;
    cv::Mat img = disparityMap(left, right, StereoMode::STEREO_MODE_SGBM);
    if (img.empty())
      return 3;
    display_img(img, "Ma carte de disp !");
    return 0;
  }

  EXPORT int display_cams() {
    if (left.empty())
      return 1;
    if (right.empty())
      return 2;
    cv::Mat img;
    merge(left, right, img);
    if (img.empty())
      return 3;
    display_img(img, "Display");
    return 0;
  }

  //Sauvegarde l'image des cams (visiblement fait crash l'editeur, probablement à cause du context, opencv ne doit pas réussir à save)
  EXPORT void dump_left(char* name) {
    cv::imwrite(name, left);
  }
  EXPORT void dump_right(char* name) {
    cv::imwrite(name, right);
  }

  EXPORT void test_undistort() {
    if (left.empty() || right.empty()) return;

    cv::Mat cML, cMR, dCL, dCR, R, T;

    if (!runCalibration(left, right, cv::Size(1,1), 0.25,
                      cML, dCL,
                      cMR, dCR,
                      R, T)) return;
    cv::Mat cL, cR;
    cv::undistort(left, cL, cML, dCL);
    cv::undistort(right, cR, cMR, dCR);

    display_img(cL, "Gauche corrigée");
    display_img(cR, "Droite corrigée");    
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
        //cout << x + left.cols+1 << endl;
      }
    }

    tmp.copyTo(output);
  }

  cv::Mat disparityMap(const cv::Mat& leftImage, const cv::Mat& rightImage, StereoMode mode) {
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

  bool runCalibration(const cv::Mat& left, const cv::Mat& right, const cv::Size& boardSize, const float squareSize,
                      cv::Mat& cameraMatrixL, cv::Mat& distCoeffsL,
                      cv::Mat& cameraMatrixR, cv::Mat& distCoeffsR,
                      cv::Mat& R, cv::Mat& T) {
    std::vector<cv::Point2f> pointBufL, pointBufR;
    //On cherche le pattern dans les deux images et on garde dans pointBuf*
    bool found = cv::findChessboardCorners(left, boardSize, pointBufL, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);
    if (!found) return false;
    found = cv::findChessboardCorners(right, boardSize, pointBufR, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);
    if (!found) return false;

    //Améliore la précision des positions des points d'interêt
    cv::Mat viewGray;
    cv::cvtColor(left, viewGray, CV_BGR2GRAY);
    cv::cornerSubPix( viewGray, pointBufL, cv::Size(11,11), cv::Size(-1,-1), cv::TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
    cv::cvtColor(right, viewGray, CV_BGR2GRAY);
    cv::cornerSubPix( viewGray, pointBufR, cv::Size(11,11), cv::Size(-1,-1), cv::TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

    std::vector<std::vector<cv::Point2f> > imagePointsL, imagePointsR;
    imagePointsL.push_back(pointBufL);
    imagePointsR.push_back(pointBufR);    

    std::vector<std::vector<cv::Point3f>> objectPoints(1);
    //Fokin wot? Je sais pas trop pourquoi
    for( int i = 0; i < boardSize.height; ++i )
      for( int j = 0; j < boardSize.width; ++j )
        objectPoints[0].push_back(cv::Point3f(float( j*squareSize ), float( i*squareSize ), 0));

    objectPoints.resize(imagePointsL.size(),objectPoints[0]);
    
    cv::Mat E, F; //Essential and fundamental matrices (wot)
    cv::stereoCalibrate(objectPoints, imagePointsL, imagePointsR, cameraMatrixL, distCoeffsL, cameraMatrixR, distCoeffsR, left.size(), R, T, E, F);
  }

}