#ifndef COFFEESHACK_H
#define COFFEESHACK_H

#include <string>
#include <iostream>
#include <vector>
#include <list>

using std::string; 
using std::cout; using std::cin; 
using std::vector;
using std::list;

// Enumeration for drink sizes
enum class DrinkType {small, medium, large};

// Base class representing a coffee drink
class Drink {
public:
   // Constructor initializes drink size and price
   Drink(DrinkType type = DrinkType::small, int price = 0)
       : type_(type), price_(price) {}
   
   // Returns the price of the drink
   virtual int getPrice() const { return price_; }
   
   // Returns the name of the drink with size
   virtual std::string getName() const;

private: 
   int price_; // Base price of the drink
   const DrinkType type_; // Size of the drink
   const string drinkName_ = "coffee with "; // Base drink name
};

// Returns the full name of the drink with its size
string Drink::getName() const {
    string typeName;
    if (type_ == DrinkType::large) typeName = "large";
    else if (type_ == DrinkType::medium) typeName = "medium";
    else typeName = "small";

    return typeName + " " + drinkName_;
}

// Decorators for adding toppings to drinks
class Sugar: public Drink {
public:
    // Constructor wraps an existing drink object
    Sugar(const Drink* wrapped) : wrapped_(wrapped) {}

    // Adds $1 to the price of the drink
    int getPrice() const override {
        return wrapped_->getPrice() + 1;
    }

    // Adds "Sugar" to the drink name
    std::string getName() const override { return wrapped_->getName() + " Sugar "; }
private:
    const Drink* wrapped_; // Wrapped drink object
};

class Cream: public Drink {
public:
    // Constructor wraps an existing drink object
    Cream(const Drink* wrapped): wrapped_(wrapped) {}

    // Adds $2 to the price of the drink
    int getPrice() const override {
        return wrapped_->getPrice() + 2;
    }

    // Adds "Cream" to the drink name
    std::string getName() const override { return wrapped_->getName() + " Cream "; }
private:
    const Drink* wrapped_; // Wrapped drink object
};

class Honey: public Drink {
public:
    // Constructor wraps an existing drink object
    Honey(const Drink* wrapped): wrapped_(wrapped) {}

    // Adds $3 to the price of the drink
    int getPrice() const override {
        return wrapped_->getPrice() + 3;
    }

    // Adds "Honey" to the drink name
    std::string getName() const override { return wrapped_->getName() + " Honey "; }
private:
    const Drink* wrapped_; // Wrapped drink object
};

// Forward declaration of Customer (Observer pattern)
class Customer; 

// Base class for baristas (Chain of Responsibility + Observer patterns)
class Barista {
public:
  // Constructor initializes the successor in the chain
  Barista(Barista* successor = nullptr) : successor_(successor) {}

  // Handles part of the request or passes it to the successor
  virtual Drink* handleRequest(char choice, list<Drink*>& drink) { 
    return successor_->handleRequest(choice, drink); 
  }

  // Observer pattern: Registers a customer for notification
  void registerObserver(Customer* c) { orders_.push_back(c); }
  
  // Removes a customer from the list of observers
  void deregisterObserver(int num) { orders_.erase(orders_.begin() + num); }
  
  // Notifies all registered customers
  void notifyObservers() const;

  // Completes an order and removes the served customer
  void finishOrder(); 

  // Checks if all customers have been served
  bool allCustomersServed() { return orders_.empty(); }

  // Helper to ask for toppings
  char askToppings();

protected:
  static vector<Customer*> orders_; // Shared list of customers
  int customerServed_; // Index of the customer being served

private:
  Barista* successor_; // Pointer to the next handler in the chain
};

vector<Customer*> Barista::orders_; // Static vector initialization

// Customer class representing a coffee shop customer (Observer pattern)
class Customer {
public:
  // Constructor links the customer to a Barista and registers them
  Customer(const string& name, Barista* subject, Drink* d) 
      : name_(name), subject_(subject), drink_(d) { 
        subject->registerObserver(this); 
    }

  // Called when the customer's order is ready
  void notify() const;

  // Accessors for the customer's name and drink
  string getCusName() const { return name_; }
  Drink* getDrink() const { return drink_; }

private:
  string name_; // Customer's name
  Drink* drink_; // Customer's drink
  Barista* subject_; // Barista serving the customer
};

// Derived barista classes for handling specific tasks
class JuniorBarista: public Barista {
public:
  JuniorBarista(Barista* successor = nullptr) : Barista(successor) {}

  // Handles drink size selection
  Drink* handleRequest(char choice, list<Drink*>& drink) override;
};

class SeniorBarista: public Barista {
public:
  SeniorBarista(Barista* successor = nullptr) : Barista(successor) {}

  // Handles basic topping additions
  Drink* handleRequest(char choice, list<Drink*>& drink) override;
};

class Manager: public Barista {
public:
  Manager(Barista* successor = nullptr) : Barista(successor) {}

  // Handles advanced topping additions
  Drink* handleRequest(char choice, list<Drink*>& drink) override;
};

// Notifies all registered customers of order readiness
void Barista::notifyObservers() const {
  for (auto e : orders_) e->notify();
}

// Marks an order as ready and notifies the customer
void Barista::finishOrder() {
  customerServed_ = rand() % orders_.size();
  Customer* customer = orders_[customerServed_];

  cout << customer->getCusName() << ", your " 
       << customer->getDrink()->getName() << " is ready. "
       << "It will be $" << customer->getDrink()->getPrice() << ", please.\n";

  customer->notify();
  this->deregisterObserver(customerServed_);
}

// Prompts the customer for additional toppings
char Barista::askToppings() {
  cout << "Would you like to add [s]ugar, [c]ream, [h]oney or [d]one? ";
  char selection; 
  cin >> selection;
  return selection;
}

//JuniorBarista implmentation
Drink* JuniorBarista::handleRequest(char choice, list<Drink*>& drink) {
  cout << "Welcome to Coffee Shack, can I get you [l]arge, [m]edium, or [s]mall coffee? ";
  cin >> choice;

  DrinkType type;
  int price;
  if      (choice == 'l') { type = DrinkType::large; price = 10; }
  else if (choice == 'm') { type = DrinkType::medium; price = 7; }
  else                    { type = DrinkType::small; price = 5; }

  drink.push_back(new Drink(type, price));
  choice = this->askToppings();

  if (choice == 'd') {
    cout << "Junior Barista handles request\n";
    return drink.back();
  }
  return Barista::handleRequest(choice, drink);
}

//SeniorBarista implemntation
Drink* SeniorBarista::handleRequest(char choice, list<Drink*>& drink) {
  while (choice != 'h') {
    cout << "Senior Barista handles request\n";
    if      (choice == 's') drink.push_back(new Sugar(drink.back()));
    else if (choice == 'c') drink.push_back(new Cream(drink.back()));
    else if (choice == 'd') return drink.back();

    choice = this->askToppings();
  }
  return Barista::handleRequest(choice, drink);
}

//Manager implmentation 
Drink* Manager::handleRequest(char choice, list<Drink*>& drink) {
  while (choice != 'd') {
    cout << "Manager handles request\n";
    if      (choice == 's') drink.push_back(new Sugar(drink.back()));
    else if (choice == 'c') drink.push_back(new Cream(drink.back()));
    else if (choice == 'h') drink.push_back(new Honey(drink.back()));

    choice = this->askToppings();
  }
  return drink.back();
}

//Customer notification
void Customer::notify() const {
  cout << "This is " << name_ << ", I got my coffee. Thank you!\n";
}

#endif // COFFEESHACK_H
