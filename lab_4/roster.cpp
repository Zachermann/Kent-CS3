#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>

using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::list;
using std::move;
using std::string;
using std::vector;

void readRoster(list<string> &roster, string fileName);
void readDropouts(list<string> &dropouts, string fileName);
void printRoster(const list<list<string>> &studentEntries);

int main(int argc, char *argv[]) {

   list<list<string>> studentEntries;
   list<string> dropouts; // stores the names of the dropouts

   // Checks and reads dropout file
   for (int i = 1; i < argc; ++i) {
      if (string(argv[i]) == "dropout.txt") {
         readDropouts(dropouts, argv[i]); 
      }
   }

   for (int i = 1; i < argc; ++i) {

      string fileName = argv[i];


      list<string> classRoster;
      readRoster(classRoster, fileName);

      string className = fileName;

      // Remove the .txt extension from the class name
      if (className.substr(className.size() - 4) == ".txt")
         className.erase(className.size() - 4);

      for (const string &studentName : classRoster) {
         bool isDropout = false;

         // Check if the student is in the dropout list
         for (const string &dropoutName : dropouts) {
            if (studentName == dropoutName) {
               isDropout = true;
               break;
            }
         }

         if (isDropout) {
            continue; // Skip students who are dropouts
         }

         bool studentAdded = false;
         for (list<string> &existingStudent : studentEntries) {
            if (studentName == *existingStudent.begin()) {
               existingStudent.push_back(className);
               studentAdded = true;
               break;
            }
         }

         if (!studentAdded) {
            list<string> studentList;
            studentList.push_front(studentName);
            studentList.push_back(className);
            studentEntries.push_back(move(studentList));
         }
      }
   }

   // Sort the students by their names
   studentEntries.sort();

   // Print roster
   printRoster(studentEntries);
   cout << endl;
}

// Reads the roster
void readRoster(list<string> &roster, string fileName) {
   ifstream course(fileName);
   string first, last;
   while (course >> first >> last)
      roster.push_back(first + ' ' + last);
   course.close();
}

// Reads the dropouts from dropout.txt
void readDropouts(list<string> &dropouts, string fileName) {
   ifstream dropoutFile(fileName);
   string first, last;
   while (dropoutFile >> first >> last) {
      dropouts.push_back(first + ' ' + last); 
   }
   
   dropoutFile.close();
}

// Prints the list of students with their enrolled courses
void printRoster(const list<list<string>> &studentEntries) {
   cout << "all students, dropouts removed and sorted\n";
   cout << "first name last name: courses enrolled\n";


   for (const list<string> &studentEntry : studentEntries) {
      auto it = studentEntry.begin();
      string studentName = *it;
      cout << studentName << ":";

      ++it;
      for (; it != studentEntry.end(); ++it) {
         cout << *it << ' ';
      }
      cout << endl;
   }
}
