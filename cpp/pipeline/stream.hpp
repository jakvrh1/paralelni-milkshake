#ifndef STREAM_HPP
#define STREAM_HPP

#include <queue>
#include <pthread.h>

template <typename T>
class Consumer {

  public:
    virtual ~Consumer() {}
    virtual T consume();
};

template <typename T>
class Producer {

  public:
    virtual ~Producer() {}
    virtual void produce(T product);
};

template <typename T> class Stream : public Consumer<T>, public Producer<T> {

  private:
    std::queue<T> queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

  public:
    Stream() {
      mutex = PTHREAD_MUTEX_INITIALIZER;
      cond = PTHREAD_COND_INITIALIZER;
    }

    virtual void produce(T data) {
      // Zaklenemo mutex in dodamo [product] v vrsto
      pthread_mutex_lock(&mutex);
      queue.push(data);

      // Prebudimo eno nit, ki bo prevzela podatke iz vrste
      pthread_cond_signal(&cond);

      pthread_mutex_unlock(&mutex);
    }

    virtual T consume() {
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

#endif
