/**
 * Specifikacija za zapisovanje kode strani v datoteko.
 * V štiristopenjskem cevovodu je to zadnja stopnja.
 */

#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <fstream>
#include <string>
#include <vector>
#include "types.hpp"

class Output {

  public:

    // Zapiše drevesi in  vsebino [data] po vrsticah.
    static void write(
      const std::string &filename, 
      const std::pair<std::vector<int>*, std::vector<int>*> header, 
      const Vec<std::string> *data
    ) {
      std::ofstream file(filename, std::fstream::out | std::fstream::trunc);

      if (!file.good()) {
        throw std::invalid_argument("Cannot open file");
      }

      // Izpisemo huffmanovo drevo
      cout << header.first->size() << " ";
      for (int &j : *header.first) cout << j << " ";
      cout << "\n";

      cout << header.second->size() << " ";
      for (int &j : *header.second) cout << j << " ";
      cout << "\n";

      // Izpišemo vsako vrstico
      for (auto &line : *data) {
        // Znotraj vrstice izpišemo besede s presledki
        for (auto &text : line) {
          file << text << " ";
        }
        file << "\n";
      }
      file << std::flush;

      if (!file.good()) {
        throw std::invalid_argument("Writing to file failed");
      }

      file.close();
    }

};

#endif
