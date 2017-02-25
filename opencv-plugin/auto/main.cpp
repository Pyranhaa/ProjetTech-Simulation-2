#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>

#include <dirent.h>

#include "cmdline.h"

using namespace std;

//TODO TMP
extern "C" {
    void display_img(cv::Mat img, const string& title);
}
    

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

        string inputFile("/");
        inputFile.append(args.prefix_arg);
        inputFile.append(iterator->d_name);

        string outputFile(inputFile);
        outputFile.insert(0, args.output_arg);
        inputFile.insert(0, args.input_arg);
        
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
        switch (args.action_arg) {
            case action_arg_display:
                display_img(img, string(iterator->d_name));
                doSave = false;
                break;
        }
        //Sauvegarde
        if (doSave) {
            if (!cv::imwrite(outputFile.c_str(), img))
                cerr << "Erreur ? Enregistrement image" << endl;
        }

    }


    quit: //Ahahahah j'ai enfin trouvé où utiliser un goto
    cmdline_parser_free(&args);
    return 0;
}
