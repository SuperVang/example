//
// Created by books on 2020/6/26.
//

#include <boost/thread/barrier.hpp>
#include <iostream>
#include <thread>

#include <gtest/gtest.h>

TEST(BOOST_BARRIER, stop_barrier)
{
  class B
  {
   public:
    B(std::function<bool()> func) : m_cb(func)
    {
      m_thread = std::thread{std::ref(*this)};
    }

    ~B() {
      m_thread.join(); }

    void operator()()
    {
      while (m_cb()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
      }
    }

    std::function<bool()> m_cb;
    std::thread m_thread;
  };

  class A
  {
   public:
    A() : m_barrier(2)
    {
      m_b = std::make_unique<B>(std::bind(&A::internal_trigger, this));
    };
    ~A()
    {
      m_flag = false;
	  m_barrier.wait();
      m_b.reset(nullptr);
    };

    void external_trigger() {
      m_barrier.wait();
    }

    bool internal_trigger()
    {
      m_barrier.wait();
      return m_flag;
    }

    void update() { std::cout << "update once" << std::endl; }

    bool m_flag = true;
    boost::barrier m_barrier;
    std::unique_ptr<B> m_b;
  };

  A a;
  for (int i = 0; i < 1; i++) {
    a.external_trigger();
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
  }
}
