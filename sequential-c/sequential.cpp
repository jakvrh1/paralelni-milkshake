/**
 * Celotna naloga, implementirana kot sekvenčni program.
 */

#include <iostream>
#include <random>

#include "encoder/encoder.hpp"
#include "io/input.hpp"
#include "io/output.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
  auto image_data = Input::read("../assets/1.png");
  auto rle_data = RLE::encode(image_data);

  for (auto &i : rle_data) {
    for (auto &j : i) std::cout << j.first << " " << j.second << ", ";
    std::cout << "\n";
  }
  std::cout << std::flush;

  return 0;
}
