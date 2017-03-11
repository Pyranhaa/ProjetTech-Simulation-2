#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

#include <dirent.h>

#include "cmdline.h"
#include "genericprocessor.hpp"
#include "displayprocessor.hpp"
#include "calibrateprocessor.hpp"
#include "disparityprocessor.hpp"

using namespace std;

typedef unordered_map<enum enum_action, unique_ptr<GenericProcessor>> processormap;

/*!
 * brief: Associe une action avec un processeur
*/
void registerProcessors(processormap& map) {
    map.reserve(10);
    map.emplace(action_arg_display, new DisplayProcessor()); //(key, value)
    map.emplace(action_arg_sgbm, new DisparityProcessor(true));
    map.emplace(action_arg_bm, new DisparityProcessor(false));
    map.emplace(action_arg_calibrate, new CalibrateProcessor(cv::Size(3, 3), 1)); //TODO Changer les tailles / les configurer en paramètres (voir doc constructeur)
}

int main(int argc, char** argv) {
    struct gengetopt_args_info args;
    //Parser
    if (cmdline_parser (argc, argv, &args) != 0) {
        cerr << "Error parsing args" << endl;
        exit(1);
    }

    if (args.help_given) {
        cmdline_parser_print_help();
        goto quit;
    }
    if (args.version_given) {
        cmdline_parser_print_version();
        goto quit;
    }

    //Init processors
    processormap processors;
    registerProcessors(processors);

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

        string outputFile(args.output_arg);
        outputFile.push_back('/');        
        outputFile.append(args.prefix_arg);
        outputFile.append(iterator->d_name);
        

        //Ouverture d'image
        cv::Mat img = cv::imread(inputFile.c_str());
        if (img.data == NULL) {
            cerr << "Erreur lecture du fichier " << inputFile << ". Ignoré" << endl;
            continue;
        }

        processors[args.action_arg]->process(img, inputFile);

        if (processors[args.action_arg]->doSave()) {
            if (!cv::imwrite(outputFile.c_str(), img))
                cerr << "Erreur ? Enregistrement image" << endl;
        }
    }

    quit:
    cmdline_parser_free(&args);
    return 0;
}