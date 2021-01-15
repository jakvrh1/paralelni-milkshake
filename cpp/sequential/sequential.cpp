/**
 * Celotna naloga, implementirana kot sekvenčni program.
 */

#include <iostream>
#include <random>

#include "../rle.hpp"
#include "../huffman.hpp"
#include "../tester.hpp"
#include "../input.hpp"
#include "../output.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
  

  for (int i = 0; i < 100; i++) {
    auto image_data = Input::read("../../assets/2.png");

    auto rle_data = RLE::encode(*image_data);

    Huffman *hf = Huffman::initialize(rle_data);
    //hf->black_tree->disp();
    //hf->white_tree->disp();

    //Output::write("test.txt", hf->encode());

    hf->finalize();

    //h_tree->traverse();
    //h_tree->preorderTraverse();
    //Output::write("test.txt", huffman_data); 
  }
  


  std::srand(523);
 

  /*
  std::srand(123);

  for(int i = 0; i < 1; ++i) {
    RLE::testRLE(1000, true);
  }
  */


  /*
  while(true) {
    RLE::testRLE(10, true);
  }
  */
  return 0;
}
