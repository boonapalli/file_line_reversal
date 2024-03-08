
#include "LineReverseThread.hpp"

#include <iostream>
#include <thread>

LineReverseThreadType::LineReverseThreadType(FileReadThreadType& read_thread,
                                             FileWriteThreadType& write_thread) : BasicThread(),
  fileReadObj(read_thread),
  fileWriteObj(write_thread)
{
}

void LineReverseThreadType::ReverseString(string& str)
{
  size_t len = str.length();
  for (size_t i=0; i<len/2; i++)
  {
    swap(str[i], str[len-i-1]);
  }
}

void LineReverseThreadType::ThreadMain()
{
  while (!fileReadObj.JobComplete())
  {
    string next_line;
    if (fileReadObj.GetNextLine(next_line))
    {
      ReverseString(next_line);
      fileWriteObj.PushLine(next_line);
    }
    else
    {
      this_thread::yield();
    }
  }
}

