#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <map>
#include <vector>

#include <algorithm>

const std::vector<std::string> explode(const std::string& str, const char& c);

//https://stackoverflow.com/questions/4702732/the-program-cant-start-because-libgcc-s-dw2-1-dll-is-missing

int main(int argc , char *argv[])
{
    // Declarations
    std::ifstream ifs(argv[1]); // Input stream
    std::ofstream ofs(argv[2]); // Output stream
    std::list<std::string>::const_iterator iterator; // Iterator for looping through results
    std::list<std::string> paths;
    std::list<int> path_lengths;
    std::list<int> path_required_cuts;
    std::string line;
    std::map<std::string, int, int> path_map;

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
    while (std::getline(ifs, line))
    {
        int path_len = line.length();
        int line_over_limit = line.length() - 200;

        // Loop through each part of path (eg C:\test\test2\test3, C:\test\test2, c:\test)
        std::string cur_path = line;
        std::string cur_folder = line.substr(line.find_last_of("\\") + 1);
        std::string longest_folder = "";
        int max_len = 0;
        int cur_len = 0;
        for (int x = 0; x < std::count(line.begin(), line.end(), '\\'); x++)
        {
            cur_len = cur_folder.size();

            if (cur_len > max_len && (path_len - cur_len) < 200)
                longest_folder = cur_folder;

            std::cout << cur_path << std::endl;
            std::cout << cur_folder << std::endl;
            std::cout << longest_folder + "\n" << std::endl;

            cur_path = line.substr(0, cur_path.find_last_of("\\"));
            cur_folder = cur_path.substr(cur_path.find_last_of("\\") + 1);
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

/// Exploded a string into segments using a given delimeter (similar to PHP explode)
const std::vector<std::string> explode(const std::string& str, const char& c)
{
    std::string buffer{""};
    std::vector<std::string> vec;

    for (auto seg : str)
    {
        if (seg != c) buffer += seg; else // If segment isnt delimenter (c) add to buffer
        if (seg == c && buffer != "") { vec.push_back(buffer); buffer = "";} // if segment container delimenter and buffer is not empty add to vector
    }
    if (buffer != "") vec.push_back(buffer); // Store any remaining buffer content to vector

    return vec;
}
