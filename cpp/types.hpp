#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <vector>

// Po branju slike imamo 2D vektor bool
typedef std::vector<std::vector<bool>> bools;
// Za RLE rabimo 2D vektor parov int in bool
typedef std::vector<std::vector<std::pair<int, bool>>> intbools;
// Rezultat Huffmanovega algoritma je 2D vektor string
typedef std::vector<std::vector<std::string>> strings;

#endif
