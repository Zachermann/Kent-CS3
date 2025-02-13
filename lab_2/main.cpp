#include "wordList.hpp"
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Requires more arguments" << std::endl;
        return 1;
    }

    std::string input = argv[1];

    std::ifstream inputStream(input);

    WordList list1;

    std::string word;
    while (inputStream >> word)
    {
        std::string fixedWord;
        for (int i = 0; i < word.size(); ++i)
        {
            char tempChar = word[i];
            if (0 <= tempChar && tempChar <= 127)
            {
                if (isalnum(tempChar))
                {
                    if (isupper(tempChar))
                        tempChar = char(tolower(tempChar));
                    fixedWord.append(sizeof(tempChar), tempChar);
                }
            }
        }
        list1.addWord(fixedWord);
    }
    list1.print();
    inputStream.close();
}
