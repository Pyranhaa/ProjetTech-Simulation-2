#include "calibrateprocessor.hpp"

#include "processor.hpp"

#include <iostream>

using namespace cv;
using namespace std;

CalibrateProcessor::CalibrateProcessor(Size bSize, float sSize) : 
    boardSize(bSize), squareSize(sSize) {

}

void CalibrateProcessor::process(Mat& img, const string& fileName) {
    //Etape 1: diviser pour mieux reigner
    Mat left, right;
    split(img, left, right);
    //Etape 2: faire la collection des points d'interêts dans toutes les images (l'échiquier)
    vector<cv::Point2f> POI1, POI2; //Les deux car si on ne trouve pas le pattern dans l'une des deux on saute
    
    bool f1 = getPOI(left, boardSize, POI1);
    bool f2 = getPOI(right, boardSize, POI2);
    if (!(f1 || f2)){
        cerr << "Echiquier non trouvé dans " << fileName << ". Ignoré" << endl;
        return;
    }
    imagePointsL.push_back(POI1); //C'est copié
    imagePointsR.push_back(POI2);
}

void CalibrateProcessor::post() {
    if (imagePointsL.empty() || imagePointsR.empty())
        return;
    //Etape 3 on calibre
    cv::Mat cameraMatrixL, distCoeffsL, cameraMatrixR, distCoeffsR, R, T;
    runCalibration(imgSize, boardSize, squareSize,
                    cameraMatrixL, distCoeffsL, imagePointsL,
                    cameraMatrixR, distCoeffsR, imagePointsR,
                    R, T);
    //Etape 4 on enregistre
    cv::FileStorage file(CONFIG, cv::FileStorage::WRITE);

    file << "cameraMatrixL" << cameraMatrixL;
    file << "distCoeffsL" << distCoeffsL;
    file << "cameraMatrixR" << cameraMatrixR;
    file << "distCoeffsR" << distCoeffsR;
    file << "R" << R;
    file << "T" << T;

    file.release();
}