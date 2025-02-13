#include <stdio.h>
#include <string>
#include <iostream>
using std::string;

class WordOccurrence {
public:
    WordOccurrence(const string& word="", int num=0);
    bool matchWord(const string &); // returns true if word matches stored
    void increment(); // increments number of occurrences
    string getWord() const; 
    int getNum() const;

private:
    string word_;
    int num_;
};

class WordList{
public:
    // add copy constructor, destructor, overloaded assignment
     WordList() {size_ = 0; wordArray_ = new WordOccurrence[size_];};
    ~WordList() {delete [] wordArray_;};
    WordList(const WordList&);
    WordList& operator=(WordList);


    // implement comparison as friend
    friend bool equal(const WordList&, const WordList&);

    void addWord(const string &);
    void print();

    // Only use for testing purposes
    WordOccurrence * getWords() const {return wordArray_;};
    int getSize() const {return size_;};
private:
    WordOccurrence *wordArray_; // a dynamically allocated array of WordOccurrences
                                // may or may not be sorted
    int size_;
};
