/**
 * Specifikacija za branje bitne slike A4 strani.
 * V štiristopenjskem cevovodu je to prva stopnja.
 */

#ifndef INPUT_HPP
#define INPUT_HPP

#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"
#include "types.hpp"
#include "huffman.hpp"

#define A4_LINES 1145
#define A4_LINE_LENGTH 1728

class Input {

  public:

    /**
     * Prebere sliko v datoteki z imenom [filename]. Sliko vrne kot 2D vektor bool.
     */
    static unsigned char* read(const std::string &filename) {
      // Sliko preberemo že kot sivo (število komponent = 1)
      int width, height;
      unsigned char* image = stbi_load(filename.c_str(), &width, &height, NULL, 1);

      // Preverimo, ali datoteka vsebuje slikovne podatke
      if (image == nullptr) {
        throw std::invalid_argument("File is not an image file");
      }

      // Preverimo, ali je slika pravilne velikosti (A4 stran)
      if (width != A4_LINE_LENGTH || height != A4_LINES) {
        throw std::invalid_argument("Image is not of correct size");
      }

      return image;

      /*
      Vec<bool>* bits = new Vec<bool>();
      bits->assign(A4_LINES, std::vector<bool>(A4_LINE_LENGTH, false));

      // Sprehodimo se po sliki in za bele piksle nastavimo true
      // Če naletimo na piksel, ki ni bel ali črn, vrnemo null
      for (int line = 0; line < A4_LINES; line++) {
        for (int pixel = 0; pixel < A4_LINE_LENGTH; pixel++) {
          int idx = line * A4_LINE_LENGTH + pixel;
          if (image[idx] == 255) {
            (*bits)[line][pixel] = true;
          } else if (image[idx] != 0) {
            throw std::invalid_argument("Image is not black and white");
          }
        }
      }

      stbi_image_free(image);

      return bits;
      */
    }

    static Huffman *read_encoded(const std::string &filename) {
      std::ifstream file(filename);

      if (!file.good()) {
        throw std::invalid_argument("Cannot open file");
      }

      int value;
      int count;

      // Belo drevo
      file >> count;
      std::vector<int>* tree_white = new std::vector<int>(count);
      for (int i = 0; i < count; i++) {
        file >> value;
        (*tree_white)[i] = value;
      }

      // Crno drevo
      file >> count;
      std::vector<int>* tree_black = new std::vector<int>(count);
      for (int i = 0; i < count; i++) {
        file >> value;
        (*tree_black)[i] = value;
      }

      std::pair<std::vector<int> *, std::vector<int> *> header(tree_white, tree_black);

      int lines;
      file >> lines;
      Vec<std::string> *data = new Vec<std::string>(lines, std::vector<std::string>());

      // Kodirani podatki
      std::string code;
      for (int i = 0; i < lines; i++) {
        file >> count;
        (*data)[i].reserve(count);
        for (int j = 0; j < count; j++) {
          file >> code;
          (*data)[i].push_back(code);
        }
      }

      return Huffman::initialize(header, data);
    }
};

#endif
