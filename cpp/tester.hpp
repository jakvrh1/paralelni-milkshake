#ifndef TESTER_HPP
#define TESTER_HPP

#include <random>
#include <iostream>
#include <vector>

#include "rle.hpp"
#include "huffman.hpp"
#include "types.hpp"


void testRLE(int SIZE, bool izpis) {
    // std::srand(123);
    std::srand(789);
    Vec<bool> A(SIZE, std::vector<bool>(SIZE, 0));

    for (int i = 0; i < SIZE; ++i) {
      for (int j = 0; j < SIZE; ++j) {
        A[i][j] = std::rand() % 2;
        if (izpis) std::cout << A[i][j] << " ";
      }
      if (izpis) std::cout << std::endl;
    }

    auto a = RLE::encode(A);

    if (izpis) {
      for (auto &i : a) {
        for (auto &j : i)
          std::cout << "(" << j.first << ", " << j.second << ") ";
        std::cout << "\n";
      }
      std::cout << std::flush;
    }

    Huffman *hf = Huffman::initialize(a);
    
    auto hd = hf->header();
    auto enc = hf->encode();


    Huffman *hf2 = Huffman::initialize(hd, enc);
    auto enc1 = hf2->decode(enc);

    /*
    if (izpis) {
      // auto enc = hf->encode();
      std::cout << "\n";
      for(auto &i: hd.first) {
        std::cout << i << " ";
      }
      std::cout << "\n";
      for(auto &i: hd.second) {
        std::cout << i << " ";
      }
      std::cout << "\n(0, WHITE) = " << hf->encode_white[0] << std::endl;
      for (auto &i : enc) {
        for (auto &j : i) {
          std::cout << j << " ";
        }
        std::cout << "\n";
      }
      std::cout << std::flush;
    }
    */
    if(izpis) {
      std::cout << "TLE" << std::endl;
      for(auto &i: hf2->decode_white)  {
        std::cout << "("<< i.first << ", " << i.second << ") ";
      }
      std::cout << std::endl;
      std::cout << "TLE" << std::endl;
      for(auto &i: hf2->decode_black)  {
        std::cout << "("<< i.first << ", " << i.second << ") ";
      }
      std::cout << std::endl;
      
      std::cout << "HF1\n";
      std::cout << "\n(0, WHITE) = " << hf->encode_white[0] << std::endl;
      for (auto &i : enc) {
        for (auto &j : i) {
          std::cout << j << " ";
        }
        std::cout << "\n";
      }
      std::cout << "HF2\n";
      std::cout << "\n(0, WHITE) = " << hf->encode_white[0] << std::endl;
      for (auto &i : enc1) {
        for (auto &j : i) {
          std::cout << j << " ";
        }
        std::cout << "\n";
      }
      std::cout << std::flush;
      
    }

    
    hf2->finalize();
    hf->finalize();
  }

#endif
