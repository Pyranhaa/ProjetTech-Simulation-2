#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/legacy/legacy.hpp>


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
    
    StereoProperties properties;
    properties.minDisparity = -3;
    properties.numDisparity = 64;
    properties.SadWindowSize = 21;
    properties.P1 = 0;
    properties.P2 = 0;
    properties.dispMaxDiff = -1;
    properties.preFilterCap = 0;
    properties.uniquenessRatio = 0;
    properties.speckleWindowSize = 79;
    
    cv::Mat img = disparityMap(left, right, properties);
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

  cv::Mat disparityMap(const cv::Mat& leftImage, const cv::Mat& rightImage, StereoProperties properties) {
    cv::Mat disp16 = cv::Mat(leftImage.rows, leftImage.cols, CV_16S);
    //cv::Mat disp8 = cv::Mat(leftImage.rows, leftImage.cols, CV_8UC1); //Voir normalize en bas de la fonction
    cv::Mat lImg, dImg;

    cv::cvtColor(leftImage, lImg, CV_BGR2GRAY);
    cv::cvtColor(rightImage, dImg, CV_BGR2GRAY);
    lImg.convertTo(lImg, CV_8UC1);
    dImg.convertTo(dImg, CV_8UC1);


		cv::StereoSGBM sgbm;

		sgbm.minDisparity = properties.minDisparity;
		sgbm.numberOfDisparities = properties.numDisparity;
		sgbm.SADWindowSize = properties.SadWindowSize;
		sgbm.P1 = properties.P1;
		sgbm.P2 = properties.P2;
		sgbm.disp12MaxDiff = properties.dispMaxDiff;
		sgbm.preFilterCap = properties.preFilterCap;
		sgbm.uniquenessRatio = properties.uniquenessRatio;
		sgbm.speckleWindowSize = properties.speckleWindowSize;
		sgbm.fullDP = properties.fullDP;

    sgbm(lImg, dImg, disp16);
    
    double minVal; double maxVal;
    cv::minMaxLoc(disp16, &minVal, &maxVal);

    disp16.convertTo(disp16, CV_8U, 255.0 / (sgbm.numberOfDisparities * 16.0));
    //disp16.convertTo(disp16, CV_8UC1, 255/(maxVal - minVal));
    //cv::cvtColor(disp16, disp16, CV_GRAY2BGR);
    //cv::normalize(disp16, disp8, 0, 255, CV_MINMAX, CV_8U); //Ne sert à rien si on ne retourne pas disp8
    
    return disp16;
  }

  /*
	cv::Mat create3Dimage(cv::Mat& dispMat, cv::Mat& Q, bool handleMissingValues=false, int ddepth){
		cv::Mat 3D_img = cv::Mat(dispMat.rows, dispMat.cols, CV_32FC3);
		cv::reprojectImageTo3D(dispMat, 3D_img, Q, handleMissingValues, ddepth);
		return 3D_img;
	}
  */

  bool getPOI(const cv::Mat& img, const cv::Size& boardSize, std::vector<cv::Point2f>& POI) {
    cv::Mat viewGray;
    cv::cvtColor(img, viewGray, CV_BGR2GRAY);

    bool found = cv::findChessboardCorners(viewGray, boardSize, POI, CV_CALIB_CB_FAST_CHECK);//, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS | //_CALIB_CB_FAST_CHECK);
    if (!found) return false;

    cv::cornerSubPix( viewGray, POI, cv::Size(11,11), cv::Size(-1,-1), cv::TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

    return true;
  }

  bool runCalibration(const cv::Size& size, const cv::Size& boardSize, const float squareSize,
                      cv::Mat& cameraMatrixL, cv::Mat& distCoeffsL, std::vector<std::vector<cv::Point2f>>& imagePointsL,
                      cv::Mat& cameraMatrixR, cv::Mat& distCoeffsR, std::vector<std::vector<cv::Point2f>>& imagePointsR,
                      cv::Mat& R, cv::Mat& T) {
    std::vector<cv::Point2f> pointBufL, pointBufR;

    std::vector<std::vector<cv::Point3f>> objectPoints(1);
    //Fokin wot? Je sais pas trop pourquoi
    for( int i = 0; i < boardSize.height; ++i )
      for( int j = 0; j < boardSize.width; ++j )
        objectPoints[0].push_back(cv::Point3f(float( j*squareSize ), float( i*squareSize ), 0));

    objectPoints.resize(imagePointsL.size(),objectPoints[0]);
    
    cv::Mat E, F; //Essential and fundamental matrices (wot)
    cv::stereoCalibrate(objectPoints, imagePointsL, imagePointsR, cameraMatrixL, distCoeffsL, cameraMatrixR, distCoeffsR, size, R, T, E, F);
    return true;
  }

}
