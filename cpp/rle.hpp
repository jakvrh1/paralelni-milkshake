/**
 * Specifikacija verižnega kodiranja in Huffmanovega kanoničnega kodiranja.
 * V štiristopenjskem cevovodu je verižno kodiranje druga stopnja in
 * Huffmanovo kanonično kodiranje tretja stopnja.
 */

#ifndef RLE_HPP
#define RLE_HPP

#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "types.hpp"

class RLE {
 public:
  static Vec<int_bool> *encode(Vec<bool> *data) {
    Vec<int_bool> *encoded_data = new Vec<int_bool>();
    encoded_data->assign(data->size(), std::vector<int_bool>());

    auto &enc = *encoded_data;

    for (int line = 0; line < data->size(); ++line) {
      auto &dl = (*data)[line];

      if (dl.back() == Type::Black)
        dl.push_back(Type::White);
      else
        dl.push_back(Type::Black);

      int cnt = 0;
      for (int i = 0; i < dl.size() - 1; ++i) {
        if (dl[i] == dl[i + 1]) {
          cnt++;
        } else {
          enc[line].push_back({cnt + 1, dl[i]});
          cnt = 0;
        }
      }
      dl.pop_back();
    }

    return &enc;
  }

  static Vec<int_bool> *encode(unsigned char *data) {
    int A4_LINES = 1145;
    int A4_LINE_LENGTH = 1728;
    Vec<int_bool> *encoded_data = new Vec<int_bool>();
    encoded_data->assign(A4_LINES, std::vector<int_bool>());

    auto &enc = *encoded_data;
    int WHITE = 255;

    for (int line = 0; line < A4_LINES; ++line) {
      int cnt = 0;

      for (int pixel = 0; pixel < A4_LINE_LENGTH - 1; ++pixel) {
        int idx = line * A4_LINE_LENGTH + pixel;

        if (data[idx] == data[idx + 1]) {
          cnt++;
        } else {
          if (data[idx] == WHITE)
            enc[line].push_back({cnt + 1, Type::White});
          else
            enc[line].push_back({cnt + 1, Type::Black});

          cnt = 0;
        }
      }

      int idxl = line * A4_LINE_LENGTH + A4_LINE_LENGTH - 1;
      if (data[idxl] == WHITE)
        enc[line].push_back({cnt + 1, Type::White});
      else
        enc[line].push_back({cnt + 1, Type::Black});
    }

    free(data);
    return &enc;
  }
};

#endif
