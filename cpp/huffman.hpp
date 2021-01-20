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

  // kodirnik {št. bitov} -> {binarno zakodiran string}
  std::unordered_map<int, std::string> encode_white;
  std::unordered_map<int, std::string> encode_black;

  // dekodirnik {binarno zakodiran string} -> {št. bitov}
  std::unordered_map<std::string, int> decode_white;
  std::unordered_map<std::string, int> decode_black;

  // drevesa zgrajena iz header-ja
  Node *white_tree;
  Node *black_tree;

  // zapis header-ja, kjer {0 = leaf, 1 = not leaf}
  // za 0 imamo vedno tudi vrednost noda (dolžina zaporednih bitov).
  std::vector<int> *write_white_tree = nullptr;
  std::vector<int> *write_black_tree = nullptr;

  // služi, kot tretji parameter funkcije encodeTree(Node*, const Type&)
  std::string code = "";

  // grajenje kodirnika in zapis drevesa
  void encodeTree(Node *node, const Type &type) {
    auto &wwt = *write_white_tree;
    auto &wbt = *write_black_tree;

    if (node->isLeaf()) {
      if (type == Type::White) {
        wwt.push_back(0);
        wwt.push_back(node->value);
        encode_white.insert({node->value, code});
      } else {
        wbt.push_back(0);
        wbt.push_back(node->value);
        encode_black.insert({node->value, code});
      }

      node->erase();
      return;
    }

    if (type == Type::White)
      wwt.push_back(1);
    else
      wbt.push_back(1);

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

  // gradnja dekodirnika
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

  // zgradi drevesa iz header-ja
  void buildTrees(std::pair<std::vector<int> *, std::vector<int> *> &wb_tree) {
    this->next = -1;
    this->white_tree = this->readTree(Type::White, wb_tree.first);
    this->next = -1;
    this->black_tree = this->readTree(Type::Black, wb_tree.second);
  }

  // služi, kot tretja spremenljivka funkcije
  // readTree(const Type&, std::vector<int>*)
  int next = -1;
  // gradnja drevesa kjer {0 = leaf, 1 = not leaf}
  // za 0 imamo vedno tudi vrednost noda (dolžina zaporednih bitov).
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

  // gradnja drevesa iz podatkov pridobljenih iz prepareData()
  Node *huffmanTree(std::multimap<int, Node *> &processed_rle_data) {
    while (processed_rle_data.size() > 1) {
      auto p1 = processed_rle_data.begin();
      auto p2 = ++processed_rle_data.begin();

      Node *new_node = Node::create();
      new_node->left = p1->second;
      new_node->right = p2->second;
      // sum frequencies for two nodes and store in node->value
      new_node->value = p1->first + p2->first;

      processed_rle_data.insert({new_node->value, new_node});

      processed_rle_data.erase(p1);
      processed_rle_data.erase(p2);
    }
    return processed_rle_data.begin()->second;
  }

 public:
  Vec<int_bool> *rle_data = nullptr;
  Vec<std::string> *encoded_data = nullptr;

  static Huffman *initialize(Vec<int_bool> *rle_data) {
    Huffman *hf = new Huffman();
    hf->rle_data = rle_data;

    auto huffman_tree_root_node = Huffman::prepareData(*hf->rle_data);
    auto wt = hf->huffmanTree(huffman_tree_root_node.first);
    auto bt = hf->huffmanTree(huffman_tree_root_node.second);

    hf->write_white_tree = new std::vector<int>();
    hf->write_black_tree = new std::vector<int>();

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

  Vec<unsigned char> *decode() {
    Vec<unsigned char> *decoded_data =
        new Vec<unsigned char>(encoded_data->size(), std::vector<unsigned char>());

    auto &enc = *encoded_data;

    for (int i = 0; i < encoded_data->size(); ++i) {
      for (int j = 0; j < enc[i].size() - 1; ++j) {
        auto &enc_ij = (*encoded_data)[i][j];
        auto &dd = (*decoded_data)[i];

        // vsaka vrstica se začne z belimi biti
        if (j % 2 == 0) {
          for (int k = 0; k < decode_white[enc_ij]; ++k) dd.push_back('0');
        } else {
          for (int k = 0; k < decode_black[enc_ij]; ++k) dd.push_back('1');
        }
      }
    }
    return decoded_data;
  }

  void finalize() {
    if (this->write_white_tree != nullptr) {
      delete write_white_tree;
      write_white_tree = nullptr;
    }
    if (this->write_black_tree != nullptr) {
      delete write_black_tree;
      write_black_tree = nullptr;
    }
    if (this->rle_data != nullptr) {
      delete rle_data;
      rle_data = nullptr;
    }
    /*
    if (this->encoded_data != nullptr) {
      delete encoded_data;
      encoded_data = nullptr;
    }
    */
    delete this;
  }

  // funkcija pripravi pair slovarjev nodov
  // { {št. pojavitev, {dolžina, Type::White} }, {št. pojavitev, {dolžina,
  // Type::Black} } }
  static std::pair<std::multimap<int, Node *>, std::multimap<int, Node *>>
  prepareData(Vec<int_bool> &data) {
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

  std::pair<std::vector<int> *, std::vector<int> *> header() {
    return {write_white_tree, write_black_tree};
  }

  // vrne zakodirane podatke
  Vec<std::string> *encode() {
    Vec<std::string> *encoded_data = new Vec<std::string>();
    encoded_data->assign(rle_data->size(), std::vector<std::string>());

    for (int i = 0; i < rle_data->size(); ++i) {
      auto &enc_d_i = (*encoded_data)[i];
      auto &rle_d_i = (*rle_data)[i];

      if (rle_d_i[0].second == Type::White) {
        enc_d_i.reserve(rle_d_i.size() + 1);
      } else {
        enc_d_i.reserve(rle_d_i.size() + 2);
        enc_d_i.push_back(encode_white[0]);
      }

      for (int j = 0; j < rle_d_i.size(); ++j) {
        if (rle_d_i[j].second == Type::Black)
          enc_d_i.push_back(encode_black[rle_d_i[j].first]);
        else
          enc_d_i.push_back(encode_white[rle_d_i[j].first]);
      }
      enc_d_i.push_back(EOL);
    }

    return encoded_data;
  }
};

const std::string Huffman::EOL = "000000000001";

#endif
