
#include <llp.h>

int main(int argc , char *argv[])
{
    // Declarations
    std::ifstream ifs(argv[1]); // Input stream
    std::ofstream ofs(argv[2]); // Output stream
    std::map<std::string, path_info> offending_paths;

    log("Long Paths Parser v1.0");

    // Sanity checks
    if (!check_arguments(argc)) return 1;
    if (!check_file_streams_open(ifs, ofs)) return 1;

    offending_paths = loop_through_file(ifs);

    write_map_to_file(offending_paths, ofs);

    log(argv[2]);

    // Cleanup
    ofs.close();
    ifs.close();

    return 0;
}

/**
    Checks number of arguments is same as number of required arguments
    (also checks format). Returns true if arguments are correct, false
    if not enough arguments have been supplied
*/
bool check_arguments(int argc)
{
    log("check_arguments");

    if (argc < 3 || argc > 3) {
        log("ERROR: Not enough arguments");
        log("USAGE: long_path_parser.exe [INPUT_FILE] [OUTPUT_FILE]");
        return false;
    }

    return true;
}

/**
    Checks that both input file stream (for reading from the file) and
    output file stream (for outputting the 'cleaned' file) are open and connected
    to a file. Returns true if streams are open, false if streams are closed
*/
bool check_file_streams_open(std::ifstream& input, std::ofstream& output)
{
    log("check_file_streams_open");

    if (!input.is_open()) {
        log("ERROR: Input file could not be opened. Check this file exists.");
        return false;
    } else if (!output.is_open()) {
        log("ERROR: Output file could not be created. Check your privilege.");
        return false;
    }

    return true;
}

/**
    Loops through a file, processing it line by line and returns results in a map.

    For each path in the file the longest part of each path will be determined
    where the longest section (EG C:\test\paths contains the sections C:, test and paths)
    is longer than all other sections and the length of the section from the total path length
    brings the length to under 200 characters.
    If these conditions are not met the whole path is saved. The relative is saved in a map containing
    all 'offending' sections. These sections are either added or updated in the map, keeping track
    of how many other paths the section is causing to go over the 200 character limit.

    NOTE: The path of the longest section is saved in the map, so if the longest section in a path is
    a folder called VERY_VERY_LONG_FOLDER then the path up till that folder will be saved in the map as so:
    'C:\path\up\to\and\including\VERY_VERY_LONG_FOLDER'

    SIDENOTE: In this case the provide file is expected to be a text file and to contain
    one long path per line (over 200 characters).
*/
std::map<std::string, path_info> loop_through_file(std::ifstream& file)
{
    std::map<std::string, path_info> long_paths;
    std::string line;

    log("Cleaning file...");

    while (std::getline(file, line)) {
        bool long_path = true; // Specifies longest section is folder or file

        std::string longest_folder = find_longest_path_section(line);

        // If the offending/long path is due to a long filename, store the directory that contains the file
        if (longest_folder == line) {
            long_path = false;
            longest_folder = line.substr(0, line.find_last_of("\\"));
        }

        add_to_map(long_paths, longest_folder, long_path);
    }

    return long_paths;
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
    Adds a given path to a map of paths. Also updates path data using
    the path_info struct (which stores how many dependant paths, files, length etc)

    We also check if the path already exists in the map, if it does
    then we UPDATE the path data, else we ADD a new set of data for the path.
*/
void add_to_map(std::map<std::string, path_info>& path_map, std::string path, bool is_path)
{
    path_info data;

    if (path_map.find(path) != path_map.end()) {
        data = path_map.find(path)->second; // Poor optimisation (find twice?)

        if (is_path)
            data.dependant_paths++;
        else
            data.dependant_files++;

        path_map.at(path) = data;
    } else {
        if (is_path)
            data.dependant_paths++;
        else
            data.dependant_files++;

        data.len = path.length();
        path_map[path] = data;
    }
}

/**
    Writes a map to a file, using a provided output file stream
*/
void write_map_to_file(std::map<std::string, path_info>& m, std::ofstream& output)
{
    log("Creating new file...");

    for (auto& x: m)
    {
        path_info data = x.second;
        output << "path: " << x.first
               << " length: " << data.len
               << " path_dependants: " << data.dependant_paths
               << " file_dependants: " << data.dependant_files
               << std::endl;
    }

}

/**
    Logs a message to standard output
*/
void log(char* msg)
{
    printf("[LLP] %s \n", msg);
}
