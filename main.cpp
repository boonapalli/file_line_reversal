#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <cassert>

#include "FileReadThread.hpp"
#include "FileWriteThread.hpp"
#include "LineReverseThread.hpp"

using namespace std;

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    cout << "Usage: revlines <input text file> <output text file>\n";
    return -1;
  }

  string in_file_name(argv[1]);
  string out_file_name(argv[2]);

  if (in_file_name == out_file_name)
  {
    cout << "Input and output file names should differ." << in_file_name << "\n";
    return -1;
  }

  ifstream in_fs(in_file_name, in_fs.in);
  if (!in_fs.is_open())
  {
    cout << "Failed to open input text file: " << in_file_name << "\n";
    return -1;
  }

  ofstream out_fs(out_file_name, out_fs.out | out_fs.trunc);
  if (!out_fs.is_open())
  {
    cout << "Failed to open output text file: " << out_file_name << "\n";
    return -1;
  }

  FileWriteThreadType fileWriteObj(out_fs);
  FileReadThreadType fileReadObj(in_fs);
  vector<LineReverseThreadType> lineReverseObjs(2, LineReverseThreadType(fileReadObj, fileWriteObj));
  vector<thread>  lineReverseThreads(lineReverseObjs.size());

  // start the line reverse threads first, so file read
  // thread doesn't get to fill up the read fifo queue
  for (size_t i=0; i<lineReverseObjs.size(); i++)
  {
    lineReverseThreads[i] =
      std::thread(&LineReverseThreadType::ThreadMain, &lineReverseObjs[i]);
  }

  // start the file write thread next, so line reverse
  // threads don't fill up the write fifo queue
  std::thread fileWriteThread(&FileWriteThreadType::ThreadMain, &fileWriteObj);

  std::thread fileReadThread(&FileReadThreadType::ThreadMain, &fileReadObj);

  // wait for the file read to be complete
  while (!fileReadObj.JobComplete())
  {
    std::this_thread::yield();
  }

  // tell the file write thread to wrap up
  fileWriteObj.SetJobWrapUp();

  // wait for file read thread to be done
  fileReadThread.join();

  // wait for file write thread to be done
  fileWriteThread.join();

  // wait for line reverse threads to join
  for (size_t i=0; i<lineReverseObjs.size(); i++)
  {
    lineReverseThreads[i].join();
  }

  return 0;
}
