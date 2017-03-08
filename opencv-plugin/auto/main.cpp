#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>

#include <dirent.h>

#include "cmdline.h"
#include "processor.hpp"

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
    struct dirent* iterator;
    while ((iterator = readdir(inputDir)) != NULL) { //Retourne NULL à la fin du dossier
        if (iterator->d_type != DT_REG) continue; //On saute si c'est pas un fichier normal (dossier, lien, etc -voir man readdir)

        string inputFile(args.input_arg);
        inputFile.push_back('/');
        inputFile.append(iterator->d_name);

        
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
        } else if (args.action_arg == action_arg_test) {
            //Utiliser ça pour des tests rapides d'algo
            goto quit;
        }else { //Disparity
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
            string outputFile(args.output_arg);
            outputFile.push_back('/');        
            outputFile.append(args.prefix_arg);
            outputFile.append(iterator->d_name);
            if (!cv::imwrite(outputFile.c_str(), img))
                cerr << "Erreur ? Enregistrement image" << endl;
        }
    }


    quit: //Ahahahah j'ai enfin trouvé où utiliser un goto
    cmdline_parser_free(&args);
    return 0;
}
