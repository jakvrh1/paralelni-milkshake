/**
 * Celotna naloga, implementirana kot sekvenčni program.
 */

#include <iostream>
#include <random>

#include "../huffman.hpp"
#include "../input.hpp"
#include "../output.hpp"
#include "../rle.hpp"
#include "../tester.hpp"

#define REPS 1

using namespace std;

int main(int argc, char const *argv[]) {

  for (int i = 1; i <= 10; i++) {
    auto image_data = Input::read_image("../../assets/" + to_string(i) + ".png");
    auto rle_data = RLE::encode(image_data);

    Huffman *hf = Huffman::initialize(rle_data);
    auto hd = hf->header();
    auto enc = hf->encode();

    Output::write_encoded("test.txt", hd, enc);
    delete enc;
    hf->finalize();

    Huffman *hf_recreated = Input::read_encoded("test.txt");
    auto decoded = hf_recreated->decode();
    Output::write_image("../../assets/out/" + to_string(i) + ".png", decoded);
    hf_recreated->finalize();
    delete decoded;
  }

/*   // TEST SAMPLE
  std::srand(123);

  for (int i = 0; i < 1; ++i) {
    // test1(20, true);
    test2(20, true);
  } */

  return 0;
}
