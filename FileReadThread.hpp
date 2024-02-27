#ifndef FILE_READ_THREAD_HPP
#define FILE_READ_THREAD_HPP

#include <thread>
#include <fstream>
#include <mutex>
#include <queue>
#include <string>
#include <atomic>

using namespace std;

class FileReadThreadType
{
public:
  FileReadThreadType(ifstream& in_fs);
  void ThreadMain();

public:
  bool GetNextLine(string& next_line);
  bool JobComplete();

private:
  ifstream& inFS;
  atomic_bool fileReadComplete;
  queue<string> fifoQueue;
  mutex fifoQueueMutex;
  static const size_t SlowdownQueueSize;
};

#endif // FILE_READ_THREAD_HPP

