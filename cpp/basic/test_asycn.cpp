//
// Created by books on 2020/9/22.
//


#include <iostream>
#include <memory>
#include <future>

#include <gtest/gtest.h>

TEST(CPP_ASYCN, future_valid){
  {
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    EXPECT_TRUE(future.valid());
  }

  {
    std::future<int> future_2;
    EXPECT_FALSE(future_2.valid());
  }

  {
    std::promise<int> promise;
    std::future<int> future=promise.get_future();
    std::vector<std::promise<int>> vec;
    vec.push_back(std::move(promise));
    EXPECT_TRUE(future.valid());
  }
}
