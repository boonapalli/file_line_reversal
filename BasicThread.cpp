
#include "BasicThread.hpp"

#include <iostream>
#include <thread>

BasicThread::BasicThread() :
  threadStarted(false)
{
}

BasicThread::BasicThread(const BasicThread& other) :
  threadStarted(false)
{
  // no copy of the thread
}

BasicThread::BasicThread(BasicThread&& other) :
  threadStarted(other.threadStarted)
{
  std::cout << "BasicThread move constructor called\n";
  thisThread = std::move(other.thisThread);
}

BasicThread::~BasicThread()
{
  if (threadStarted)
  {
    Join();
  }
}

void BasicThread::Start()
{
  if (threadStarted)
  {
    throw(Exception("Thread already started"));
  }

  threadStarted = true;
  thisThread = std::thread(&BasicThread::ThreadMain, this);
}

void BasicThread::Join()
{
  if (!threadStarted)
  {
    throw(Exception("Join without thread started"));
  }

  thisThread.join();
  threadStarted = false;
}

