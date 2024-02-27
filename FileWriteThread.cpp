
#include "FileWriteThread.hpp"

#include <thread>

#include <iostream>

FileWriteThreadType::FileWriteThreadType(ofstream& out_fs) :
  outFS(out_fs),
  jobWrapUp(false)
{
}

void FileWriteThreadType::ThreadMain()
{
  string next_line;
  bool line_read;

  for (;;)
  {
    line_read = false;

    {
      std::lock_guard<mutex> lock(fifoQueueMutex);
      if (!fifoQueue.empty())
      {
        next_line = fifoQueue.front();
        fifoQueue.pop();
        line_read = true;
      }
    }

    if (line_read)
    {
      outFS << next_line << "\n";
    }
    else
    {
      if (jobWrapUp)
      {
        std::lock_guard<mutex> lock(fifoQueueMutex);
        if (fifoQueue.empty())
        {
          break;
        }
      }

      std::this_thread::yield();
    }
  }
}

void FileWriteThreadType::PushLine(string& next_line)
{
  std::lock_guard<mutex> lock(fifoQueueMutex);
  fifoQueue.push(next_line);
}

void FileWriteThreadType::SetJobWrapUp()
{
  jobWrapUp = true;
}
