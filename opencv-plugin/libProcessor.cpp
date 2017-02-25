#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>

#ifdef UNITY

#define EXPORT __attribute__((visibility("default")))

#include <GL/gl.h>
cv::Mat tex2Mat(int tex_id, int width, int height){
  glBindTexture(GL_TEXTURE_2D, tex_id);
  int nb_c = 3; // RGB

  unsigned char* data = (unsigned char*) malloc(sizeof(unsigned char) * width * height * nb_c);

  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

  return cv::Mat(width, height, CV_8UC4, data);
  
}


extern "C"{
  EXPORT void display_texture(int tex_id, int width, int height){
    cv::Mat tex = tex2Mat(tex_id, width, height);
    cv::imshow("display", tex);
    cv::waitKey(0);
  }

}
#endif /* UNITY */

extern "C"{
  void display_img(cv::Mat img, const std::string& title) { //tmp
    //cv::namedWindow(title, cv::WINDOW_AUTOSIZE ); //Implicite dans imshow
    cv::imshow(title, img);
    cv::waitKey(0); //sa race le waitkey
    cv::destroyWindow(title);
  }
}