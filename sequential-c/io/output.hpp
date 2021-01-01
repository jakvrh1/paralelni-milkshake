/**
 * Specifikacija za zapisovanje kode strani v datoteko.
 * V štiristopenjskem cevovodu je to zadnja stopnja.
 */

#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <fstream>
#include <string>
#include <vector>

class Output {

  public:

    /**
     * Zapiše vsebino [data] po vrsticah.
     */
    static void write(std::string filename, std::vector<std::vector<std::string>> data) {
      std::ofstream file;
      file.open(filename);

      if (!file.good()) {
        throw std::invalid_argument("Cannot open file");
      }

      // Izpišemo vsako vrstico
      for (auto &line : data) {
        // Znotraj vrstice izpišemo besede s presledki
        for (auto &text : line) {
          file << text << " ";
        }
        file << std::endl;
      }

      if (!file.good()) {
        throw std::invalid_argument("Writing to file failed");
      }

      file.close();
    }

};

#endif
