/**
 * Specifikacija verižnega kodiranja in Huffmanovega kanoničnega kodiranja.
 * V štiristopenjskem cevovodu je verižno kodiranje druga stopnja in
 * Huffmanovo kanonično kodiranje tretja stopnja.
 */

#ifndef ENCODER_HPP
#define ENCODER_HPP

#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <string>
#include <vector>

class Node {
 public:
  int value = 0;
  bool bit;
  Node *left = nullptr;
  Node *right = nullptr;

  static Node *create(int value, int bit) {
    Node *new_node = new Node();
    new_node->value = value;
    new_node->bit = bit;
    return new_node;
  }

  static Node *create() { return new Node(); }

  bool isLeaf() {
    if (left == nullptr && right == nullptr) return true;
    return false;
  }

  void preorderTraverse() {
    if (isLeaf()) {
      std::cout << value << " " << bit << std::endl;
      return;
    }
    if (left != nullptr) left->preorderTraverse();
    if (right != nullptr) right->preorderTraverse();
  }

  void traverse() {
    std::queue<Node *> q;
    q.push(this);

    while (!q.empty()) {
      auto t = q.front();
      if (t->left != nullptr) q.push(t->left);
      if (t->right != nullptr) q.push(t->right);

      if (t->isLeaf()) {
        std::cout << t->value << " " << t->bit << std::endl;
      }
      q.pop();
    }
  }
};

class Huffman {
 private:
  static constexpr int MOD = 64;
  static const std::string EOL;
  static const std::string WHITE[];
  static const std::string BLACK[];

 public:
  static std::multimap<int, Node *> preparData(
      std::vector<std::vector<std::pair<int, bool>>> &data) {
    std::map<int, int> w;
    std::map<int, int> b;

    for (auto &i : data) {
      for (auto &j : i) {
        if (j.second)
          b[j.first]++;
        else
          w[j.first]++;
      }
    }

    std::multimap<int, Node *> new_data;
    for (auto &i : w) new_data.insert({i.second, Node::create(i.first, false)});
    for (auto &i : b) new_data.insert({i.second, Node::create(i.first, true)});

    return new_data;
  }

  static Node *huffmanTree(std::multimap<int, Node *> &data) {
    while (data.size() > 1) {
      auto p1 = data.begin();
      auto p2 = ++data.begin();

      // std::cout << p1->second->value << " " << p2->second->value <<

      Node *new_node = Node::create();
      new_node->left = p1->second;
      new_node->right = p2->second;
      new_node->value = p1->first + p2->first;

      data.insert({new_node->value, new_node});

      data.erase(p1);
      data.erase(p2);
    }

    return data.begin()->second;
  }

  static std::vector<std::vector<std::string>> encode(
      std::vector<std::vector<std::pair<int, bool>>> &data) {
    std::vector<std::vector<std::string>> encoded_data(
        data.size(), std::vector<std::string>());

    for (int i = 0; i < data.size(); ++i) {
      int ind_offset = 0;
      if (!data[i][0].second) {
        encoded_data[i].assign(data[i].size() + 1, WHITE[0]);
      } else {
        encoded_data[i].assign(data[i].size() + 2, WHITE[0]);
        ind_offset = 1;
      }

      for (int j = 0; j < data[i].size(); ++j) {
        int offset = data[i][j].first / MOD;
        int bits = data[i][j].first % MOD;

        if (data[i][j].second) {
          encoded_data[i][j + ind_offset] =
              (offset > 0 ? WHITE[MOD + offset] : "") + WHITE[bits];
        } else {
          encoded_data[i][j + ind_offset] =
              (offset > 0 ? BLACK[MOD + offset] : "") + BLACK[bits];
        }
      }
      encoded_data[i].back() = EOL;
    }

    return encoded_data;
  }
};

const std::string Huffman::EOL = "000000000001";
const std::string Huffman::WHITE[] = {
    "00110101",     "000111",       "0111",         "1000",
    "1011",         "1100",         "1110",         "1111",
    "10011",        "10100",        "00111",        "01000",
    "001000",       "000011",       "110100",       "110101",
    "101010",       "101011",       "0100111",      "0001100",
    "0001000",      "0010111",      "0000011",      "0000100",
    "0101000",      "0101011",      "0010011",      "0100100",
    "0011000",      "00000010",     "00000011",     "00011010",
    "00011011",     "00010010",     "00010011",     "00010100",
    "00010101",     "00001110",     "00010111",     "00101000",
    "00101001",     "00101010",     "00101011",     "00101100",
    "00101101",     "00000100",     "00000101",     "00001010",
    "00001011",     "01010010",     "01010011",     "01010100",
    "01010101",     "00100100",     "00100101",     "01011000",
    "01011001",     "01011010",     "01011011",     "01001010",
    "01001011",     "00110010",     "001110011",    "00110100",
    "11011",        "10010",        "010111",       "0110111",
    "00110110",     "00110111",     "01100100",     "01100101",
    "01101000",     "01100111",     "011001100",    "011001101",
    "011010010",    "101010011",    "011010100",    "011010101",
    "011010110",    "011010111",    "011011000",    "011011001",
    "011011010",    "011011011",    "010011000",    "010011001",
    "010011010",    "011000",       "010011011",    "00000001000",
    "00000001100",  "00000001101",  "000000010010", "000000010011",
    "000000010100", "000000010101", "000000010110", "000000010111",
    "000000011100", "000000011101", "000000011110", "000000011111",
    "000000000001"};

