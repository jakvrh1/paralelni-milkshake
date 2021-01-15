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
  static Vec<int_bool> encode(Vec<bool> &data) {
    Vec<int_bool> encoded_data(data.size(), std::vector<int_bool>());

    for (int line = 0; line < data.size(); ++line) {
      if (data[line].back() == Type::Black)
        data[line].push_back(Type::White);
      else
        data[line].push_back(Type::Black);

      int cnt = 0;
      for (int i = 0; i < data[line].size() - 1; ++i) {
        if (data[line][i] == data[line][i + 1]) {
          cnt++;
        } else {
          encoded_data[line].push_back({cnt + 1, data[line][i]});
          cnt = 0;
        }
      }
      data[line].pop_back();
    }
    return encoded_data;
  }

  
};

#endif
