// 825730146

#include <unistd.h>
#include <queue>
#include <iostream>
#include <semaphore.h>
#include <pthread.h>

#include "requestsQueue.h"
#include "threadargs.h"
#include "consumer.h"
#include <semaphore.h>

void *consume(void *arg)
{
  // casts given arguments to data
  ThreadArgs *data = static_cast<ThreadArgs *>(arg);
  RequestType coinType;

  // continuously loops until done consuming
  while (true)
  {
    // sleeps according to if the thread is an X or Y consumer
    if (data->threadType == 'X')
    {
      usleep(data->x * 1000);
    }
    else
    {
      usleep(data->y * 1000);
    }

    pthread_mutex_lock(data->cryptoMutex);
    // checks if data is done producing and broker is empty or if the other consumer thread already finished
    // to determine if the current consumer thread is done
    if (!*data->isProducing && data->requests->count == 0 || !*data->isConsuming)
    {
      // sets is consuming flag to false
      *data->isConsuming = false;
      pthread_cond_broadcast(data->consumerCond);
      pthread_mutex_unlock(data->cryptoMutex);
      return nullptr;
    }
    // waits while broker is empty for producers to produce more requests
    data->requests->waitForCount(&data->requests->minQueueSize, &data->requests->count, data->cryptoMutex, &data->requests->queueCond);

    // removes request from broker
    coinType = data->requests->cryptoRequests.front().type;
    data->requests->popRequest(data->threadType);
    data->requests->decrimentCount();

    // reports removal
    if (data->threadType == 'X')
    {
      report_request_removed(BlockchainX, coinType, data->requests->consumedBlockchainX, data->requests->inRequestQueue);
    }
    else
    {
      report_request_removed(BlockchainY, coinType, data->requests->consumedBlockchainY, data->requests->inRequestQueue);
    }

    pthread_mutex_unlock(data->cryptoMutex);
  }
}
