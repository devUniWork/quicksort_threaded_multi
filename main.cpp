#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Timer.h"
#include <mutex>
using namespace std;

std::mutex lockMe;

// There are files holding sets of 100,000 random numbers in ../randoms/
int loadRandsFile(vector<int>& destination, string fileName);
// Write an array to file if you want to confirm sort is working
bool writeRandsFile(vector<int>& randsArray, string filename);

// Quicksort a portion (or all of) a vector of ints in ascending order
// args: reference to the vector, start and end indices of the section we want to sort
void quicksort(vector<int>& numbers, int start, int end);
// Used by quicksort: chooses pivot, moves lower numbers left, higher numbers right
int partition(vector<int>& numbers, int start, int end);
// Swap the values of two variables (using references)
void swap(int& left, int& right);

void getNumberLines(string filename);

int main()
{
  int listsCount = 16;  
  // How many lists would you like to load and sort?
  // read on length of list to generate array length


  int listLength = 1000;   // How many numbers are in each list?
  vector<vector<int>> randLists(listsCount);
  
  cout << "\nQUICKSORT SINGLE THREADED\n";
  cout << "Loading random number text files..\n";

  //Allocate some nice big vectors and load 100K randoms into each
  for (int i = 0; i < listsCount; i++)
  {
    randLists[i] = (vector<int>(listLength,0));  // create 100,000 integer array

  
    loadRandsFile(randLists[i], "../randoms/100K_rands_" + to_string(i) + ".txt");


  }

 // getNumberLines("../randoms/100K_rands_" + to_striz + ".txt");

  cout << "\nNow sorting " << listsCount << " lists of " << listLength << " random integers..\n";
  // Create a precision timer and start timing
  dmac::Timer timer;
  timer.start();
  // Sorts our lists
  std::vector<std::thread> threads;
  for (int i = 0; i < listsCount; i++)
  {
      // look more into creating threads inside a loop and joining/mutex etc
      quicksort(randLists[i], 0, randLists[i].size() - 1);
      
  }

  timer.stop();


  // OK to destroy now
  threads.clear();

  cout << "\n---------------------------------------\n";
  cout << "Lists sorted! \n";
  cout << "Total sort time: " << timer.timeTakenMilli() << " milliseconds \n";
  cout << "---------------------------------------\n";
  // writeRandsFile(randLists[0], "rands0_out.txt");
}

void getNumberLines(string filename)
{
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile("textexample.txt");

    while (std::getline(myfile, line))
        ++number_of_lines;
    std::cout << "Number of lines in text file: " << number_of_lines;
}

int loadRandsFile(vector<int>& destination, string fileName)
{
  ifstream randsFile;
  string line;
  int lineCount = 0;
  randsFile.open(fileName, ios::in);
  while (getline(randsFile, line))
  {
    destination[lineCount] = std::stoi(line);
    lineCount += 1;
  }
  randsFile.close();
  return lineCount;
}

bool writeRandsFile(vector<int>& randsArray, string fileName)
{
  ofstream randsFile;
  randsFile.open(fileName, ios::out);
  // Take each int from randsArray and dump it in randsFile
  for (int rand : randsArray)
  {
    randsFile << rand << endl;
  }
  randsFile.close();
  return true;
}


void quicksort(vector<int>& numbers, int start, int end)
{
  // If our left and right indicies haven't finally collided 
  int pivotIndex = 0;
  if (start < end)
  {
    // Picks a number in (part of the) array and moves all 
    // smaller numbers left, all larger numbers right. 
    pivotIndex = partition(numbers, start, end);
    // Recursively call self for left and right sides until array is sorted.
    lockMe.lock();
    quicksort(numbers, start, pivotIndex - 1);
    lockMe.unlock();
    quicksort(numbers, pivotIndex + 1, end);
  }
}

int partition(vector<int>& numbers, int left, int right)
{
  // Choose the left most number as the pivot.
  int pivotValue = numbers[left];
  int leftWall = left;  // Set up a wall always at latest lower-than-pivot-value index
  for (int i = leftWall + 1; i <= right; i++)
  {
    if (numbers[i] < pivotValue)
    {
      // Swap the small number with the large one next to the wall 
      swap(numbers[i], numbers[leftWall + 1]);
      leftWall += 1;	// Move the wall to gobble up the nice low value.
    }
  }
  // Swap original pivot (first item in list) with small number at wall
  swap(numbers[leftWall], numbers[left]);
  return leftWall;
}

void swap(int& left, int& right)
{
  int temp = left;
  left = right;
  right = temp;
}
