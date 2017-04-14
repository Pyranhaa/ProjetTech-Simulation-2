#include <vector>

#include "stereo_flycap/controler.h"
#include "processor.hpp"

class Robot_controler : public Controler{
public:
  void process(const cv::Mat & left_img,
                       const cv::Mat & right_img,
                       float * vx, float * vy, float * omega);
  
  
private:
  std::vector<cv::Mat> l_image_droite;     
  std::vector<cv::Mat> l_image_gauche;
  std::vector<cv::Mat> l_depth_map;
  
}
