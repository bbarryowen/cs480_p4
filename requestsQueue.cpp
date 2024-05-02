//825730146

#include <queue>
#include <string>
#include <condition_variable>
#include <pthread.h>
#include <mutex>
#include <iostream>
#include "report.h"

#include "requestsQueue.h"

  // adds request with given type
  void RequestsQueue::addRequest(RequestType type)
  {
    cryptoRequests.push(Request(type));
    totalRequests += 1;
    // if type is bitcoin it adds a bitcoin request
    if (type == Bitcoin)
    {

      produced[Bitcoin] += 1;
      inRequestQueue[Bitcoin] += 1;
    }
    // if type is Ethereum it adds an ethereum request
    else
    {
      produced[Ethereum] += 1;
      inRequestQueue[Ethereum] += 1;
    }
  }

  // removes request from broker
  void RequestsQueue::popRequest(char consumer)
  {
    // checks both the type of the coin being removed and the consumer that is removing the coin
    // incriments and decrements appropriate objects for coin and consumer type
    if (cryptoRequests.front().type == Bitcoin)
    {
      inRequestQueue[Bitcoin] -= 1;
      if (consumer == 'X')
      {
        consumedBlockchainX[Bitcoin] += 1;
      }
      else
      {
        consumedBlockchainY[Bitcoin] += 1;
      }
    }
    else
    {
      inRequestQueue[Ethereum] -= 1;
      if (consumer == 'X')
      {
        consumedBlockchainX[Ethereum] += 1;
      }
      else
      {
        consumedBlockchainY[Ethereum] += 1;
      }
    }
    cryptoRequests.pop();
  }

  // incriments the count of the broker
  void RequestsQueue::incrimentCount()
  {
    count += 1;
    pthread_cond_broadcast(&queueCond);
  }

  // decrements the count of the broker
  void RequestsQueue::decrimentCount()
  {
    count -= 1;
    pthread_cond_broadcast(&queueCond);
  }

  // waits while a is larger than or equal to b
  void RequestsQueue::waitForCount(int *a, int *b, pthread_mutex_t *mutex, pthread_cond_t *cond)
  {
    // checks if a is still larger than or equal to b
    while (*a >= *b)
    {
      // puts thread to sleep
      pthread_cond_wait(cond, mutex);
    }
  }