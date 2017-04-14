#pragma once

#include "opencv2/core/core.hpp"

class Controler
{
public:
  /// Use the provided left and right images to update the desired speed
  /// - vx: speed forward [m/s]
  /// - vy: lateral speed [m/s] (+ is left)
  /// - omega: angular speed [rad/s] (+ is rotating to left)
  virtual void process(const cv::Mat & left_img,
                       const cv::Mat & right_img,
                       float * vx, float * vy, float * omega) = 0;
};
