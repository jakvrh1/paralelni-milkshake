#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "rle.hpp"
#include "types.hpp"

class Huffman {
 private:
  static const std::string EOL;

  // bulding encoding table and freeing nodes
  std::string code = "";
  std::vector<int> *w_white_tree = nullptr;
  std::vector<int> *w_black_tree = nullptr;
  void encodeTree(Node *node, const Type &type) {
    if (node->isLeaf()) {
      if (type == Type::White) {
        (*w_white_tree).push_back(0);
        (*w_white_tree).push_back(node->value);
        encode_white.insert({node->value, code});
      } else {
        (*w_black_tree).push_back(0);
        (*w_black_tree).push_back(node->value);
        encode_black.insert({node->value, code});
      }

      node->erase();
      return;
    }

    if (type == Type::White)
      (*w_white_tree).push_back(1);
    else
      (*w_black_tree).push_back(1);

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

  void decodeTree(Node *node, const Type &type) {
    if (node->isLeaf()) {
      if (type == Type::White)
        decode_white.insert({code, node->value});
      else
        decode_black.insert({code, node->value});

      node->erase();
      return;
    }

    if (node->left != nullptr) {
      code.push_back('1');
      decodeTree(node->left, type);
      code.pop_back();
    }

    if (node->right != nullptr) {
      code.push_back('0');
      decodeTree(node->right, type);
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
  Vec<int_bool> *rle_data = nullptr;
  Vec<std::string> *encoded_data = nullptr;
  std::unordered_map<int, std::string> encode_white;
  std::unordered_map<int, std::string> encode_black;

  std::unordered_map<std::string, int> decode_white;
  std::unordered_map<std::string, int> decode_black;

  Node *white_tree;
  Node *black_tree;

  static Huffman *initialize(Vec<int_bool> *rle_data) {
    Huffman *hf = new Huffman();
    hf->rle_data = rle_data;

    auto huffman_tree_root_node = Huffman::preparData(*hf->rle_data);
    auto wt = Huffman::huffmanTree(huffman_tree_root_node.first);
    auto bt = Huffman::huffmanTree(huffman_tree_root_node.second);

    hf->w_white_tree = new std::vector<int>();
    hf->w_black_tree = new std::vector<int>();

    hf->encodeTree(wt, Type::White);
    hf->encodeTree(bt, Type::Black);

    return hf;
  }

  static Huffman *initialize(
      std::pair<std::vector<int> *, std::vector<int> *> wb_tree,
      Vec<std::string> *encoded_data) {
    Huffman *hf = new Huffman();
    hf->encoded_data = encoded_data;

    hf->buildTrees(wb_tree);

    /*
    hf->white_tree->disp();
    hf->black_tree->disp();
    */

    hf->decodeTree(hf->white_tree, Type::White);
    hf->decodeTree(hf->black_tree, Type::Black);

    return hf;
  }

  Vec<char> *decode() {
    Vec<char> *decoded_data =
        new Vec<char>(encoded_data->size(), std::vector<char>());

    for (int i = 0; i < encoded_data->size(); ++i) {
      for (int j = 0; j < (*encoded_data)[i].size() - 1; ++j) {
        // we start with white
        if (j % 2 == 0) {
          if (decode_white[(*encoded_data)[i][j]] == 0) continue;
          for (int k = 0; k < decode_white[(*encoded_data)[i][j]]; ++k) 
            (*decoded_data)[i].push_back('0');
        } else {
          if (decode_black[(*encoded_data)[i][j]] == 0) continue;
          for (int k = 0; k < decode_black[(*encoded_data)[i][j]]; ++k)
            (*decoded_data)[i].push_back('1');
        }
      }
    }
    return decoded_data;
  }

  void buildTrees(std::pair<std::vector<int> *, std::vector<int> *> &wb_tree) {
    this->next = -1;
    this->white_tree = this->readTree(Type::White, wb_tree.first);
    this->next = -1;
    this->black_tree = this->readTree(Type::Black, wb_tree.second);
  }

  int next = -1;
  Node *readTree(const Type &type, std::vector<int> *tree) {
    ++next;
    if ((*tree)[next] == 0) {
      ++next;
      Node *root = Node::create((*tree)[next], type);
      return root;
    }
    Node *root = Node::create();
    root->left = readTree(type, tree);

    root->right = readTree(type, tree);
    return root;
  }

  void finalize() {
    if (this->w_white_tree != nullptr) {
      delete w_white_tree;
      w_white_tree = nullptr;
    }
    if (this->w_black_tree != nullptr) {
      delete w_black_tree;
      w_black_tree = nullptr;
    }
    // uporabnik sam poskrbi za free
    /*
    if (this->rle_data != nullptr) {
      delete rle_data;
      rle_data = nullptr;
    }
    if (this->encoded_data != nullptr) {
      delete encoded_data;
      encoded_data = nullptr;
    }
    */
    delete this;
  }

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

  std::pair<std::vector<int> *, std::vector<int> *> header() {
    return {w_white_tree, w_black_tree};
  }

  Vec<std::string> *encode() {
    Vec<std::string> *encoded_data =
        new Vec<std::string>(rle_data->size(), std::vector<std::string>());

    for (int i = 0; i < rle_data->size(); ++i) {
      if ((*rle_data)[i][0].second == Type::White) {
        (*encoded_data)[i].reserve((*rle_data)[i].size() + 1);
      } else {
        (*encoded_data)[i].reserve((*rle_data)[i].size() + 2);
        (*encoded_data)[i].push_back(encode_white[0]);
      }

      for (int j = 0; j < (*rle_data)[i].size(); ++j) {
        if ((*rle_data)[i][j].second == Type::Black)
          (*encoded_data)[i].push_back(encode_black[(*rle_data)[i][j].first]);
        else
          (*encoded_data)[i].push_back(encode_white[(*rle_data)[i][j].first]);
      }
      (*encoded_data)[i].push_back(EOL);
    }

    return encoded_data;
  }
};

const std::string Huffman::EOL = "000000000001";

#endif
