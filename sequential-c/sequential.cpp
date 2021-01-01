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

  for (int i = 0; i < 100; i++) {
    auto image_data = Input::read("../assets/1.png");
    auto rle_data = RLE::encode(image_data);
    auto huffman_data = Huffman::encode(rle_data);
    
    Output::write("test.txt", huffman_data); 
  }

  return 0;
}
