/**
 * Celotna naloga, implementirana kot sekvenčni program.
 */

#include <iostream>
#include <random>

#include "../encoder.hpp"
#include "../input.hpp"
#include "../output.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
  for (int i = 0; i < 100; i++) {
    auto image_data = Input::read("../../assets/1.png");
    auto rle_data = RLE::encode(image_data);
    //auto huffman_data = Huffman::encode(rle_data);
    auto huffman_t_data = Huffman::preparData(rle_data);
    auto h_tree = Huffman::huffmanTree(huffman_t_data);
    //h_tree->traverse();
    //h_tree->preorderTraverse();
    
    //Output::write("test.txt", huffman_data); 
  }

  return 0;
}
