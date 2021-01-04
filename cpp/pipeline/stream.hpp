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

    void push(T data) {
      // Zaklenemo mutex in dodamo [data] v vrsto
      pthread_mutex_lock(&mutex);
      queue.push(data);
      pthread_mutex_unlock(&mutex);

      // Prebudimo eno nit, ki bo prevzela podatke iz vrste
      pthread_cond_signal(&cond);
    }

    T pop() {
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
