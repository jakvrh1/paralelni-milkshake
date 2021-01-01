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
  auto huffman_data = Huffman::encode(rle_data);
  
  Output::write("test.txt", huffman_data); 
  
  /*
  for(int i = 0; i < huffman_data.size(); ++i) {
    for(int j = 0; j < rle_data[i].size(); ++j) {
      cout << "(" << rle_data[i][j].first << "," << rle_data[i][j].second << ") ";
    }
    cout << endl;
    for(int j = 0; j < huffman_data[i].size(); ++j) {
      cout << huffman_data[i][j] << " ";
    }
    cout << endl;
    if(i == 10) break;
  }*/

   
   


  /*
  for (auto &i : rle_data) {
    for (auto &j : i) std::cout << j.first << " " << j.second << ", ";
    std::cout << "\n";
  }
  std::cout << std::flush;
  */

  return 0;
}
