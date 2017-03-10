#ifndef GENERICPROCESSOR_HPP
#define GENERICPROCESSOR_HPP

#include <opencv2/core/core.hpp>


class GenericProcessor {
    public:
        
        /*
         * brief: Called on each read images
        */
        virtual void process(cv::Mat& img, const std::string& fileName) = 0;

        /*
         * brief: Called after all images have been processed. 
         */
        virtual void post() = 0;

        /*
         * brief: Should the images be saved ?
         */
        bool doSave() {return false;}
};

#endif