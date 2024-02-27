
#include "FileReadThread.hpp"

#include <iostream>
#include <chrono>

const size_t FileReadThreadType::SlowdownQueueSize = 1000;

FileReadThreadType::FileReadThreadType(ifstream& in_fs) :
  inFS(in_fs),
  fileReadComplete(false)
{
}

void FileReadThreadType::ThreadMain()
{
  string read_line;
  size_t queue_size;

  while (getline(inFS, read_line))
  {
    {
      std::lock_guard<mutex> lock(fifoQueueMutex);
      queue_size = fifoQueue.size();
    }

    if (queue_size > SlowdownQueueSize)
    {
      cout << "Slowing file read thread\n";
      std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    }

    {
      std::lock_guard<mutex> lock(fifoQueueMutex);
      fifoQueue.push(read_line);
    }
  }

  fileReadComplete = true;
}

bool FileReadThreadType::GetNextLine(string& next_line)
{
  bool line_read = false;

  {
    std::lock_guard<mutex> lock(fifoQueueMutex);
    if (!fifoQueue.empty())
    {
      next_line = fifoQueue.front();
      fifoQueue.pop();
      line_read = true;
    }
  }

  return line_read;
}

bool FileReadThreadType::JobComplete()
{
  bool job_complete = false;

  if (fileReadComplete)
  {
    std::lock_guard<mutex> lock(fifoQueueMutex);
    job_complete = fifoQueue.empty();
  }

  return job_complete;
}
