#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>

#include <dirent.h>

#include "cmdline.h"
#include "processor.hpp"

#define CONFIG "cal.yml"

using namespace std;
    

int main (int argc, char** argv) {
    struct gengetopt_args_info args;
    //Parser
    if (cmdline_parser (argc, argv, &args) != 0)
         exit(1);

    if (args.help_given) {
        cmdline_parser_print_help();
        goto quit;
    }
    if (args.version_given) {
        cmdline_parser_print_version();
        goto quit;
    }

    //On parcours chaque image
    DIR* inputDir = opendir(args.input_arg);
    if (inputDir == NULL) {
        perror("Erreur à l'ouverture de input");
        return errno;
    }

    //Pour la calibration
    std::vector<std::vector<cv::Point2f>> imagePointsL, imagePointsR;
    cv::Size imgSize, boardSize(12, 12); //TODO BoardSize
    float squareSize = 3; // 1/4 de la planche ?


    struct dirent* iterator;
    while ((iterator = readdir(inputDir)) != NULL) { //Retourne NULL à la fin du dossier
        if (iterator->d_type != DT_REG) continue; //On saute si c'est pas un fichier normal (dossier, lien, etc -voir man readdir)

        string inputFile(args.input_arg);
        inputFile.push_back('/');
        inputFile.append(iterator->d_name);

        string outputFile(args.output_arg);
        outputFile.push_back('/');        
        outputFile.append(args.prefix_arg);
        outputFile.append(iterator->d_name);
        
        //Si print, on le traite avant pour ne pas charger d'image pour rien
        if (args.action_arg == action_arg_print) {
            cout << inputFile << "\t==>\t" << outputFile << endl;
            continue;
        }
        //Ouverture d'image
        cv::Mat img = cv::imread(inputFile.c_str());
        if (img.data == NULL) {
            cerr << "Erreur lecture du fichier " << inputFile << endl;
            return 1;
        }
        bool doSave = true;
        //On applique l'action voulue
        if (args.action_arg == action_arg_display) {
            display_img(img, string(iterator->d_name));
            cv::waitKey(0);
            doSave = false;
        } else if (args.action_arg == action_arg_calibrate) { //uniquement les POI, la calibration est après le parcourt des images
            //Etape 1: diviser pour mieux reigner
            cv::Mat left, right;
            split(img, left, right);
            //Etape 2: faire la collection des points d'interêts dans toutes les images (l'échiquier)
            //std::vector<std::vector<cv::Point2f>> imagePointsL, imagePointsR; //Tableau qui contient les tableaux de POI des images (définis au dessus)
            std::vector<cv::Point2f> POI1, POI2; //Les deux car si on ne trouve pas le pattern dans l'une des deux on saute
            
            bool f1 = getPOI(left, boardSize, POI1);
            bool f2 = getPOI(right, boardSize, POI2);
            if (!(f1 && f2)) {
                cerr << "Pas d'échiquier trouvé dans l'image, on saute" << endl;
                continue;
            }
            imagePointsL.push_back(POI1); //C'est copié
            imagePointsR.push_back(POI2); //C'est copié
            imgSize = left.size(); //Ouais c'est refait à tout les tours, il faudrait réécrire l'automate en entier pour corriger ça (en utilisant des classes et tout, faire un truc propre quoi)
            doSave = false;
        } else if (args.action_arg == action_arg_undistort) { //Appliquer le undistort
            //TODO Undistort
        } else if (args.action_arg == action_arg_test) {
            //Utiliser ça pour des tests rapides d'algo
            goto quit;
        } else { //Disparity
            cv::Mat left, right;
            split(img, left, right);
            if (args.action_arg == action_arg_bm) {
                img = disparityMap(left, right, StereoMode::STEREO_MODE_BM);
            } else {
                img = disparityMap(left, right, StereoMode::STEREO_MODE_SGBM);                    
            }
        }

        //Sauvegarde
        if (doSave) {
            if (!cv::imwrite(outputFile.c_str(), img))
                cerr << "Erreur ? Enregistrement image" << endl;
        }
    }

    if (args.action_arg == action_arg_calibrate) { //Ici on effectue la calibration et on sauvegarde dans cal.yaml
        if (imagePointsL.empty() || imagePointsR.empty())
            goto quit;
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


    quit: //Ahahahah j'ai enfin trouvé où utiliser un goto
    cmdline_parser_free(&args);
    return 0;
}
