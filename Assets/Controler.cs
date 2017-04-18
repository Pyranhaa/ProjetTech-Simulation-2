using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

public class Controler : MonoBehaviour {
    [DllImport ("opencv-plugin/unity_controler.so")]
    protected static extern float getVx();
    [DllImport ("opencv-plugin/unity_controler.so")]
    protected static extern float getVy();
    [DllImport ("opencv-plugin/unity_controler.so")]
    protected static extern float getOmega();
    [DllImport ("opencv-plugin/unity_controler.so")]
    protected static extern void process(int tex_left, int tex_right, int width, int height, float vx, float vy, float omega);


    void Start () {

    }

    void Update() {

    }

}