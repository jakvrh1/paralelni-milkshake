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

using namespace std;

int main(int argc, char const *argv[]) {
  /*
  // SAMPLE ON REAL IMAGE
  for (int i = 0; i < 1; i++) {
    auto image_data = Input::read("../../assets/2.png");
    auto rle_data = RLE::encode(image_data);
    delete image_data;

    Huffman *hf = Huffman::initialize(rle_data);

    auto hd = hf->header();
    auto enc = hf->encode();
    delete rle_data;

    cout << "ENCODED FILE\n";
    for (int &j : *hd.first) cout << j << " ";
    cout << "\n";
    for (int &j : *hd.second) cout << j << " ";
    cout << "\n";

    for (auto &j : *enc) {
      for (auto &k : j) cout << k;
      cout << "\n";
    }
    cout << flush;

    // hf->black_tree->disp();
    // hf->white_tree->disp();

    delete enc;
    hf->finalize();

    // Output::write("test.txt", huffman_data);
  }
  */

  // TEST SAMPLE
  std::srand(123);

  for (int i = 0; i < 1; ++i) {
    // test1(20, true);
    test2(20, true);
  }

  return 0;
}
