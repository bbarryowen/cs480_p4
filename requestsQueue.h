// 825730146

#ifndef REQUESTS_H
#define REQUESTS_H

#include <queue>
#include <string>
#include <condition_variable>
#include <pthread.h>
#include <mutex>
#include <iostream>
#include "report.h"

class RequestsQueue
{
public:
  // defines the structure for a request
  struct Request
  {
    RequestType type;

    Request(RequestType type) : type(type) {}
  };

  // defines the broker and other fields to manage the broker
  std::queue<Request> cryptoRequests;
  int numBitCoin;
  int totalRequests;
  int count;
  pthread_cond_t queueCond;
  pthread_mutex_t queueMutex;

  int totalBTC;
  int totalETC;

  int maxQueueSize;
  int maxNumBitCoin;
  int minQueueSize;

  unsigned int produced[RequestTypeN];
  unsigned int inRequestQueue[RequestTypeN];

  unsigned int consumedBlockchainX[RequestTypeN];
  unsigned int consumedBlockchainY[RequestTypeN];

  // adds request with given type
  void addRequest(RequestType type);

  // removes request from broker
  void popRequest(char consumer);

  // incriments the count of the broker
  void incrimentCount();

  // decrements the count of the broker
  void decrimentCount();

  // waits while a is larger than or equal to b
  void waitForCount(int *a, int *b, pthread_mutex_t *mutex, pthread_cond_t *cond);

  // constructor for broker
  RequestsQueue()
  {
    pthread_cond_init(&queueCond, nullptr);
    pthread_mutex_init(&queueMutex, nullptr);

    numBitCoin = 0;
    totalRequests = 0;
    count = 0;
    maxQueueSize = 15;
    maxNumBitCoin = 6;
    minQueueSize = 0;

    totalBTC = 0;
    totalETC = 0;

    for (int i = 0; i < RequestTypeN; ++i)
    {
      produced[i] = 0;
      inRequestQueue[i] = 0;
      consumedBlockchainX[i] = 0;
      consumedBlockchainY[i] = 0;
    }
  }
};

#endif