// vector and list algorithms with objects in containers
// Mikhail Nesterenko
// 9/10/2018
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>
#include <map>


using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move;
using std::map;


class Student {
public:
    Student(string firstName, string lastName) :
        firstName_(firstName), lastName_(lastName) {}

    // move constructor, not really needed, generated automatically
    Student(Student&& org) :
        firstName_(move(org.firstName_)),
        lastName_(move(org.lastName_))
    {}

    // force generation of default copy constructor
    Student(const Student& org) = default;

    string print() const { return firstName_ + ' ' + lastName_; }

    // needed for unique() and for remove()
    friend bool operator== (Student left, Student right) {
        return left.lastName_ == right.lastName_ &&
            left.firstName_ == right.firstName_;
    }

    // needed for sort()
    friend bool operator< (Student left, Student right) {
        return left.lastName_ < right.lastName_ ||
            (left.lastName_ == right.lastName_ &&
                left.firstName_ < right.firstName_);
    }

private:
    string firstName_;
    string lastName_;
};




// reading a list from a fileName
void readRoster(map<Student, list<string>>& studentEntries, string fileName);
// printing a list out
void printRoster(const map<Student, list<string>>& studentEntries);
void readDropouts(map<Student, list<string>>& studentEntries, string fileName);

int main(int argc, char* argv[]){

   if (argc <= 1){ cout << "usage: " << argv[0] 
      << " list of courses, dropouts last" << endl; exit(1);}

   // Map of courses of students
   map<Student, list<string>> studentEntries;

   for(int i=1; i < argc-1; ++i){
      readRoster(studentEntries, argv[i]);
   }


   readDropouts(studentEntries, argv[argc - 1]);


   printRoster(studentEntries);
}


void readRoster(map<Student, list<string>>& studentEntries, string fileName){
   ifstream course(fileName);
   string first, last;

   string className = fileName.erase(fileName.length() - 4, 4);

   while (course >> first >> last) {
       Student temp(first, last);
       list<string> tempList;
       tempList.push_back(className);

       // inserts the new student into the map
       auto ret = studentEntries.insert(std::pair<Student, list<string>>(Student(first, last), tempList));
       
       // if the student already exists in the map then push back the class name
       if (!ret.second) {
           studentEntries[temp].push_back(className);
       }
   }
   course.close();
}

void readDropouts(map<Student, list<string>>& studentEntries, string fileName) {
    ifstream course(fileName);
    string first, last;

    while (course >> first >> last) {
        Student temp(first, last);

        // Erase the student from the map
        auto it = studentEntries.find(temp);
        if (it != studentEntries.end()) {
            studentEntries.erase(it);
        }
    }

    course.close();
}


// printing a list out
void printRoster(const map<Student, list<string>>& studentEntries){
    for (auto it = studentEntries.cbegin(); it != studentEntries.cend(); ++it) {
        string classList;
        
        for (const auto& className : it->second) {
            classList += (className + ' ');
        }

        cout << it->first.print() << ": " << classList << endl;
    }
}
