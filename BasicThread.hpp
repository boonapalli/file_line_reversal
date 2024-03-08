#ifndef BASIC_THREAD_HPP
#define BASIC_THREAD_HPP

#include <thread>
#include <exception>

class BasicThread
{
public:
  BasicThread();
  BasicThread(const BasicThread&);
  BasicThread(BasicThread&&);
  virtual ~BasicThread();

  void Start();
  void Join();

private:
  virtual void ThreadMain() = 0;

private:
  bool threadStarted;
  std::thread thisThread;
};

#endif // BASIC_THREAD_HPP

