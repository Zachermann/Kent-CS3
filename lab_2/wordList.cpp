#include"wordList.hpp"
#include<stdio.h>
#include<iostream>

WordOccurrence::WordOccurrence(const std::string& word, int num) {
    word_ = word;
    num_ = num;
}

bool WordOccurrence::matchWord(const std::string &word) {
    if(word_ == word)
        return true;
    else
        return false;
}

void WordOccurrence::increment() {
    num_ = ++num_;
} 

std::string WordOccurrence::getWord() const {
    return word_;
} 

int WordOccurrence::getNum() const {
    return num_;
}

WordList::WordList(const WordList& rightSide) {
    size_ = rightSide.size_;
    wordArray_ = new WordOccurrence[rightSide.size_];
    for(int i = 0; i < size_; ++i)
        wordArray_[i] = rightSide.wordArray_[i];
}

void WordList::addWord(const std::string& word) {
    for(int i = 0; i < size_; ++i) {
        if(wordArray_[i].matchWord(word)) {
            wordArray_[i].increment();
            return;
        }
    }

    WordOccurrence *tempArray = new WordOccurrence[size_+1];
    for (int i = 0; i < size_; ++i)
        tempArray[i] = wordArray_[i];
    delete[] wordArray_;
    wordArray_ = tempArray;

    




    int insertIndex = 0;
    for (int i = 0; i < size_; ++i) {
        if (wordArray_[i].getWord().compare(word) > 0) {  
            insertIndex = i;
            break; 
        }
        else 
            insertIndex = ++i; //////
  }
  
    for (int i = size_-1; insertIndex <= i; --i) {
        wordArray_[i + 1] = wordArray_[i];
    }
  
    wordArray_[insertIndex] = WordOccurrence(word, 1); 
    ++size_;
}






void WordList::print() {
    if (size_ == 0)  {
        std::cout << "nothing to print" << std::endl;
        return;
    }
  
    std::cout << "# \tWORD" << std::endl;
  
    int mostFrequent = 0;
  
    for (int i = 0; i < size_; ++i) {
        if (wordArray_[i].getNum() > mostFrequent) 
            mostFrequent = wordArray_[i].getNum();
    }

    while (mostFrequent > 0) {
        for (int i = 0; i < size_; ++i) {
            if (wordArray_[i].getNum() == mostFrequent)
                std::cout << wordArray_[i].getNum()<< "\t" << wordArray_[i].getWord() << std::endl;
        }
    --mostFrequent;
  }
}

WordList& WordList::operator=(WordList rightSide) {
  if (this == &rightSide)
    return *this;
  
  if (wordArray_ != nullptr)
    delete[] wordArray_;
  
  size_ = rightSide.size_;
  wordArray_ = new WordOccurrence[size_];
  
  for (int i = 0; i < size_; ++i) 
    wordArray_[i] = rightSide.wordArray_[i];
  
  return *this;
}

bool equal(const WordList& leftSide, const WordList& rightSide) {
    if (leftSide.size_ != rightSide.size_) return false;
  
    for (int i = 0; i < leftSide.size_; ++i) {
        if (leftSide.wordArray_[i].getWord() != rightSide.wordArray_[i].getWord() ||
            leftSide.wordArray_[i].getNum() != rightSide.wordArray_[i].getNum())
            return false;
        }
    return true;
}
