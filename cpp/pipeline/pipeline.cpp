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

#define REPS 250

using namespace std;

/**
 * Struct za drugo in tretjo stopnjo cevovoda, kjer podatki prihajajo
 * po enem streamu in odhajajo po drugem.
 */
template <typename T, typename U>
struct io_stream {
  Stream<T>* in;
  Stream<U>* out;
};

/* 
 * Funkcija za niti, ki berejo.
 */
void* read(void* arg) {
  Stream<Vec<bool>*>* read_stream = (Stream<Vec<bool>*>*) arg;

  for (int i = 0; i < REPS; i++) {
    auto image_data = Input::read("../../assets/1.png");
    read_stream->produce(image_data);
    //printf("Read image %d\n", i + 1);
  }

  return nullptr;
}

/*
 * Funkcija za niti, ki prebrane podatke kodirajo z RLE.
 */ 
void* rle(void* arg) {
  io_stream<Vec<bool>*, Vec<int_bool>>* stream = (io_stream<Vec<bool>*, Vec<int_bool>>*) arg;

  int i = 0;
  while (true) {
    auto image_data = stream->in->consume();
    auto rle_data = RLE::encode(*image_data);
    stream->out->produce(rle_data);
    //printf("RLE encoded %d\n", ++i);
  }
}

/*
 * Funkcija za niti, ki RLE podatke kodirajo s huffmanom.
 */  
void* huffman(void* arg) {
  io_stream<Vec<int_bool>, Vec<string>>* stream = (io_stream<Vec<int_bool>, Vec<string>>*) arg;

  int i = 0;
  while (true) {
    auto rle_data = stream->in->consume();
    auto huffman_data = Huffman::encode(rle_data);
    stream->out->produce(huffman_data);
    // printf("Huffman encoded %d\n", ++i);
  }
}

/*
 * Funkcija za niti, ki berejo.
 */
void* write(void* arg) {
  Stream<Vec<string>>* write_stream = (Stream<Vec<string>>*) arg;

  for (int i = 0; i < REPS; i++) {
    auto huffman_data = write_stream->consume();
    Output::write("test.txt", huffman_data);
    //printf("Written %d\n", i + 1);
  }

  // S tem se bo program zaključil (glavna nit čaka na join)
  return nullptr;
}

/*
 * Glavna nit
 */
int main(int argc, char const *argv[]) {
  Stream<Vec<bool>*> read_stream;
  Stream<Vec<int_bool>> encoded_stream;
  Stream<Vec<string>> write_stream;

  // Stream za drugo stopnjo cevovoda, run length encoding
  io_stream<Vec<bool>*, Vec<int_bool>> rle_io_stream;
  rle_io_stream.in = &read_stream;
  rle_io_stream.out = &encoded_stream;

  // Stream za tretjo stopnjo cevovoda, huffman encoding
  io_stream<Vec<int_bool>, Vec<string>> huffman_io_stream;
  huffman_io_stream.in = &encoded_stream;
  huffman_io_stream.out = &write_stream;

  pthread_t read_thread;
  pthread_t rle_thread;
  pthread_t huffman_thread;
  pthread_t write_thread;

  pthread_create(&read_thread, NULL, read, &read_stream);
  pthread_create(&rle_thread, NULL, rle, &rle_io_stream);
  pthread_create(&huffman_thread, NULL, huffman, &huffman_io_stream);
  pthread_create(&write_thread, NULL, write, &write_stream);

  pthread_join(write_thread, NULL);
  return 0;
}
