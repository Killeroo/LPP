#ifndef LLP_H_INCLUDED
#define LLP_H_INCLUDED

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>

bool check_arguments(int argc);
bool check_file_streams_open(std::ifstream& input, std::ofstream& output);
std::map<std::string, int> loop_through_file(std::ifstream& file);
std::string find_longest_path_section(std::string path);
void add_to_map(std::map<std::string, int>& path_map, std::string path, bool is_path);
void write_map_to_file(std::map<std::string, int>& m, std::ofstream& output);
void log(char* msg);

#endif // LLP_H_INCLUDED
