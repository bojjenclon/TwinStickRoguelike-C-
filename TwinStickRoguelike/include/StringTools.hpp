#ifndef STRING_TOOLS_HPP
#define STRING_TOOLS_HPP

#include <string>
#include <vector>

// https://stackoverflow.com/questions/236129/split-a-string-in-c
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

#endif