#ifndef DISPARITYPROCESSOR_HPP
#define DISPARITYPROCESSOR_HPP

#include "genericprocessor.hpp"

#include "processor.hpp"

#include <opencv2/core/core.hpp>

class DisparityProcessor : public GenericProcessor {
    public:
        /*
         * param: useSGBM : use SGBM or BM algorithm (defaults to BM)
         */
        DisparityProcessor(bool useSGBM);
        
        void process(cv::Mat& img, const std::string& fileName);
        void post() {};
        bool doSave() { return true; }

    private:
        enum StereoMode mode;
};

#endif