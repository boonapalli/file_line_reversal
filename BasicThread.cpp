
#include "BasicThread.hpp"

#include <iostream>

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
  thisThread = std::move(other.thisThread);
  other.threadStarted = false;
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
    throw(std::runtime_error("Thread already started"));
  }

  threadStarted = true;
  thisThread = std::jthread(&BasicThread::ThreadMain, this);
}

void BasicThread::RequestStop()
{
  if (!threadStarted)
  {
    throw(std::runtime_error("Requesting stop on a thread that's not started."));
  }

  thisThread.request_stop();
}

void BasicThread::Join()
{
  if (!threadStarted)
  {
    throw(std::runtime_error("Join without thread started"));
  }

  thisThread.join();
  threadStarted = false;
}

