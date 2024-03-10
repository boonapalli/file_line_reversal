#ifndef BASIC_THREAD_HPP
#define BASIC_THREAD_HPP

#include <thread>

class BasicThread
{
public:
  BasicThread();
  BasicThread(const BasicThread&);
  BasicThread(BasicThread&&);
  virtual ~BasicThread();

  void Start();
  void RequestStop();
  void Join();

private:
  virtual void ThreadMain(std::stop_token) = 0;

private:
  bool threadStarted;
  std::jthread thisThread;
};

#endif // BASIC_THREAD_HPP

