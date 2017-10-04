#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include <llp.h>

int main(int argc , char *argv[])
{
    // Declarations
    std::ifstream ifs(argv[1]); // Input stream
    std::ofstream ofs(argv[2]); // Output stream
    std::map<std::string, int>::const_iterator map_iterator;
    std::map<std::string, int> offending_paths;//std::pair<int, int>> offending_paths; // path, dependant paths, dependant files
    std::string line;

    // Arguments check
    if (argc < 3 || argc > 3)
    {
        printf("ERROR: Not enough arguments.\n");
        printf("long_path_parser.exe [INPUT_FILE] [OUTPUT_FILE]\n");
        return 1;
    }

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
        bool long_path = true; // Specifies longest section is folder or file

        std::string longest_folder = find_longest_path_section(line);

        // If the offending/long path is due to a long filename, store the directory that contains the file
        if (longest_folder == line) {
            long_path = false;
            longest_folder = line.substr(0, line.find_last_of("\\"));
        }

        add_to_map(offending_paths, longest_folder, long_path);
    }

    for (auto& x: offending_paths)
        //std::cout << "path: " << x.first << " dependants: " << x.second << std::endl;
        ofs << "path: " << x.first << " dependants: " << x.second << std::endl;

    // Cleanup
    ofs.close();
    ifs.close();

    printf("Outputted file: %s \n", argv[2]);

    return 0;
}

/**
    Finds longest section/folder of a given path, and returns the path of
    the folder.

    Works by splitting whole path into parts(sections), we then check if the
    current section is longer than any other sections AND if the length of
    the current section minus the total path length is less than the
    acceptable length threshold (under 200 characters).
    If not then the whole path is returned.

    EG. C:\test\long\very_long_section\some\more\paths\stuff.txt
        would return:
        C:\test\long\very_long_section
        OR
        C:\test\long\path\reallylongfilename.txt
        would return:
        C:\test\long\path\reallylongfilename.txt
*/
std::string find_longest_path_section(std::string path)
{
    std::string cur_path = path;
    std::string path_section = path.substr(path.find_last_of("\\") + 1);
    std::string longest_folder = "";
    int path_len = path.length();
    int max_len = 0;
    int cur_len = path_section.size();

    for (int x = 0; x < std::count(path.begin(), path.end(), '\\'); x++) {

        if (cur_len > max_len && (path_len - cur_len) < 200) {
            longest_folder = cur_path;
            max_len = cur_len;
        }

        // Prep next path section
        cur_path = path.substr(0, cur_path.find_last_of("\\"));
        path_section = cur_path.substr(cur_path.find_last_of("\\") + 1);
        cur_len = path_section.size();
    }

    if (longest_folder == "")
        longest_folder = path;

    return longest_folder;
}

/**
    Adds a given path to a map of paths.

    We also check if the path already exists in the map, if it does
    then we UPDATE the appropriate entry, else we ADD a new entry.
    We also add whether the inputted path is that of a long/offending folder
    or path, this is done using the is_path bool, if this is true we are
    specifying an offending path, else we are inputting an offending/long
    file name at the given path.
*/
void add_to_map(std::map<std::string, int>& path_map, std::string path, bool is_path)
{
    if (path_map.find(path) != path_map.end()) {
        int count = path_map.find(path)->second; // Poor optimisation (find twice?)
        path_map.at(path) = count + 1;
    } else {
        path_map[path] = 1;
    }
}
