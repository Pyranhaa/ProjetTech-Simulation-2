#include <vector>

#include "processor.hpp"
#include "controler.h"


class Robot_controler : public Controler{
public:

  Robot_controler();
  void process(const cv::Mat & left_img,
                       const cv::Mat & right_img,
                       float * vx, float * vy, float * omega);
  
  
private:
  std::vector<cv::Mat> l_image_droite;     
  std::vector<cv::Mat> l_image_gauche;
  std::vector<cv::Mat> l_depth_map;
  
  StereoProperties prop;
  unsigned int compteur = 0;
};
