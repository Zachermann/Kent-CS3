#include "coffeeShack.hpp"
#include <cstdlib>
#include <ctime>

int main() {
  srand(time(nullptr)); // Seed the random number generator with the current time for random selections.

  const int totalCustomers = 3; // Define the total number of customers to serve in this simulation.

  // Create a chain of responsibility: JuniorBarista -> SeniorBarista -> Manager.
  JuniorBarista *barist = new JuniorBarista(new SeniorBarista(new Manager));

  int customerNum = 0; // Track the number of customers who have placed orders.

  // Loop until all customers have placed their orders and all customers are served.
  do {
      int selection = rand() % 2; // Randomly decide whether to take a new order or serve an existing one.

      // Take a new order if the random selection is 0 and there are still customers to serve.
      if (selection == 0 && customerNum != totalCustomers) {
          // Begin making a drink.
          char choice;               // Placeholder for customer input.
          list<Drink*> drink;        // A list to store the drink and its toppings as decorators.

          barist->handleRequest(choice, drink); // Pass the drink request through the barista chain.

          // Ask the customer for their name.
          cout << "Can I get your name? ";
          string name; 
          cin >> name;
          cout << "please wait " << name << ".\n";

          // Create a new Customer object and register them as an observer.
          Customer *newCustomer = new Customer(name, barist, drink.back());
          ++customerNum; // Increment the count of customers who have placed an order.
      }
      // Finish an order if the random selection is 1 and there are customers waiting for their orders.
      else if (selection == 1 && !barist->allCustomersServed()) {
          barist->finishOrder(); // Serve the next customer in the queue.
      }
  } while (customerNum < totalCustomers || !barist->allCustomersServed());
  // Continue the loop until all orders are placed and all customers are served.

  return 0; // Exit the program when all customers are served.
}
