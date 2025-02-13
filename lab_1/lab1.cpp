#include <iostream>
#include <fstream>
#include <string.h>

using std::cout; using std::endl;

int main(int argc, char* argv[]){
 if(argc != 3){
  std::cout << "two file names are need compare" << std::endl;
 }

 std::ifstream file1;
 std::ifstream file2;

 file1.open(argv[1]);
 file2.open(argv[2]);

 int lineCount = 1;

 while(!(file1.eof() && file2.eof())) {
  std::string line1;
  std::string line2;

  if(file1.eof())
       line1 = " ";
  else
     getline(file1, line1);
  
  if(file2.eof())
     line2 = " ";
  else
     getline(file2, line2);


  if (line1 != line2) {
    std::cout << argv[1] << ":" << lineCount << ":" << line1 << std::endl;
    std::cout << argv[2] << ":" << lineCount << ":" << line2 << std::endl;

    int i = 0;
    while(line1[i] == line2[i])
    i++;

    std::string spacer(strlen(argv[1]) + std::to_string(lineCount).length() + 2 + i, ' ');
    std::cout << spacer << "^" << std::endl;
  }
  lineCount++;
 }
 file1.close();
 file2.close();

 return 0;
   
}
