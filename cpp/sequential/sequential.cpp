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

#define REPS 10

using namespace std;

int main(int argc, char const *argv[]) {
  for (int i = 1; i <= REPS; i++) {
    struct image im;
    int file_id = (i % 10) + 1;
    try {
      im = Input::read_image("../../assets/" + std::to_string(file_id) + ".png");
    } catch(const std::exception& e) {
      std::cerr << "Image " << i << ": " << e.what() << '\n';
      // Iz te moke ne bo testa
      continue;
    }

    auto rle_data = RLE::encode(im);

    Huffman *hf = Huffman::initialize(rle_data);
    auto hd = hf->header();
    auto enc = hf->encode();

    //Huffman *hf1 = Huffman::initialize(hd, enc);

    Output::write_encoded("../../out_encoded/test" + std::to_string(file_id) + ".txt", hd, enc);
    //Output::write_image("../../out/" + std::to_string(file_id) + ".png", hf1->decode());
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
