#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

#include <dirent.h>

#include "cmdline.h"
#include "genericprocessor.hpp"
#include "displayprocessor.hpp"
#include "disparityprocessor.hpp"
#include "distanceprocessor.hpp"

using namespace std;

typedef unordered_map<enum enum_action, unique_ptr<GenericProcessor>> processormap;

/*!
 * brief: Associe une action avec un processeur
*/
void registerProcessors(processormap& map) {
    map.reserve(10);
    map.emplace(action_arg_display, new DisplayProcessor()); //(key, value)
    map.emplace(action_arg_sgbm, new DisparityProcessor());
    map.emplace(action_arg_dist, new DistanceProcessor(80, 3.5, 6)); //En mm, SensorSize 1/3" == 6mm
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
    cout << "Reading " << args.input_arg << endl;
    DIR* inputDir = opendir(args.input_arg);
    if (inputDir == NULL) {
        perror("Erreur à l'ouverture de input");
        return errno;
    }

    struct dirent* iterator;
    while ((iterator = readdir(inputDir)) != NULL) { //Retourne NULL à la fin du dossier
        if (iterator->d_type != DT_REG) {
            cerr << iterator->d_name << " is not a file, jumping." << endl;
            continue; //On saute si c'est pas un fichier normal (dossier, lien, etc -voir man readdir)
        }    

        string inputFile(args.input_arg);
        inputFile.push_back('/');
        inputFile.append(iterator->d_name);

        string outputFile(args.output_arg);
        outputFile.push_back('/');        
        outputFile.append(args.prefix_arg);
        outputFile.append(iterator->d_name);
        

        //Ouverture d'image
        cv::Mat img = cv::imread(inputFile);
        if (img.data == NULL) {
            cerr << "Erreur lecture du fichier " << inputFile << ". Ignoré." << endl;
            continue;
        }

        cout << "Processing " << inputFile << endl;
        processors[args.action_arg]->process(img, inputFile);

        if (processors[args.action_arg]->doSave()) {
            cout << "Saving to " << outputFile << endl;
            if (!cv::imwrite(outputFile, img))
                cerr << "Erreur ? Enregistrement image." << endl;
        }
    }

    quit:
    cmdline_parser_free(&args);
    return 0;
}