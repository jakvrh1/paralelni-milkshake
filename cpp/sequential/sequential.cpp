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

#define REPS 1000

using namespace std;

int main(int argc, char const *argv[]) {

  for (int i = 0; i < REPS; i++) {
    auto image_data = Input::read("../../assets/1.png");
    auto rle_data = RLE::encode(image_data);
    delete image_data;

    Huffman *hf = Huffman::initialize(rle_data);

    auto hd = hf->header();
    auto enc = hf->encode();
    delete rle_data;

    Output::write("test.txt", hd, enc);
    delete enc;
    hf->finalize();
  }

/*   // TEST SAMPLE
  std::srand(123);

  for (int i = 0; i < 1; ++i) {
    // test1(20, true);
    test2(20, true);
  } */

  return 0;
}
