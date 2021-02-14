/**
 * Uporaba cevovoda s štirimi stopnjami.
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

#define REPS 1000

struct huffman_data {
  Huffman *hf;
  Vec<string> *encoded;
};

using namespace std;

// Funkcija za niti, ki berejo.
void* read(void* arg) {
  PipelineStage<int, string, struct image>* stage = (PipelineStage<int, string, struct image>*) arg;

  while (true) {
    auto p = stage->consume(); 
    auto key = p.first;
    auto filename = p.second;

    try {
      auto image_data = Input::read_image(filename.c_str());
      stage->produce(key, image_data);
    } catch(const std::exception& e) {
      std::cerr << filename << ": " << e.what() << '\n';
    }
  }

  return nullptr;
}

// Funkcija za niti, ki prebrane podatke kodirajo z RLE. 
void* rle(void* arg) {
  PipelineStage<int, struct image, Vec<int_bool>*>* stage = (PipelineStage<int, struct image, Vec<int_bool>*>*) arg;

  while (true) {
    auto p = stage->consume(); 
    auto key = p.first;
    auto image_data = p.second;

    auto rle_data = RLE::encode(image_data);
    stage->produce(key, rle_data);
  }
}

// Funkcija za niti, ki RLE podatke kodirajo s huffmanom.  
void* huffman(void* arg) {
  PipelineStage<int, Vec<int_bool>*, huffman_data> *stage = (PipelineStage<int, Vec<int_bool>*, huffman_data>*) arg;

  while (true) {
    auto p = stage->consume(); 
    auto key = p.first;
    auto rle_data = p.second;

    Huffman *hf = Huffman::initialize(rle_data);
    auto encoded = hf->encode();

    huffman_data data;
    data.hf = hf;
    data.encoded = encoded;

    stage->produce(key, data);
  }
}

// Funkcija za niti, ki pisejo.
void* write(void* arg) {
  PipelineStage<int, huffman_data, void>* stage = (PipelineStage<int, huffman_data, void>*) arg;

  for (int i = 0; i < REPS; i++) {
    auto p = stage->consume(); 
    auto key = p.first;
    auto data = p.second;
    auto header = data.hf->header();
    auto filename = "../../out_encoded/out" + std::to_string(key) + ".txt";
    
    try {
      Output::write_encoded(filename, header, data.encoded);
    } catch(const std::exception& e) {
      std::cerr << "Write encoded " << key << ": " << e.what() << '\n';
    }

    data.hf->finalize();
    delete data.encoded;
  }

  // S tem se bo program zaključil (glavna nit čaka na join)
  return nullptr;
}

// Glavna nit
int main(int argc, char const *argv[]) {

  FifoStream<int, string> input_stream(0);
  FifoStream<int, struct image> image_stream(0);
  FifoStream<int, Vec<int_bool>*> encoded_stream(0);
  FifoStream<int, huffman_data> output_stream(0);

  // Prva stopnja cevovoda, image reading
  PipelineStage<int, string, struct image> read_stage(&input_stream, &image_stream);  

  // Druga stopnja cevovoda, run length encoding
  PipelineStage<int, struct image, Vec<int_bool>*> rle_stage(&image_stream, &encoded_stream);

  // Tretja stopnja cevovoda, huffman encoding
  PipelineStage<int, Vec<int_bool>*, huffman_data> huffman_stage(&encoded_stream, &output_stream);

  // Cetrta stopnja cevovoda, writing
  PipelineStage<int, huffman_data, void> write_stage(&output_stream);

  pthread_t read_thread;
  pthread_t rle_thread;
  pthread_t huffman_thread;
  pthread_t write_thread;

  pthread_create(&read_thread, NULL, read, &read_stage);
  pthread_create(&rle_thread, NULL, rle, &rle_stage);
  pthread_create(&huffman_thread, NULL, huffman, &huffman_stage);
  pthread_create(&write_thread, NULL, write, &write_stage);

  // V cevovod posljemo delo
  for (int i = 1; i <= REPS; i++) {
    int file_id = (i % 10) + 1;
    input_stream.produce(i, "../../assets/" + std::to_string(file_id) + ".png");
  } 

  pthread_join(write_thread, NULL);
  return 0;
}
