#ifndef DISPLAYPROCESSOR_HPP
#define DISPLAYPROCESSOR_HPP

#include "genericprocessor.hpp"

#include "processor.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class DisplayProcessor : public GenericProcessor {
    public:
        DisplayProcessor() {};

        void process(cv::Mat& img, const std::string& fileName) {
            display_img(img, fileName);
            cv::waitKey(0);
        }
        
        void post() {};
};

#endif