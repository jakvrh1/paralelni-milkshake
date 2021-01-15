#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "types.hpp"
#include "rle.hpp"

class Huffman {
 private:
  static const std::string EOL;

  // bulding encoding table and freeing nodes
  std::string code = "";
  std::vector<int> white_tree, black_tree;
  int cnt = 0;
  void encodeTree(Node *node, const Type &type) {
    if (node->isLeaf()) {
      if (type == Type::White) {
        white_tree.push_back(cnt);
        cnt = 0;
        white_tree.push_back(node->value);

        encode_white.insert({node->value, code});
      }
      else {
        black_tree.push_back(cnt);
        cnt = 0;
        black_tree.push_back(node->value);

        encode_black.insert({node->value, code});
      }

      node->erase();
      return;
    }
    cnt++;

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

  int temp = 0;
  void preorderTraverse(Node *node) {
    std::cout << node->value << " ";
    std::cout << "- ";
    if (node->isLeaf()) {
      std::cout << " x ";
      return;
    }
    if (node->left != nullptr) preorderTraverse(node->left);
    if (node->right != nullptr) preorderTraverse(node->right);
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

    for(auto &i : hf->white_tree) {
      std::cout << i << " ";
    }
    std::cout << std::endl;

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

#endif
