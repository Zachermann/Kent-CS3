#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>
#include <set> // Include set

using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move;
using std::set; // Use std::set for storing students

// Define a struct to represent a student
struct Student {
    string firstName;
    string lastName;

    // Define operator< to compare students
    bool operator<(const Student& other) const {
        // Compare full names
        return (firstName + " " + lastName) < (other.firstName + " " + other.lastName);
    }
};

// Reading a list from a fileName
void readRoster(set<Student>& roster, string fileName);  

// Printing a set out
void printRoster(const set<Student>& roster); 

int main(int argc, char* argv[]){

   if (argc <= 1){ 
      cout << "usage: " << argv[0] 
      << " list of courses, dropouts last" 
      << endl;
      exit(1);
   }

   // Vector of sets of students in each course
   vector<set<Student>> courseStudents; 

   for(int i=1; i < argc-1; ++i){
      set<Student> roster;
      readRoster(roster, argv[i]);  
      courseStudents.push_back(move(roster));
   }

  
   // Reading in dropouts
   set<Student> dropouts; 
   readRoster(dropouts, argv[argc-1]); 

   // Set of all enrolled students
   set<Student> enrolledStudents;  

   // Populate set of enrolled students
   for(auto& course : courseStudents) {
      for(const auto& student : course) {
         // Add the student to enrolledStudents if not a dropout
         if (dropouts.find(student) == dropouts.end()) {
            enrolledStudents.insert(student);
         }
      }
   }

   cout << "\n\nCurrently Enrolled Students:\n"; 
   printRoster(enrolledStudents);
   
}

// Reading in a file of names into a set of students
void readRoster(set<Student>& roster, string fileName){
   ifstream course(fileName);
   string first, last;
   while(course >> first >> last) {
      roster.insert({first, last}); // Insert each student into the set
   }
   course.close();
}

// Printing a set out
void printRoster(const set<Student>& roster){
   for(const auto& student : roster) {
      cout << student.firstName << " " << student.lastName << endl;
   }
}


