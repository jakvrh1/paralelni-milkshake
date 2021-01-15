#ifndef STREAM_HPP
#define STREAM_HPP

#include <queue>
#include <pthread.h>

template <typename T> 
class Stream {

  private:
    std::queue<T> queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

  public:
    Stream() {
      mutex = PTHREAD_MUTEX_INITIALIZER;
      cond = PTHREAD_COND_INITIALIZER;
    }

    void produce(T data) {
      // Zaklenemo mutex in dodamo [product] v vrsto
      pthread_mutex_lock(&mutex);
      queue.push(data);

      // Prebudimo eno nit, ki bo prevzela podatke iz vrste
      pthread_cond_signal(&cond);

      pthread_mutex_unlock(&mutex);
    }

    T consume() {
      // Zaklenemo mutex in počakamo, dokler queue ni prazen
      pthread_mutex_lock(&mutex);
      while (queue.empty()) {
        pthread_cond_wait(&cond, &mutex);
      }

      T data = queue.front();
      queue.pop();

      pthread_mutex_unlock(&mutex);
      return data;
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
