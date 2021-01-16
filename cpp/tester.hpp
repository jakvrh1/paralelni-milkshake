#ifndef TESTER_HPP
#define TESTER_HPP

#include <iostream>
#include <random>
#include <vector>

#include "huffman.hpp"
#include "rle.hpp"
#include "types.hpp"

void test1(int SIZE, bool izpis) {
  Vec<bool> *A = new Vec<bool>(SIZE, std::vector<bool>(SIZE, 0));

  if (izpis) {
    std::cout << "STARTING FILE\n";
  }
  for (int i = 0; i < SIZE; ++i) {
    for (int j = 0; j < SIZE; ++j) {
      (*A)[i][j] = std::rand() % 2;
      if (izpis) std::cout << (*A)[i][j] << " ";
    }
    if (izpis) std::cout << std::endl;
  }

  auto a = RLE::encode(A);
  delete A;

  if (izpis) {
    std::cout << "RLE FOR STARTING FILE\n";
    for (auto &i : *a) {
      for (auto &j : i) std::cout << "(" << j.first << ", " << j.second << ") ";
      std::cout << "\n";
    }
    std::cout << std::flush;
  }

  Huffman *hf = Huffman::initialize(a);
  auto hd = hf->header();
  auto enc = hf->encode();

  if (izpis) {
    std::cout << "HUFFMAN ENCODED FILE\n";
    for (auto &i : *hd.first) {
      std::cout << i << " ";
    }
    std::cout << "\n";
    for (auto &i : *hd.second) {
      std::cout << i << " ";
    }
    std::cout << "\n";
    // std::cout << "\n(0, WHITE) = " << hf->encode_white[0] << std::endl;
    for (auto &i : *enc) {
      for (auto &j : i) {
        std::cout << j;
      }
      std::cout << "\n";
    }
  }

  delete enc;
  delete a;
  hf->finalize();
}

void test2(int SIZE, bool izpis) {
  Vec<bool> *A = new Vec<bool>(SIZE, std::vector<bool>(SIZE, 0));

  if (izpis) {
    std::cout << "STARTING FILE\n";
  }
  for (int i = 0; i < SIZE; ++i) {
    for (int j = 0; j < SIZE; ++j) {
      (*A)[i][j] = std::rand() % 2;
      if (izpis) std::cout << (*A)[i][j] << " ";
    }
    if (izpis) std::cout << std::endl;
  }

  auto a = RLE::encode(A);
  delete A;

  if (izpis) {
    std::cout << "RLE FOR STARTING FILE\n";
    for (auto &i : *a) {
      for (auto &j : i) std::cout << "(" << j.first << ", " << j.second << ") ";
      std::cout << "\n";
    }
    std::cout << std::flush;
  }

  Huffman *hf1 = Huffman::initialize(a);
  auto hd = hf1->header();
  auto enc = hf1->encode();

  if (izpis) {
    std::cout << "HUFFMAN ENCODED FILE\n";
    for (auto &i : *hd.first) {
      std::cout << i << " ";
    }
    std::cout << "\n";
    for (auto &i : *hd.second) {
      std::cout << i << " ";
    }
    std::cout << "\n";
    // std::cout << "\n(0, WHITE) = " << hf1->encode_white[0] << std::endl;
    for (auto &i : *enc) {
      for (auto &j : i) {
        std::cout << j;
      }
      std::cout << "\n";
    }
  }

  Huffman *hf2 = Huffman::initialize(hd, enc);
  auto dec = hf2->decode();

  if (izpis) {
    std::cout << "DECONSTRUCTED FILE\n";
    for (auto &i : *dec) {
      for (auto &j : i) {
        std::cout << j << " ";
      }
      std::cout << "\n";
    }
  }

  std::cout << std::flush;

  delete enc;
  delete a;
  delete dec;
  hf2->finalize();
  hf1->finalize();
}

#endif
