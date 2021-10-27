#pragma once

#include <functional>
#include <thread>
#include <vector>

namespace raytracerchallenge {
  /**
   * @brief Execute the provided function in parallel
   * @param nElements Number of elements which will be iterated over
   * @param functor Function <int start, int end> which will process a given chunk of the loop
   */
  void parallelFor(int nElements, std::function<void(int start, int end)> functor) {
    int nbThreadsHint = (int)std::thread::hardware_concurrency();
    int nbThreads = nbThreadsHint == 0 ? 8 : (nbThreadsHint);
    int batchSize = nElements / nbThreads;
    int batchRemainder = nElements % nbThreads;
    std::vector<std::thread> threads(nbThreads);
    for (int i = 0; i < nbThreads; ++i) {
      int start = i * batchSize;
      threads[i] = std::thread(functor, start, start + batchSize);
    }
    int start = (int)(nbThreads * batchSize);
    int end = (int)start + batchRemainder;
    functor(start, end);
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
  }
}  // namespace raytracerchallenge