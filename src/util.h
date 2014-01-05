#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>
#include <vector>

std::vector<std::string> split(const std::string& ss, const std::string& delim, const bool keep_empty=true);

void split(std::vector<std::string>&, 
    const std::string& ss, const std::string& delim, const bool keep_empty=true);

std::string trim_copy(const std::string& s);

void to_lower(std::string& s);

#endif
