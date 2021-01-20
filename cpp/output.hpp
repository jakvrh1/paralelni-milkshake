/**
 * Specifikacija za zapisovanje kode strani v datoteko.
 * V štiristopenjskem cevovodu je to zadnja stopnja.
 */

#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <fstream>
#include <string>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "libs/stb_image_write.h"
#include "types.hpp"

#define A4_LINES 1145
#define A4_LINE_LENGTH 1728

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
      file << header.first->size() << " ";
      for (int &j : *header.first) file << j << " ";
      file << "\n";

      file << header.second->size() << " ";
      for (int &j : *header.second) file << j << " ";
      file << "\n";

      file << (*data).size() << "\n";
      // Izpišemo vsako vrstico
      for (auto &line : *data) {
        // Znotraj vrstice izpišemo besede s presledki
        file << line.size() << " ";
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

  static void write_image(const std::string &filename, const Vec<unsigned char>* data) {
    // Razvijemo data po vrsticah
    unsigned char *flat = (unsigned char*) malloc(A4_LINES * A4_LINE_LENGTH);
    for (int line = 0; line < A4_LINES; line++)
      for (int pixel = 0; pixel < A4_LINE_LENGTH; pixel++)
        flat[line * A4_LINE_LENGTH + pixel] = (*data)[line][pixel];
    
    // Zapisemo s STB
    int result = stbi_write_png(
      filename.c_str(),                       // filename
      A4_LINE_LENGTH,                         // width
      A4_LINES,                               // height
      1,                                      // channels (grayscale)
      flat,                                   // image data
      A4_LINE_LENGTH * sizeof(unsigned char)  // stride (bytes between rows)
    );
  }

};

#endif
