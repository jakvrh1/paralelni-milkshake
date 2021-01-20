#ifndef TYPES_HPP
#define TYPES_HPP

#include <vector>
#include <queue>
#include <string>
#include <iostream>

// Struktura za branje in pisanje slik. Predpostavka je, 
// da je slika grayscale (1 channel).
struct image {
  unsigned char* bytes;
  int width;
  int height;
};

template <typename T> 
using Vec = std::vector<std::vector<T>>;

typedef std::pair<int, bool> int_bool;

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

#endif
