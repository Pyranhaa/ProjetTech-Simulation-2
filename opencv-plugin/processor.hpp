#ifndef _PROCESSOR_HPP
#define _PROCESSOR_HPP

#include <opencv2/core/core.hpp>

extern "C" {
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
    cv::Mat disparityMap(const cv::Mat& leftImage, const cv::Mat& rightImage);

    /*
    Get points of interests in image containing chessboard
    Returns false if no chessboard found
    */
    bool getPOI(const cv::Mat& img, const cv::Size& boardSize, std::vector<cv::Point2f>& POI);

    /*
    Return false if calibration failed (no chessboard detected?)
    Expect chessboard in image, of size boardSize. Returns calibration matrices cameraMatrix*, distCoeffs*
    rotation matrix between the two cams and translation vector
    */
    bool runCalibration(const cv::Size& size, const cv::Size& boardSize, const float squareSize,
                      cv::Mat& cameraMatrixL, cv::Mat& distCoeffsL, std::vector<std::vector<cv::Point2f>>& imagePointsL,
                      cv::Mat& cameraMatrixR, cv::Mat& distCoeffsR, std::vector<std::vector<cv::Point2f>>& imagePointsR,
                      cv::Mat& R, cv::Mat& T);
}
#endif
