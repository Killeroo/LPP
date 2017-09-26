#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <map>

//https://stackoverflow.com/questions/4702732/the-program-cant-start-because-libgcc-s-dw2-1-dll-is-missing

int main(int argc , char *argv[])
{
    // Arguments check
    if (argc < 3 || argc > 3)
    {
        printf("ERROR: Not enough arguments.\n");
        printf("long_path_parser.exe [INPUT_FILE] [OUTPUT_FILE]\n");
        return 1;
    }

    // Local variable setup
    std::ifstream ifs(argv[1]); // Input stream
    std::ofstream ofs(argv[2]); // Output stream
    std::list<std::string>::const_iterator iterator; // Iterator for looping through results
    std::list<std::string> paths;
    std::list<int> path_lengths;
    std::list<int> path_required_cuts;
    std::string line;
    std::map<std::string, int, int> path_map;

    // Check file streams open
    if (!ifs.is_open())
    {
        printf("ERROR: %s could not be opened. Check this file exists.", argv[1]);
        return 1;
    }
    else if (!ofs.is_open())
    {
        printf("ERROR: %s could not be created. Check your privilege.", argv[2]);
        return 1;
    }

    // Loop through each line of input file
    while (std::getline(ifs, line)) {

        for (std::string seg : line.Split("\\")) {
            std::cout << seg << std::endl;
        }

        system("pause");

        // Find last segment of path
        std::string small_line = line.substr(0, line.find_last_of("\\"));

        // Add path to end of paths list
        paths.push_back(small_line);
        path_lengths.push_back(line.length());
        path_required_cuts.push_back(line.length() - 200);
    }

    // Clear duplicates
    paths.unique();

    // Loop through results
    for (iterator = paths.begin(); iterator != paths.end(); ++iterator) {
        ofs << *iterator << std::endl;
    }

    // Cleanup
    ofs.close();
    ifs.close();

    printf("Outputted file: %s \n", argv[2]);
    printf("Unique long paths: %i", paths.size());

    return 0;
}
