/**
 * Interface entre la simulation Unity et le robot_controler
 * Pourquoi ? Parce que le controler utilise la lib, je peux pas avoir la lib qui utilise le controler alors que le controler utilise la lib
**/

#include <GL/gl.h>

#include "robot_controler.h"
#include "processor.hpp"

/**
 * Contient les infos à récupèrer par Unity après un appel à process
**/
typedef struct {
    float vx;
    float vy;
    float omega;
} robotMvmt;

static robotMvmt mvmt;
static Robot_controler robot(80, 1, 1);

/**
 *  Converti texture opengl en Mat
**/
void tex2Mat(int tex_id, int width, int height, cv::Mat& target);

extern "C" {
    float getVx() { return mvmt.vx; }
    float getVy() { return mvmt.vy; }
    float getOmega() { return mvmt.omega; }

    /**
     * Interface avec le robot, prend les IDs des textures des caméras en entrée ainsi que les infos du robots et stock les valeurs finales dans mvmt
    **/
    void process(int tex_left, int tex_right, int width, int height, float vx, float vy, float omega) {
        cv::Mat left, right;
        tex2Mat(tex_left, width, height, left);
        tex2Mat(tex_right, width, height, right);

        mvmt.vx = vx;
        mvmt.vy = vy;
        mvmt.omega = omega;
        robot.process(left, right, &mvmt.vx, &mvmt.vy, &mvmt.omega);
    }

    void print_mats(int tex_left, int tex_right, int w, int h) {
        cv::Mat left, right;
        tex2Mat(tex_left, w, h, left);
        tex2Mat(tex_right, w, h, right);

        robot.print_disparity(left, right);
        robot.print_depth(left, right);
    }
}

void tex2Mat(int tex_id, int width, int height, cv::Mat& target){
  glBindTexture(GL_TEXTURE_2D, tex_id);
  int nb_c = 3; // RGB

  unsigned char* data = (unsigned char*) malloc(sizeof(unsigned char) * width * height * nb_c);

  glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

  cv::Mat img(width, height, CV_8UC3, data);

  cv::flip(img, target, 0);
}