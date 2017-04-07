#ifndef DISTANCEPROCESSOR_HPP
#define DISTANCEPROCESSOR_HPP

#include "genericprocessor.hpp"
#include "disparityprocessor.hpp"

#include "processor.hpp"

#include <opencv2/core/core.hpp>

class DistanceProcessor : public GenericProcessor {
    public:

        DistanceProcessor(float bl, float f, float ss);
        
        void process(cv::Mat& img, const std::string& fileName);
        void post() {};
        bool doSave() { return true; }

    private:
        float calculateDepth(unsigned char disp, int imgsize);
        DisparityProcessor proc;

        float baseline;
        float focal;
        float sensorSize;
};

#endif