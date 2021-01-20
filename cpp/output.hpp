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
#include "bit_manipulation.hpp"

#define A4_LINES 1145
#define A4_LINE_LENGTH 1728

class Output {

  public:
    // Zapiše drevesi in  vsebino [data] po vrsticah.
    static void write_encoded(
      const std::string &filename, 
      const std::pair<std::vector<int>*, std::vector<int>*> header, 
      const Vec<std::string> *data
    ) {
      std::ofstream file(filename, std::fstream::out | std::fstream::trunc | std::fstream::binary);

      if (!file.good()) {
        throw std::invalid_argument("Cannot open file");
      }

      // Izpisemo huffmanovo drevo
      Bit::write_bytes(file, header.first->size(), Bit::B3);
      for(int i = 0; i < header.first->size(); ++i) {
        auto &hi = (*header.first)[i];
        // enke
        if(i % 2 == 0) {
          Bit::write_bytes(file, hi, Bit::B1);
        } else {
          Bit::write_bytes(file, hi, Bit::B2);
        }
      }

      Bit::write_bytes(file, header.second->size(), Bit::B3);
      for(int i = 0; i < header.second->size(); ++i) {
        auto &hi = (*header.second)[i];
        // enke
        if(i % 2 == 0) {
          Bit::write_bytes(file, hi, Bit::B1);
        } else {
          Bit::write_bytes(file, hi, Bit::B2);
        }
      }

      Bit::write_bytes(file, (*data).size(), Bit::B3);

      int cnt = 7;
      unsigned char b = 0;
      // Izpišemo vsako vrstico
      for (auto &line : *data) {
        // Znotraj vrstice izpišemo besede s presledki
        Bit::write_bytes(file, line.size(), Bit::B3);
        for (auto &text : line) {
          for(auto &c: text) {
            if(cnt == -1) {
              file << b;
              cnt = 7;
            }
            if(c == '1') {
              Bit::set_bit(b, cnt--, true);
            } else {
              Bit::set_bit(b, cnt--, false);
            }
          }
        }
      }
      if(cnt != 7) {
        file << b;
      }

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
