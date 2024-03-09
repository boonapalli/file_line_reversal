
#include "FileReadThread.hpp"

#include <iostream>
#include <chrono>

FileReadThreadType::FileReadThreadType(ifstream& in_fs) :
  inFS(in_fs),
  fileReadComplete(false)
{
}

void FileReadThreadType::ThreadMain()
{
  string read_line;

  while (getline(inFS, read_line))
  {
    std::lock_guard<mutex> lock(fifoQueueMutex);
    fifoQueue.push(read_line);
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
