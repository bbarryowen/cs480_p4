// 825730146

#include <string>
#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <ctime>
#include <cstring>

#include "threadargs.h"
#include "producers.h"
#include "consumer.h"
#include "requestsQueue.h"
#include "report.h"

int main(int argc, char *argv[])
{

  // default values for optional arguments
  int n = 120;
  int x = 0;
  int y = 0;
  int b = 0;
  int e = 0;

  // reads in optional arguments if they are given
  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-n") == 0)
    {
      i += 1;
      n = std::stoi(argv[i]);
    }
    else if (strcmp(argv[i], "-x") == 0)
    {
      i += 1;
      x = std::stoi(argv[i]);
    }
    else if (strcmp(argv[i], "-y") == 0)
    {
      i += 1;
      y = std::stoi(argv[i]);
    }
    else if (strcmp(argv[i], "-b") == 0)
    {
      i += 1;
      b = std::stoi(argv[i]);
    }
    else if (strcmp(argv[i], "-e") == 0)
    {
      i += 1;
      e = std::stoi(argv[i]);
    }
  }

  // sets values to be shared through threads
  int maxVal = 15;
  bool isProducing = true;
  bool isConsuming = true;
  RequestsQueue myRequests = RequestsQueue();
  pthread_mutex_t cryptoMutex;
  pthread_mutex_init(&cryptoMutex, nullptr);
  pthread_cond_t consumerCond;
  pthread_cond_init(&consumerCond, nullptr);

  // sets thread names
  pthread_t bitCoin;
  pthread_t etherium;
  pthread_t xChain;
  pthread_t yChain;

  // starts time
  std::time_t start = std::time(nullptr);

  // creates arguments to be passed to each thread
  // note that each one points to the same variables and the only difference is the first argument that tells the thread what it is supposed to produce or consume
  ThreadArgs *threadArgsE = new ThreadArgs('E', b, e, n, x, y, maxVal, &isProducing, &isConsuming, &myRequests, &cryptoMutex, start, &consumerCond);
  ThreadArgs *threadArgsB = new ThreadArgs('B', b, e, n, x, y, maxVal, &isProducing, &isConsuming, &myRequests, &cryptoMutex, start, &consumerCond);
  ThreadArgs *threadArgsX = new ThreadArgs('X', b, e, n, x, y, maxVal, &isProducing, &isConsuming, &myRequests, &cryptoMutex, start, &consumerCond);
  ThreadArgs *threadArgsY = new ThreadArgs('Y', b, e, n, x, y, maxVal, &isProducing, &isConsuming, &myRequests, &cryptoMutex, start, &consumerCond);

  // creates threads
  pthread_create(&bitCoin, nullptr, produce, threadArgsE);
  pthread_create(&etherium, nullptr, produce, threadArgsB);

  pthread_create(&xChain, nullptr, consume, threadArgsX);
  pthread_create(&yChain, nullptr, consume, threadArgsY);

  // waits for consumer threads to stop consuming
  while (true)
  {
    pthread_mutex_lock(&cryptoMutex);
    // checks if threads are consuming and exits wait if not
    if (!*threadArgsX->isConsuming)
    {
      break;
    }
    pthread_cond_wait(&consumerCond, &cryptoMutex);
    pthread_mutex_unlock(&cryptoMutex);
  }

  // prints production history
  report_production_history(threadArgsE->requests->produced,
                            threadArgsE->requests->consumedBlockchainX,
                            threadArgsE->requests->consumedBlockchainY);
  return 0;
}
