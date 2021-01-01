/**
 * Specifikacija verižnega kodiranja in Huffmanovega kanoničnega kodiranja.
 * V štiristopenjskem cevovodu je verižno kodiranje druga stopnja in
 * Huffmanovo kanonično kodiranje tretja stopnja.
 */

#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <vector>
#include <random>
#include <iostream>

class Huffman {
  private:
  public:
};

class RLE {
 public:
  static std::vector<std::vector<std::pair<int, bool>>> encode(
      std::vector<std::vector<bool>> &data) {
    std::vector<std::vector<std::pair<int, bool>>> encoded_data(
        data.size(), std::vector<std::pair<int, bool>>());

    for (int line = 0; line < data.size(); ++line) {
      if (data[line][data[line].size() - 1]) {
        data[line].push_back(false);
      } else {
        data[line].push_back(true);
      }

      int cnt = 0;
      for (int i = 0; i < data[line].size() - 1; ++i) {
        if (data[line][i] == data[line][i + 1]) {
          cnt++;
        } else {
          encoded_data[line].push_back({cnt + 1, data[line][i]});
          cnt = 0;
        }
      }
    }
    return encoded_data;
  }

  static void testRLE() {
    std::srand(123);
    std::vector<std::vector<bool>> A(10, std::vector<bool>(10, 0));

    for (int i = 0; i < 10; ++i) {
      for (int j = 0; j < 10; ++j) {
        A[i][j] = std::rand() % 2;
        std::cout << A[i][j] << " ";
      }
      std::cout << std::endl;
    }

    auto a = RLE::encode(A);

    for (auto &i : a) {
      for (auto &j : i)
        std::cout << j.first << "" << j.second;
      std::cout << "\n";
    }
    std::cout << std::flush;
  }
};

#endif
