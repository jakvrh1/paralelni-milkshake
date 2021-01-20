#ifndef STREAM_HPP
#define STREAM_HPP

#include <limits>
#include <map>
#include <queue>
#include <stdexcept>
#include <utility>
#include <pthread.h>

using namespace std;

// Abstrakcija za producer-consumer relacijo. Stream<K, T> 
// omogoca, da na thread-safe nacin (naloga implementacije) 
// proizvajamo (produce) in porabljamo (consume) naloge.
// 
// Vsaka naloga je sestavljena iz kljuca K in podatkov T. Kljuc K sluzi
// kot identifikator za nalogo.
//
// Operacija consume lahko sprejme tudi kljuc, ki pove,
// katero nalogo zelimo porabiti.
//

template <typename K, typename T>
class Stream {

  public:
    virtual void produce(K key, T data);
    virtual pair<K, T> consume();
    virtual pair<K, T> consume(K key);
};

// Implementacija Stream, ki sledi principu FIFO. Vrstni red
// produciranja je tako enak kot vrstni red porabljanja.
// 
// Za hranjenje nalog je uporabljena vrsta parov kljucev in podatkov.
//
template <typename K, typename T> 
class FifoStream : public Stream<K, T> {

  private:
    std::queue<pair<K, T>> queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond_produced;
    pthread_cond_t cond_consumed;
    unsigned int max_size;

  public:
    FifoStream(unsigned int max_size) {
      mutex = PTHREAD_MUTEX_INITIALIZER;
      cond_produced = PTHREAD_COND_INITIALIZER;
      cond_consumed = PTHREAD_COND_INITIALIZER;
      if (max_size > 0) this->max_size = max_size;
      else this->max_size = UINT_MAX;
    }

    void produce(K key, T data) override {
      // Zaklenemo mutex in cakamo, dokler je vrsta polna
      pthread_mutex_lock(&mutex);
      while (queue.size() >= max_size) {
        pthread_cond_wait(&cond_consumed, &mutex);
      }
  
      pair<K, T> keydata(key, data);
      queue.push(keydata);

      pthread_mutex_unlock(&mutex);

      // Prebudimo eno nit, ki bo prevzela podatke iz vrste
      pthread_cond_signal(&cond_produced);
    }

    pair<K, T> consume() override {
      // Zaklenemo mutex in čakamo, dokler je vrsta prazna
      pthread_mutex_lock(&mutex);
      while (queue.empty()) {
        pthread_cond_wait(&cond_produced, &mutex);
      }

      pair<K, T> keydata = queue.front();
      queue.pop();

      pthread_mutex_unlock(&mutex);

      // Prebudimo eno nit, ki bo dodala podatke v vrsto
      pthread_cond_signal(&cond_consumed);

      return keydata;
    }

    // TODO search in queue?
    pair<K, T> consume(K key) override {
      return consume();
    }
};

// Abstrakcija za stopnjo v cevovodu z uporabo dveh Stream.
// Prvi Stream se uporablja za consume operacijo; podatki po prvem
// Stream torej prihajajo v stopnjo cevovoda. Po drugem Stream
// obdelani/predelani podatki odhajajo (preko produce).
//
template <typename K, typename T, typename U> 
class PipelineStage {

  private:
    Stream<K, T> *consumer;
    Stream<K, U> *producer;

  public:
    PipelineStage(Stream<K, T> *consumer, Stream<K, U> *producer) {
      this->consumer = consumer;
      this->producer = producer;
    }

    pair<K, T> consume() {
      return consumer->consume();
    }

    pair<K, T> consume(K key) {
      return consumer->consume(key);
    }

    void produce(K key, U data) {
      return producer->produce(key, data);
    }
};

// Specializacija za stopjo cevovoda, ki ima samo consumer del
//
template <typename K, typename T> 
class PipelineStage<K, T, void> {

  private:
    Stream<K, T> *consumer;

  public:
    PipelineStage(Stream<K, T> *consumer) {
      this->consumer = consumer;
    }

    pair<K, T> consume() {
      return consumer->consume();
    }

    pair<K, T> consume(K key) {
      return consumer->consume(key);
    }
};

#endif