const std::string Huffman::BLACK[] = {"0000110111",
                                      "010",
                                      "11",
                                      "10",
                                      "011",
                                      "0011",
                                      "0010",
                                      "00011",
                                      "000101",
                                      "000100",
                                      "0000100",
                                      "0000101",
                                      "0000111",
                                      "00000100",
                                      "00000111",
                                      "000011000",
                                      "0000010111",
                                      "0000011000",
                                      "0000001000",
                                      "00001100111",
                                      "00001101000",
                                      "00001101100",
                                      "00000110111",
                                      "00000101000",
                                      "00000010111",
                                      "00000011000",
                                      "000011001010",
                                      "000011001011",
                                      "000011001100",
                                      "000011001101",
                                      "000001101000",
                                      "000001101001",
                                      "000001101010",
                                      "000001101011",
                                      "000011010010",
                                      "000011010011",
                                      "000011010100",
                                      "000011010101",
                                      "000011010110",
                                      "000011010111",
                                      "000001101100",
                                      "000001101101",
                                      "000011011010",
                                      "000011011011",
                                      "000001010100",
                                      "000001010101",
                                      "000001010110",
                                      "000001010111",
                                      "000001100100",
                                      "000001100101",
                                      "000001010010",
                                      "000001010011",
                                      "000000100100",
                                      "000000110111",
                                      "000000111000",
                                      "000000100111",
                                      "000000101000",
                                      "000001011000",
                                      "000001011001",
                                      "000000101011",
                                      "000000101100",
                                      "000001011010",
                                      "000001100110",
                                      "000001100111",
                                      "000000111",
                                      "00011001000",
                                      "000011001001",
                                      "000001011011",
                                      "000000110011",
                                      "000000110100",
                                      "000000110101",
                                      "0000001101100",
                                      "0000001101101",
                                      "0000001001010",
                                      "0000001001011",
                                      "0000001001100",
                                      "0000001001101",
                                      "0000001110010",
                                      "0000001110011",
                                      "0000001110100",
                                      "0000001110101",
                                      "0000001110110",
                                      "0000001110111",
                                      "0000001010010",
                                      "0000001010011",
                                      "0000001010100",
                                      "0000001010101",
                                      "0000001011010",
                                      "0000001011011",
                                      "0000001100100",
                                      "0000001100101",
                                      "00000001000",
                                      "00000001100",
                                      "00000001101",
                                      "000000010010",
                                      "000000010011",
                                      "000000010100",
                                      "000000010101",
                                      "000000010110",
                                      "000000010111",
                                      "000000011100",
                                      "000000011101",
                                      "000000011110",
                                      "000000011111",
                                      "000000000001"};

class RLE {
 public:
  static std::vector<std::vector<std::pair<int, bool>>> encode(
      std::vector<std::vector<bool>> &data) {
    std::vector<std::vector<std::pair<int, bool>>> encoded_data(
        data.size(), std::vector<std::pair<int, bool>>());

    for (int line = 0; line < data.size(); ++line) {
      if (data[line][data[line].size() - 1]) {
        data[line].push_back(false);
      } else {
        data[line].push_back(true);
      }

      int cnt = 0;
      for (int i = 0; i < data[line].size() - 1; ++i) {
        if (data[line][i] == data[line][i + 1]) {
          cnt++;
        } else {
          encoded_data[line].push_back({cnt + 1, data[line][i]});
          cnt = 0;
        }
      }
      data[line].pop_back();
    }
    return encoded_data;
  }

  static void testRLE() {
    std::srand(123);
    std::vector<std::vector<bool>> A(10, std::vector<bool>(10, 0));

    for (int i = 0; i < 10; ++i) {
      for (int j = 0; j < 10; ++j) {
        A[i][j] = std::rand() % 2;
        std::cout << A[i][j] << " ";
      }
      std::cout << std::endl;
    }

    auto a = RLE::encode(A);

    for (auto &i : a) {
      for (auto &j : i) std::cout << j.first << "" << j.second;
      std::cout << "\n";
    }
    std::cout << std::flush;
  }
};

#endif
