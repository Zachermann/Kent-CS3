// node class tempate used to contruct lists
// Mikhail Nesterenko
// 5/01/00

#ifndef LIST_HPP_
#define LIST_HPP_
#include <iostream>
#include <string>

using std::cout; using std::endl;
using std::string;
// the pointer part of every list is the same
// lists differ by data part
template <typename T>
class node{
public:

  node(): next_(nullptr) {}

  // functions can be inlined
  T getData() const {return data_;}
  void setData(const T& data){data_=data;}

  // or can be defined outside
  node<T>* getNext() const;
  void setNext(node<T> *const);

private:
  T data_;
  node<T> *next_;
};

//
// member functions for node
//
template <typename T>
node<T>* node<T>::getNext() const {
  return next_;
}

template<typename T>
void node<T>::setNext(node<T> *const next){
   next_=next;
}



template <typename T> class Collection;
template <typename T> bool equal(const Collection<T>&, const Collection<T>&);

template <typename T>
class Collection {
  public:
    Collection();
    void add(const T &item);
    void remove(const T &);
    void print();
    T last();

    friend bool equal<T>(const Collection<T> &, const Collection<T> &);

  private:
    node<T> *head;
};

template <typename T> Collection<T>::Collection() {
    head = nullptr;
}

template <typename T> void Collection<T>::add(const T &item) {
    node<T> *temp = new node<T>;
    temp->setData(item);
    temp->setNext(head);
    head = temp;
}

template <typename T>void Collection<T>::remove(const T& item) {
    node<T> *previous = nullptr;
    node<T> *current = head;
    node<T> *next;

    while (current != nullptr) {
        next = current->getNext();
        
        if (current->getData() == item) {
            if (previous == nullptr) {
                head = current->getNext();
            } else {
                previous->setNext(current->getNext());
            }
            delete current;
            current = next;
        } else {
            previous = current;
            current = next;
        }
    }
}

template <typename T> void Collection<T>::print() {
    node<T> *current = head;
    while (current != nullptr) {
        std::cout << current->getData() << " ";
        current = current->getNext();
    }
    
    std::cout << std::endl;
}

template <typename T> T Collection<T>::last() {
    return head->getData();
}

template <typename T> bool equal(const Collection<T> &lhs, const Collection<T> &rhs) {
    
    node<T> *left = lhs.head;
    node<T> *right = rhs.head;

    if (left == nullptr && right == nullptr)
        return true;
    else if (left == nullptr && right != nullptr)
        return false;
    else if (left != nullptr && right == nullptr)
        return false;
    else {
        while (left != nullptr && right != nullptr) {
            if (left->getData() != right->getData())
                return false;
            left = left->getNext();
            right = right->getNext();
        }

        if (left == nullptr && right != nullptr)
            return false;
        else if (left != nullptr && right == nullptr)
            return false;
    }

    return true;
}

#endif // LIST_HPP_

