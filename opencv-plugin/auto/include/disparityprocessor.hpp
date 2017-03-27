#ifndef DISPARITYPROCESSOR_HPP
#define DISPARITYPROCESSOR_HPP

#include "genericprocessor.hpp"

#include "processor.hpp"

#include <opencv2/core/core.hpp>

class DisparityProcessor : public GenericProcessor {
    public:

        DisparityProcessor();
        
        void process(cv::Mat& img, const std::string& fileName);
        void post() {};
        bool doSave() { return true; }

    private:
        StereoProperties prop;
};

#endif