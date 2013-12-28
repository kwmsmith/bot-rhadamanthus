#include "util.h"
#include <algorithm>
#include <iterator>

void split(std::vector<std::string>& result, 
    const std::string& s, const std::string& delim, const bool keep_empty)
{
  if (delim.empty()) {
    result.push_back(s);
  }
  std::string::const_iterator substart = s.begin(), subend;
  while (true) {
    subend = std::search(substart, s.end(), delim.begin(), delim.end());
    std::string temp(substart, subend);
    if (keep_empty || !temp.empty()) {
      result.push_back(temp);
    }
    if (subend == s.end()) {
      break;
    }
    substart = subend + delim.size();
  }
}

std::string trim_copy(const std::string& s)
{
  auto ch = s.cbegin();
  auto rch = s.crbegin();
  for(; *ch == ' '; ++ch);
  for(; *rch == ' '; ++rch);
  return std::string(ch, rch.base());
}

void to_lower(std::string& s)
{
  for(auto ch=s.begin(); ch != s.end(); ++ch) {
    if ('A' <= *ch && 'Z' >= *ch) {
      *ch = (*ch - 'A') + 'a';
    }
  }
}
