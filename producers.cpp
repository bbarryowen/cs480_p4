// 825730146

#include <unistd.h>
#include <queue>
#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <ctime>

#include "requestsQueue.h"
#include "threadargs.h"
#include "producers.h"
#include "report.h"

void *produce(void *arg)
{
  // casts passed arguments to data
  ThreadArgs *data = static_cast<ThreadArgs *>(arg);
  RequestType coinType;

  // determines the coin type based on input
  if (data->threadType == 'B')
  {
    coinType = Bitcoin;
  }
  else
  {
    coinType = Ethereum;
  }
  int *numBitcoin;

  // loops while there is less requests produced than the max number allowed
  while (true)
  {

    // determines sleep time based on thread type input
    if (data->threadType == 'B')
    {
      usleep(data->b * 1000);
    }
    else
    {
      usleep(data->e * 1000);
    }

    pthread_mutex_lock(data->cryptoMutex);

    // waits on the number of bitcoin in queue if thread is producing bitcoin
    if (data->threadType == 'B')
    {
      numBitcoin = reinterpret_cast<int *>(&data->requests->inRequestQueue[Bitcoin]);
      data->requests->waitForCount(numBitcoin, &data->requests->maxNumBitCoin, data->cryptoMutex, &data->requests->queueCond);
    }

    // waits for there to be room in the broker
    data->requests->waitForCount(&data->requests->count, &data->requests->maxQueueSize, data->cryptoMutex, &data->requests->queueCond);

    // checks if the max number of requests has been produced yet
    if (data->requests->totalRequests >= data->n)
    {
      *data->isProducing = false;
      pthread_cond_broadcast(&data->requests->queueCond);
      // broadcasts to the condition that checks if consumers are still consuiming in case producers end after consumers
      pthread_cond_broadcast(data->consumerCond);
      pthread_mutex_unlock(data->cryptoMutex);
      return nullptr;
    }

    // adds request to broker
    data->requests->incrimentCount();
    data->requests->addRequest(coinType);
    report_request_added(coinType, data->requests->produced, data->requests->inRequestQueue);

    // checks if thread should still be producing again to handle case where producers end after consumers
    if (data->requests->totalRequests >= data->n)
    {
      *data->isProducing = false;
      pthread_cond_broadcast(&data->requests->queueCond);
      // broadcasts to the condition that checks if consumers are still consuiming in case producers end after consumers
      pthread_cond_broadcast(data->consumerCond);
      pthread_mutex_unlock(data->cryptoMutex);
      return nullptr;
    }
    pthread_mutex_unlock(data->cryptoMutex);
  }
}
