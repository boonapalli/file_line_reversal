
#include "BasicThread.hpp"

#include <iostream>

BasicThread::BasicThread()
{
}

BasicThread::BasicThread(const BasicThread& other)
{
  // no copy of the thread
}

BasicThread::BasicThread(BasicThread&& other)
{
  thisThread = std::move(other.thisThread);
}

BasicThread::~BasicThread()
{
}

void BasicThread::Start()
{
  thisThread = std::jthread(&BasicThread::ThreadMain, this);
}

void BasicThread::RequestStop()
{
  thisThread.request_stop();
}

void BasicThread::Join()
{
  thisThread.join();
}

