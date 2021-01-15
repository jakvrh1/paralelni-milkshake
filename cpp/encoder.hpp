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
#include <unordered_map>
#include <vector>

#include "types.hpp"

enum Type : bool { White = false, Black = true };

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

  void erase() { delete this; }

  bool isLeaf() {
    if (left == nullptr && right == nullptr) return true;
    return false;
  }

  void disp() { ascii_rep("", this); }

  void ascii_rep(std::string s, Node *node) {
    if (node->isLeaf()) {
      std::cout << s << "---- " << node->value << " " << node->bit << std::endl;
      return;
    }

    ascii_rep(s + "    ", node->right);
    std::cout << s << "----|\n";
    ascii_rep(s + "    ", node->left);
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
  static const std::string EOL;

  // bulding encoding table and freeing nodes
  std::string code = "";
  void encodeTree(Node *node, const Type &type) {
    if (node->isLeaf()) {
      if (type == Type::White)
        encode_white.insert({node->value, code});
      else
        encode_black.insert({node->value, code});

      node->erase();
      return;
    }

    if (node->left != nullptr) {
      code.push_back('1');
      encodeTree(node->left, type);
      code.pop_back();
    }

    if (node->right != nullptr) {
      code.push_back('0');
      encodeTree(node->right, type);
      code.pop_back();
    }

    node->erase();
  }

 public:
  Vec<int_bool> rle_data;
  std::unordered_map<int, std::string> encode_white;
  std::unordered_map<int, std::string> encode_black;

  static Huffman *initialize(Vec<int_bool> &rle_data) {
    Huffman *hf = new Huffman();
    hf->rle_data = rle_data;

    auto huffman_tree_root_node = Huffman::preparData(hf->rle_data);

    hf->encodeTree(Huffman::huffmanTree(huffman_tree_root_node.first),
                   Type::White);
    hf->encodeTree(Huffman::huffmanTree(huffman_tree_root_node.second),
                   Type::Black);

    return hf;
  }

  void finalize() { delete this; }

  static std::pair<std::multimap<int, Node *>, std::multimap<int, Node *>>
  preparData(Vec<int_bool> &data) {
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

    std::multimap<int, Node *> new_data_white;
    std::multimap<int, Node *> new_data_black;

    // ročno vstavimo WHITE dolžine 0 ( v txt datoteki nimamo dolžin 0, jih pa
    // potrebujemo pri zapisu )
    new_data_white.insert({1, Node::create(0, Type::White)});

    for (auto &i : w)
      new_data_white.insert({i.second, Node::create(i.first, Type::White)});
    for (auto &i : b)
      new_data_black.insert({i.second, Node::create(i.first, Type::Black)});

    return {new_data_white, new_data_black};
  }

  static Node *huffmanTree(std::multimap<int, Node *> &processed_rle_data) {
    while (processed_rle_data.size() > 1) {
      auto p1 = processed_rle_data.begin();
      auto p2 = ++processed_rle_data.begin();

      Node *new_node = Node::create();
      new_node->left = p1->second;
      new_node->right = p2->second;
      new_node->value = p1->first + p2->first;

      processed_rle_data.insert({new_node->value, new_node});

      processed_rle_data.erase(p1);
      processed_rle_data.erase(p2);
    }
    return processed_rle_data.begin()->second;
  }

  Vec<std::string> encode() {
    Vec<std::string> encoded_data(rle_data.size(), std::vector<std::string>());

    for (int i = 0; i < rle_data.size(); ++i) {
      if (rle_data[i][0].second == Type::White) {
        encoded_data[i].reserve(rle_data[i].size() + 1);
      } else {
        encoded_data[i].reserve(rle_data[i].size() + 2);
        encoded_data[i].push_back(encode_white[0]);
      }

      for (int j = 0; j < rle_data[i].size(); ++j) {
        if (rle_data[i][j].second == Type::Black)
          encoded_data[i].push_back(encode_black[rle_data[i][j].first]);
        else
          encoded_data[i].push_back(encode_white[rle_data[i][j].first]);
      }
      encoded_data[i].push_back(EOL);
    }

    return encoded_data;
  }
};

const std::string Huffman::EOL = "000000000001";

class RLE {
 public:
  static Vec<int_bool> encode(Vec<bool> &data) {
    Vec<int_bool> encoded_data(data.size(), std::vector<int_bool>());

    for (int line = 0; line < data.size(); ++line) {
      if (data[line].back() == Type::Black)
        data[line].push_back(Type::White);
      else
        data[line].push_back(Type::Black);

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

  static void testRLE(int SIZE, bool izpis) {
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
    auto enc = hf->encode();

    if (izpis) {
      // auto enc = hf->encode();
      std::cout << "(0, WHITE) = " << hf->encode_white[0] << std::endl;
      for (auto &i : enc) {
        for (auto &j : i) {
          std::cout << j << " ";
        }
        std::cout << "\n";
      }
      std::cout << std::flush;
    }

    hf->finalize();
  }
};

#endif
