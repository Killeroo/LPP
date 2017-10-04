#ifndef LLP_H_INCLUDED
#define LLP_H_INCLUDED

#include <string>
#include <map>

std::string find_longest_path_section(std::string path);
void add_to_map(std::map<std::string, int>& path_map, std::string path, bool is_path);

#endif // LLP_H_INCLUDED
