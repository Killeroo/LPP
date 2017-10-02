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
    std::map<std::string, int>::const_iterator map_iterator;
    std::map<std::string, int> offending_paths;
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
    while (std::getline(ifs, line))
    {
        int path_len = line.length();

        // Loop through each part of path (eg C:\test\test2\test3, C:\test\test2, c:\test) and find longest part (offending path)
        std::string cur_path = line;
        std::string cur_path_section = line.substr(line.find_last_of("\\") + 1);
        std::string longest_folder = "";
        int max_len = 0;
        int cur_len = cur_path_section.size();
        for (int x = 0; x < std::count(line.begin(), line.end(), '\\'); x++)
        {
            // Compare against current longest section of path
            if (cur_len > max_len && (path_len - cur_len) < 200)
            {
                longest_folder = cur_path;
                max_len = cur_len;
            }

            // Load next section of path
            cur_path = line.substr(0, cur_path.find_last_of("\\"));
            cur_path_section = cur_path.substr(cur_path.find_last_of("\\") + 1);
            cur_len = cur_path_section.size();
        }

        // If longest_folder could not be found, save whole path
        if (longest_folder == "")
            longest_folder = line;

        // If path already exists in map
        if (offending_paths.find(longest_folder) != offending_paths.end())//std::find(offending_paths.begin(), offending_paths.end(), longest_folder) != offending_paths.end())
        {
            // Update
            int count = offending_paths.find(longest_folder)->second; // Poor optimisation (find twice?)
            offending_paths.at(longest_folder) = count + 1;
        }
        else
        {
            // Add
            offending_paths[longest_folder] = 1;
        }
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
