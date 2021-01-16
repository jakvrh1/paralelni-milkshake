/**
 * Uporaba cevovoda s štirimi stopnjami;
 * posamezno stopnjo lahko opravlja več niti
 */

#include <iostream>
#include <random>
#include <pthread.h>
#include <unistd.h>

#include "../input.hpp"
#include "../rle.hpp"
#include "../huffman.hpp"
#include "../output.hpp"
#include "../stream.hpp"

#define REPS 250

#define READ_THREADS 2
#define RLE_THREADS 2
#define HUFFMAN_THREADS 2
#define WRITE_THREADS 2

using namespace std;

// ker vec niti pise, moramo uporabiti mutex,
// da povecujemo stevec [writes], dokler
// ne dosezemo stevila REPS
pthread_mutex_t mutex_write;
int writes = 0;

// Funkcija za niti, ki berejo.
void* read(void* arg) {
  PipelineStage<int, string, Vec<bool>*>* stage = (PipelineStage<int, string, Vec<bool>*>*) arg;

  while (true) {
    auto p = stage->consume(); 
    auto key = p.first;
    auto filename = p.second;

    auto image_data = Input::read(filename.c_str());
    stage->produce(key, image_data);
  }

  return nullptr;
}

// Funkcija za niti, ki prebrane podatke kodirajo z RLE. 
void* rle(void* arg) {
  PipelineStage<int, Vec<bool>*, Vec<int_bool>>* stage = (PipelineStage<int, Vec<bool>*, Vec<int_bool>>*) arg;

  while (true) {
    auto p = stage->consume(); 
    auto key = p.first;
    auto image_data = p.second;

    auto rle_data = RLE::encode(*image_data);
    stage->produce(key, rle_data);
  }
}

// Funkcija za niti, ki RLE podatke kodirajo s huffmanom. 
void* huffman(void* arg) {
  PipelineStage<int, Vec<int_bool>, Vec<string>>* stage = (PipelineStage<int, Vec<int_bool>, Vec<string>>*) arg;

  while (true) {
    auto p = stage->consume(); 
    auto key = p.first;
    auto rle_data = p.second;

    Huffman *hf = Huffman::initialize(rle_data);
    stage->produce(key, hf->encode());
    hf->finalize();
  }
}

// Funkcija za niti, ki pisejo
void* write(void* arg) {
  PipelineStage<int, Vec<string>, void>* stage = (PipelineStage<int, Vec<string>, void>*) arg;

  while (true) {

    pthread_mutex_lock(&mutex_write);
    if (writes < REPS) {
      writes++;
    } else {
      pthread_mutex_unlock(&mutex_write);
      break;
    }
    pthread_mutex_unlock(&mutex_write);

    auto p = stage->consume(); 
    auto key = p.first;
    auto huffman_data = p.second;

    Output::write("test.txt", huffman_data);
  }

  // S tem se bo program zaključil (glavna nit čaka na join)
  return nullptr;
}

/*
 * Glavna nit
 */
int main(int argc, char const *argv[]) {
  mutex_write = PTHREAD_MUTEX_INITIALIZER;

  Stream<int, string>* input_stream;
  Stream<int, Vec<bool>*>* bit_stream;
  Stream<int, Vec<int_bool>>* encoded_stream;
  Stream<int, Vec<string>>* output_stream;

  input_stream = new FifoStream<int, string>();
  bit_stream = new FifoStream<int, Vec<bool>*>();
  encoded_stream = new FifoStream<int, Vec<int_bool>>();
  output_stream = new FifoStream<int, Vec<string>>();

  // Prva stopnja cevovoda, image reading
  PipelineStage<int, string, Vec<bool>*> read_stage(input_stream, bit_stream);
  pthread_t read_threads[READ_THREADS];
  for (int i = 0; i < READ_THREADS; i++)
    pthread_create(&read_threads[i], NULL, read, &read_stage);

  // Druga stopnja cevovoda, run length encoding
  PipelineStage<int, Vec<bool>*, Vec<int_bool>> rle_stage(bit_stream, encoded_stream);
  pthread_t rle_threads[RLE_THREADS];
  for (int i = 0; i < RLE_THREADS; i++)
    pthread_create(&rle_threads[i], NULL, rle, &rle_stage);

  // Tretja stopnja cevovoda, huffman encoding
  PipelineStage<int, Vec<int_bool>, Vec<string>> huffman_stage(encoded_stream, output_stream);
  pthread_t huffman_threads[HUFFMAN_THREADS];
  for (int i = 0; i < HUFFMAN_THREADS; i++)
    pthread_create(&huffman_threads[i], NULL, huffman, &huffman_stage);

  // Cetrta stopnja cevovoda, writing
  PipelineStage<int, Vec<string>, void> write_stage(output_stream);
  pthread_t write_threads[WRITE_THREADS];
  for (int i = 0; i < WRITE_THREADS; i++)
    pthread_create(&write_threads[i], NULL, write, &write_stage);

  // V cevovod posljemo delo
  for (int i = 0; i < REPS; i++) 
    input_stream->produce(i, "../../assets/1.png");

  // Pocakamo, da se pisanje zakljuci
  for (int i = 0; i < WRITE_THREADS; i++)
    pthread_join(write_threads[i], NULL);

  free(input_stream);
  free(bit_stream);
  free(encoded_stream);
  free(output_stream);

  return 0;
}
