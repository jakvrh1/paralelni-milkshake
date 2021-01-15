#ifndef STREAM_HPP
#define STREAM_HPP

#include <map>
#include <queue>
#include <stdexcept>
#include <utility>
#include <pthread.h>

using namespace std;

template <typename K, typename T> 
class Stream {

  private:
    std::queue<pair<K, T> queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

  public:
    Stream() {
      mutex = PTHREAD_MUTEX_INITIALIZER;
      cond = PTHREAD_COND_INITIALIZER;
    }

    virtual void produce(K key, T data) {
      // Zaklenemo mutex in dodamo [product] v vrsto
      pthread_mutex_lock(&mutex);

      pair<K, T> keydata(key, data);
      queue.push(keydata);

      // Prebudimo eno nit, ki bo prevzela podatke iz vrste
      pthread_cond_signal(&cond);

      pthread_mutex_unlock(&mutex);
    }

    virtual pair<K, T> consume(K key) {
      // Zaklenemo mutex in počakamo, dokler queue ni prazen
      pthread_mutex_lock(&mutex);
      while (queue.empty()) {
        pthread_cond_wait(&cond, &mutex);
      }

      pair<K, T> keydata = queue.front();
      queue.pop();

      pthread_mutex_unlock(&mutex);
      return keydata;
    }
};

template <typename K, typename T> 
class OrderedStream : public Stream<K, T> {

  private:
    std::unordered_map<K, T> map;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

  public:
    OrderedStream() {
      mutex = PTHREAD_MUTEX_INITIALIZER;
      cond = PTHREAD_COND_INITIALIZER;
    }

    void produce(K key, T data) override {
      // Zaklenemo mutex in dodamo data v map
      pthread_mutex_lock(&mutex);
      map[key] = data;

      // Prebudimo eno nit, ki bo prevzela podatke iz vrste.
      // Seveda bo se enkrat preverila, ce se kljuc ujema
      pthread_cond_signal(&cond);

      pthread_mutex_unlock(&mutex);
    }

    pair<K, T> consume(K key) override {
      // Zaklenemo mutex in počakamo, dokler ni elementa s tem kljucem
      pthread_mutex_lock(&mutex);

      auto it = map.find(key);
      while ((it = map.find(key)) == map.end()) {
        pthread_cond_wait(&cond, &mutex);
      }

      pair<K, T> keydata(it->first, it->second);
      map.erase(it);

      pthread_mutex_unlock(&mutex);
      return keydata;
    }
};


template <typename T, typename U> 
class ProduceConsumeStream {

  private:
    Stream<T> *consumer;
    Stream<U> *producer;

  public:
    ProduceConsumeStream(Stream<T> *consumer, Stream<U> *producer) {
      this->consumer = consumer;
      this->producer = producer;
    }

    T consume() {
      return consumer->consume();
    }

    void produce(U data) {
      return producer->produce(data);
    }
};

#endif
