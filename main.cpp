#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <queue>
#include <cassert>
#include <ctime>

#include "FileReadThread.hpp"
#include "FileWriteThread.hpp"
#include "LineReverseThread.hpp"

namespace FileLineReversal
{
  const int MIN_REV_THREADS = 1;
  const int MAX_REV_THREADS = 10;
};

using namespace std;
using namespace FileLineReversal;

int main(int argc, char* argv[])
{
  if ((argc < 3) || (argc > 3))
  {
    cout << "Usage: "
         << argv[0]
         << " <input text file> <output text file>\n";
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


  const clock_t c_start = clock();

  FileWriteThreadType fileWriteThread(out_fs);
  FileReadThreadType fileReadThread(in_fs);
  LineReverseThreadType lineReverseThread(fileReadThread, fileWriteThread);

  // start the line reverse thread first, so file read
  // thread doesn't get to fill up the read fifo queue
  lineReverseThread.Start();

  // start the file write thread next, so line reverse
  // threads don't fill up the write fifo queue
  fileWriteThread.Start();

  // finaly start the file read thread
  fileReadThread.Start();

  // wait for the file read to be complete
  while (!fileReadThread.JobComplete())
  {
    std::this_thread::yield();
  }

  // wait for file read thread to be done
  fileReadThread.Join();

  // tell the file write thread to wrap up
  fileWriteThread.RequestStop();

  // wait for file write thread to be done
  fileWriteThread.Join();

  // wait for line reverse thread to join
  lineReverseThread.Join();

  const clock_t c_end = clock();

  cout << "Took " << ((float)c_end - c_start)/CLOCKS_PER_SEC << " seconds\n";

  return 0;
}
