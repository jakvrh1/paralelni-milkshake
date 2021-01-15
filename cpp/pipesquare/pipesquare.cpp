/**
 * Uporaba cevovoda s štirimi stopnjami;
 * posamezno stopnjo lahko opravlja več niti
 */

#include <iostream>
#include <random>
#include <pthread.h>
#include <unistd.h>

#include "../encoder.hpp"
#include "../input.hpp"
#include "../output.hpp"
#include "../pipeline/stream.hpp"

#define REPS 250

using namespace std;

pthread_mutex_t mutex_read;
int reads = 0;

pthread_mutex_t mutex_write;
int writes = 0;

/* 
 * Funkcija za niti, ki berejo.
 */
void* read(void* arg) {
  ProduceConsumeStream<string, Vec<bool>*>* stream = (ProduceConsumeStream<string, Vec<bool>*>*) arg;

  while (true) {
    auto filename = stream->consume(); 
    auto image_data = Input::read(filename.c_str());
    stream->produce(image_data);
  }

  return nullptr;
}

/*
 * Funkcija za niti, ki prebrane podatke kodirajo z RLE.
 */ 
void* rle(void* arg) {
  ProduceConsumeStream<Vec<bool>*, Vec<int_bool>>* stream = (ProduceConsumeStream<Vec<bool>*, Vec<int_bool>>*) arg;

  while (true) {
    auto image_data = stream->consume();
    auto rle_data = RLE::encode(*image_data);
    stream->produce(rle_data);
  }
}

/*
 * Funkcija za niti, ki RLE podatke kodirajo s huffmanom.
 */  
void* huffman(void* arg) {
  ProduceConsumeStream<Vec<int_bool>, Vec<string>>* stream = (ProduceConsumeStream<Vec<int_bool>, Vec<string>>*) arg;

  while (true) {
    auto rle_data = stream->consume();
    Huffman *hf = Huffman::initialize(rle_data);
    auto huffman_data = hf->encode();
    stream->produce(huffman_data);
  }
}

/*
 * Funkcija za niti, ki berejo.
 */
void* write(void* arg) {
  Stream<Vec<string>>* write_stream = (Stream<Vec<string>>*) arg;

  while (true) {

    pthread_mutex_lock(&mutex_write);
    if (writes < REPS) {
      writes++;
    } else {
      pthread_mutex_unlock(&mutex_write);
      break;
    }
    pthread_mutex_unlock(&mutex_write);

    auto huffman_data = write_stream->consume();
    Output::write("test.txt", huffman_data);
  }

  // S tem se bo program zaključil (glavna nit čaka na join)
  return nullptr;
}

/*
 * Glavna nit
 */
int main(int argc, char const *argv[]) {
  mutex_read = PTHREAD_MUTEX_INITIALIZER;
  mutex_write = PTHREAD_MUTEX_INITIALIZER;

  Stream<string> input_stream;
  Stream<Vec<bool>*> bit_stream;
  Stream<Vec<int_bool>> encoded_stream;
  Stream<Vec<string>> write_stream;

  // Stream za prvo stopnjo cevovoda, image reading
  ProduceConsumeStream<string, Vec<bool>*> read_stream(&input_stream, &bit_stream);  

  // Stream za drugo stopnjo cevovoda, run length encoding
  ProduceConsumeStream<Vec<bool>*, Vec<int_bool>> rle_stream(&bit_stream, &encoded_stream);

  // Stream za tretjo stopnjo cevovoda, huffman encoding
  ProduceConsumeStream<Vec<int_bool>, Vec<string>> huffman_stream(&encoded_stream, &write_stream);

  pthread_t read_thread_1;
  pthread_create(&read_thread_1, NULL, read, &read_stream);
  pthread_t read_thread_2;
  pthread_create(&read_thread_2, NULL, read, &read_stream);
  pthread_t read_thread_3;
  pthread_create(&read_thread_3, NULL, read, &read_stream);

  pthread_t rle_thread_1;
  pthread_create(&rle_thread_1, NULL, rle, &rle_stream);
  pthread_t rle_thread_2;
  pthread_create(&rle_thread_2, NULL, rle, &rle_stream);

  pthread_t huffman_thread_1;
  pthread_create(&huffman_thread_1, NULL, huffman, &huffman_stream);
  pthread_t huffman_thread_2;
  pthread_create(&huffman_thread_2, NULL, huffman, &huffman_stream);

  pthread_t write_thread_1;
  pthread_create(&write_thread_1, NULL, write, &write_stream);
  pthread_t write_thread_2;
  pthread_create(&write_thread_2, NULL, write, &write_stream);

  // V cevovod posljemo delo
  for (int i = 0; i < REPS; i++) 
    input_stream.produce("../../assets/1.png");

  pthread_join(write_thread_1, NULL);
  pthread_join(write_thread_2, NULL);
  return 0;
}
