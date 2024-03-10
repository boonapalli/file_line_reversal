#ifndef FILE_WRITE_THREAD_HPP
#define FILE_WRITE_THREAD_HPP

#include "BasicThread.hpp"

#include <fstream>
#include <mutex>
#include <queue>
#include <string>
#include <atomic>

using namespace std;

class FileWriteThreadType : public BasicThread
{
public:
  FileWriteThreadType(ofstream& out_fs);

public:
  void PushLine(string& next_line);

private:
  void ThreadMain(std::stop_token);

private:
  ofstream& outFS;
  queue<string> fifoQueue;
  mutex fifoQueueMutex;
};

#endif // FILE_WRITE_THREAD_HPP

