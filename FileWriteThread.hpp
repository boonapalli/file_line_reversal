#ifndef FILE_WRITE_THREAD_HPP
#define FILE_WRITE_THREAD_HPP

#include <fstream>
#include <mutex>
#include <queue>
#include <string>
#include <atomic>

using namespace std;

class FileWriteThreadType
{
public:
  FileWriteThreadType(ofstream& out_fs);
  void ThreadMain();

public:
  void PushLine(string& next_line);
  void SetJobWrapUp();

private:
  ofstream& outFS;
  atomic_bool jobWrapUp;
  queue<string> fifoQueue;
  mutex fifoQueueMutex;
};

#endif // FILE_WRITE_THREAD_HPP

