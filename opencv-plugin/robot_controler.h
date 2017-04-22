#include <vector>

#include "processor.hpp"
#include "controler.h"


class Robot_controler : public Controler{
public:

  Robot_controler();
  Robot_controler(float bl, float f, float ss);  
  void process(const cv::Mat & left_img,
                       const cv::Mat & right_img,
                       float * vx, float * vy, float * omega);

  void print_disparity(const cv::Mat & left_img, const cv::Mat & right_img);
  void print_depth(const cv::Mat & left_img, const cv::Mat & right_img);
  
  
private:
  void initProp();
  //Caméras
  float baseline;
  float focal;
  float sensorSize;

  /*
  std::vector<cv::Mat> l_image_droite;     
  std::vector<cv::Mat> l_image_gauche;
  std::vector<cv::Mat> l_depth_map;
  */
  
  StereoProperties prop;
  unsigned int compteur = 0;
};
