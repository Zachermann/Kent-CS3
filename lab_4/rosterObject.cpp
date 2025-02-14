#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>

using std::ifstream;
using std::string;
using std::list;
using std::vector;
using std::cout;
using std::endl;
using std::move;

class Student {
public:
   Student(string firstName, string lastName) :
      firstName_(firstName), lastName_(lastName) {}

   // Move constructor (not strictly necessary, but shown for completeness)
   Student(Student && org) noexcept:
      firstName_(move(org.firstName_)),
      lastName_(move(org.lastName_)),
      courses_(move(org.courses_))
   {}

   // Default copy constructor
   Student(const Student & org) = default;

   // Add a course to the student's courses listed 
   void addCourse(const string& course) {
      courses_.push_back(course);
   }

   // Return the student's full name
   string getName() const {
      return firstName_ + " " + lastName_;
   }

   // Needed for for unique() and remove()
   friend bool operator== (const Student& left, const Student& right) {
      return left.lastName_ == right.lastName_ &&
             left.firstName_ == right.firstName_;
   }

   // Needed for sorting students
   friend bool operator< (const Student& left, const Student& right) {
      return left.lastName_ < right.lastName_ ||
             (left.lastName_ == right.lastName_ &&
              left.firstName_ < right.firstName_);
   }

   // Print the student's name followed by their courses
   void print() const {
      cout << getName() << ":";
      for (const auto& course : courses_) {
         cout << course << " ";
      }
      cout << endl;
   }

private:
   string firstName_;
   string lastName_;
   list<string> courses_; // List of courses 
};

// Reads a list and adds courses to students names
void readRoster(list<Student>& roster, const string& fileName);

// Prints the final list of students and their courses
void printRoster(const list<Student>& roster);

// Remove dropouts from the list of students
void removeDropouts(list<Student>& roster, const list<Student>& dropouts);

int main(int argc, char* argv[]) {

   if (argc <= 1) {
      cout << "usage: " << argv[0] << " list of courses, dropouts last" << endl;
      exit(1);
   }

   // List to store all students from the course files
   list<Student> allStudents;

   // Read each course roster and add students to the list
   for (int i = 1; i < argc - 1; ++i) {
      readRoster(allStudents, argv[i]);
   }

   // Read the list of dropouts
   list<Student> dropouts;
   readRoster(dropouts, argv[argc - 1]);

   // Remove dropouts from the student list
   removeDropouts(allStudents, dropouts);

   // Print the final list of students and their courses
   printRoster(allStudents);

   return 0;
}

//read a list of students and add courses
void readRoster(list<Student>& roster, const string& fileName) {
   ifstream courseFile(fileName);
   string first, last;
   string course = fileName.substr(0, fileName.find_last_of(".")); // Extract course name from file

   while (courseFile >> first >> last) {
      Student newStudent(first, last);

      // Check if student already exists in the roster
      bool found = false;
      for (auto& student : roster) {
         if (student == newStudent) {
            student.addCourse(course); // Add course to existing student's name
            found = true;
            break;
         }
      }

      //adds a new student if the name is not found 
      if (!found) {
         newStudent.addCourse(course);
         roster.push_back(newStudent);
      }
   }

   courseFile.close();
}

//remove students who are listed as dropouts
void removeDropouts(list<Student>& roster, const list<Student>& dropouts) {
   for (const auto& dropout : dropouts) {
      roster.remove(dropout);
   }
}

//print the final list of students and their courses
void printRoster(const list<Student>& roster) {
   for (const auto& student : roster) {
      student.print();
   }
}
