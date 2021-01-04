/**
 * Uporaba cevovoda s štirimi stopnjami.
 */

#include <iostream>
#include <random>
#include <pthread.h>
#include <unistd.h>

#include "../encoder.hpp"
#include "../input.hpp"
#include "../output.hpp"
#include "stream.hpp"

using namespace std;

template <typename T, typename U>
struct in_out_stream {
  Stream<T>* in;
  Stream<U>* out;
};

void* read(void* arg) {
  Stream<vector<vector<bool>>>* read_stream = (Stream<vector<vector<bool>>>*) arg;

  for (int i = 0; i < 3; i++) {
    auto image_data = Input::read("../../assets/1.png");
    read_stream->push(image_data);
    printf("Read image %d\n", i + 1);
  }

  return nullptr;
}

void* rle(void* arg) {
  in_out_stream<vector<vector<bool>>, vector<vector<pair<int, bool>>>>* stream = (struct in_out_stream<vector<vector<bool>>, vector<vector<pair<int, bool>>>>*) arg;

  int i = 0;
  while (true) {
    auto image_data = stream->in->pop();
    auto rle_data = RLE::encode(image_data);
    stream->out->push(rle_data);
    printf("RLE encoded %d\n", ++i);
  }
}

void* huffman(void* arg) {
  in_out_stream<vector<vector<pair<int, bool>>>, vector<vector<string>>>* stream = (in_out_stream<vector<vector<pair<int, bool>>>, vector<vector<string>>>*) arg;

  int i = 0;
  while (true) {
    auto rle_data = stream->in->pop();
    auto huffman_data = Huffman::encode(rle_data);
    stream->out->push(huffman_data);
    printf("Huffman encoded %d\n", ++i);
  }
}

void* write(void* arg) {
  Stream<vector<vector<string>>>* write_stream = (Stream<vector<vector<string>>>*) arg;

  int i = 0;
  while (true) {
    auto huffman_data = write_stream->pop();
    Output::write("test.txt", huffman_data);
    printf("Written %d\n", ++i);
  }
}

int main(int argc, char const *argv[]) {
  Stream<vector<vector<bool>>> read_stream;
  Stream<vector<vector<pair<int, bool>>>> encoded_stream;
  Stream<vector<vector<string>>> write_stream;

  // Streama za drugo stopnjo cevovoda, run length encoding
  in_out_stream<vector<vector<bool>>, vector<vector<pair<int, bool>>>> rle_in_out_stream;
  rle_in_out_stream.in = &read_stream;
  rle_in_out_stream.out = &encoded_stream;

  // Streama za tretjo stopnjo cevovoda, huffman encoding
  in_out_stream<vector<vector<pair<int, bool>>>, vector<vector<string>>> huffman_in_out_stream;
  huffman_in_out_stream.in = &encoded_stream;
  huffman_in_out_stream.out = &write_stream;

  pthread_t read_thread;
  pthread_t rle_thread;
  pthread_t huffman_thread;
  pthread_t write_thread;

  pthread_create(&read_thread, NULL, read, &read_stream);
  pthread_create(&rle_thread, NULL, rle, &rle_in_out_stream);
  pthread_create(&huffman_thread, NULL, huffman, &huffman_in_out_stream);
  pthread_create(&read_thread, NULL, write, &write_stream);

  pthread_join(read_thread, NULL);
  pthread_join(rle_thread, NULL);

  return 0;
}
