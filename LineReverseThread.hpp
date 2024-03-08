#ifndef LINE_REVERSE_THREAD_HPP
#define LINE_REVERSE_THREAD_HPP

#include "FileReadThread.hpp"
#include "FileWriteThread.hpp"
#include "BasicThread.hpp"

class LineReverseThreadType : public BasicThread
{
public:
  LineReverseThreadType(FileReadThreadType& read_thread,
    FileWriteThreadType& write_thread);

private:
  void ThreadMain();
  static void ReverseString(string& str);

private:
  FileReadThreadType& fileReadObj;
  FileWriteThreadType& fileWriteObj;
};

#endif // LINE_REVERSE_THREAD_HPP

