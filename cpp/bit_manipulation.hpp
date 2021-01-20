#ifndef BIT_MANIPULATION_HPP
#define BIT_MANIPULATION_HPP

#include <fstream>

class Bit {
 public:
  static const int B1 = 1;
  static const int B2 = 2;
  static const int B3 = 3;

  static int read_bytes(std::ifstream &file, const int bytes) {
    char *c;
    int value = 0;

    if(bytes == B3) {
      file.read(c, 1);
      value |= (*c << 16);
    }

    if(bytes == B2) {
      file.read(c, 1);
      value |= (*c << 8);
    }

    if(bytes == B1) {
      file.read(c, 1);
      value |= *c;
    }

    return value;
  }

  static void write_bytes(std::ofstream &file, const int &value,
                          const int bytes) {
    unsigned char b;
    if (bytes == B3) {
      b = (0xf & (value >> 16));
      file << b;
    }

    if (bytes >= B2) {
      b = (0xf & (value >> 8));
      file << b;
    }

    if (bytes >= B1) {
      b = (0xf & value);
      file << b;
    }
  }

  static void set_bit(unsigned char &b, const int bit, const bool value) {
    if (value) {
      b |= 1 << bit;
    } else {
      b &= ~(1 << bit);
    }
  }
};

#endif
