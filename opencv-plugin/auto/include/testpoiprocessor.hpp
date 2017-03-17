#ifndef TESTPOITPROCESSOR_HPP
#define TESTPOITPROCESSOR_HPP

#include <opencv2/core/core.hpp>


class TestPOIProcessor : public GenericProcessor{
    public:

        TestPOIProcessor(cv::Size bs) :
            boardSize(bs) {

        }
        
        void process(cv::Mat& img, const std::string& fileName) {
            std::vector<cv::Point2f> POI;
            bool f = getPOI(img, boardSize, POI);
            std::string s = (f) ? "" : "non ";
            std::cout << "Echiquier " << s << "trouvé dans " << fileName << std::endl;
        }

        void post() {

        }

    private:
        cv::Size boardSize;
};

#endif