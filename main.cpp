#include <iostream>
#include <vector>
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
  if ((argc < 3) || (argc > 4))
  {
    cout << "Usage: "
         << argv[0]
         << " <input text file> <output text file> <optional-num-rev-threads>\n";
    return -1;
  }

  string in_file_name(argv[1]);
  string out_file_name(argv[2]);

  if (in_file_name == out_file_name)
  {
    cout << "Input and output file names should differ." << in_file_name << "\n";
    return -1;
  }

  int num_rev_threads = 1;
  if (argc == 4)
  {
    num_rev_threads = stoi(argv[3]);
    if ((num_rev_threads < MIN_REV_THREADS) ||
        (num_rev_threads > MAX_REV_THREADS))
    {
      cout << "Unsupported number of reversal threads "
           << num_rev_threads
           << " MIN:" << MIN_REV_THREADS
           << " MAX:" << MAX_REV_THREADS
           << "\n";
      return -1;
    }
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

  FileWriteThreadType fileWriteObj(out_fs);
  FileReadThreadType fileReadObj(in_fs);

  // creating two threads for line reversal. don't expect
  // needing more as these threads are fairly efficient
  // and should be faster than the file read thread
  vector<LineReverseThreadType> lineReverseObjs(num_rev_threads, LineReverseThreadType(fileReadObj, fileWriteObj));
  vector<thread> lineReverseThreads(num_rev_threads);

  // start the line reverse threads first, so file read
  // thread doesn't get to fill up the read fifo queue
  for (auto i=0; i<num_rev_threads; i++)
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

  const clock_t c_end = clock();

  cout << "Took " << ((float)c_end - c_start)/CLOCKS_PER_SEC << " seconds\n";

  return 0;
}
