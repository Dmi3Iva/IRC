//
// Created by Shandy Mephesto on 1/13/22.
//

#include "utils.hpp"

vector<string> ft_split(std::string s, char delim) {
  vector<string> result;
  istringstream iss(s);
  string item;
  while (getline(iss, item, delim)) {
    if (!item.empty())
      result.push_back(item);
  }
  return result;
}