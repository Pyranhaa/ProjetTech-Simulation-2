#ifndef CALIBRATEPROCESSOR_HPP
#define CALIBRATEPROCESSOR_HPP

#include "genericprocessor.hpp"

#include <opencv2/core/core.hpp>

#define CONFIG "cal.yml"

class CalibrateProcessor : public GenericProcessor {
    public:
        /*
         * param: bSize: number of inner corners (rows, cols)
         * param: sSize: size of a square in the pattern in custom unit (??)
         */
        CalibrateProcessor(cv::Size bSize, float sSize);

        void process(cv::Mat& img, const std::string& fileName);
        void post();

    private:
        std::vector<std::vector<cv::Point2f>> imagePointsL, imagePointsR;
        cv::Size imgSize, boardSize;
        float squareSize;

};

#endif