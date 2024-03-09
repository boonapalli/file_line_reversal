#ifndef FILE_READ_THREAD_HPP
#define FILE_READ_THREAD_HPP

#include "BasicThread.hpp"

#include <fstream>
#include <mutex>
#include <queue>
#include <string>
#include <atomic>

using namespace std;

class FileReadThreadType : public BasicThread
{
public:
  FileReadThreadType(ifstream& in_fs);

public:
  bool GetNextLine(string& next_line);
  bool JobComplete();

private:
  void ThreadMain();

private:
  ifstream& inFS;
  atomic_bool fileReadComplete;
  queue<string> fifoQueue;
  mutex fifoQueueMutex;
};

#endif // FILE_READ_THREAD_HPP

