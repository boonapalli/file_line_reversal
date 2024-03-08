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

public:
  class Exception : public std::exception
  {
    public:
      Exception( const std::string& msg ) :
        msg_(msg)
      {
      }

      const char* what() const noexcept
      {
        return msg_.c_str();
      }

    private:
      std::string msg_;
  };

private:
  virtual void ThreadMain() = 0;

private:
  bool threadStarted;
  std::thread thisThread;
};

#endif // BASIC_THREAD_HPP

