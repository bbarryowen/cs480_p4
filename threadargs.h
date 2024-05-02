// 825730146

#include <pthread.h>
#include <semaphore.h>
#include <condition_variable>
#include <time.h>

#include "requestsQueue.h"

struct ThreadArgs
{
  // objects that need to be passed to threads
  char threadType;
  int b;
  int e;
  int n;
  int x;
  int y;
  int maxVal;
  bool *isProducing;
  bool *isConsuming;

  time_t start;

  RequestsQueue *requests;
  pthread_mutex_t *cryptoMutex;

  pthread_cond_t *consumerCond;

  ThreadArgs(char threadType, int b, int e, int n, int x, int y, int maxVal, bool *isProducing, bool *isConsuming, RequestsQueue *requests, pthread_mutex_t *cryptoMutex, time_t start, pthread_cond_t *consumerCond) : threadType(threadType), b(b), e(e), n(n), x(x), y(y), maxVal(maxVal), isProducing(isProducing), isConsuming(isConsuming), requests(requests), cryptoMutex(cryptoMutex), start(start), consumerCond(consumerCond) {}
};