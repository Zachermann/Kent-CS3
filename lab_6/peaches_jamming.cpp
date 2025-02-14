#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iterator>

using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector; using std::deque;

struct Peaches {
    double weight; // oz
    bool ripe;     // ripe or not
    void print() const { cout << (ripe ? "ripe" : "green") << ", " <<  weight << endl; }
};

bool less_than(const Peaches& a, const double& maxWeight) {
    return a.weight < maxWeight;
};

int main() {
    srand(time(nullptr));
    const double minWeight = 3.;
    const double maxWeight = 8.;

    cout << "Input basket size: ";
    int size;
    cin >> size;

    vector <Peaches> basket(size);

    // assign random weight and ripeness peaches in the basket
    //replace with generate()
    std::generate(basket.begin(), basket.end(), [=]() {
        Peaches p;
        p.weight = minWeight + static_cast<double>(rand()) / RAND_MAX * (maxWeight - minWeight);
        p.ripe = rand() % 2 == 1;
        return p;
    });

    // Use for_each() to print all peaches in the basket
    cout << "all peaches" << endl;
    std::for_each(basket.begin(), basket.end(), [](const Peaches& e) {
        e.print();
    });

    // Moving all the ripe peaches from basket to peck
     // remove_copy_if() with back_inserter()/front_inserter() or equivalents
    deque<Peaches> peck;
    std::remove_copy_if(basket.begin(), basket.end(), std::back_inserter(peck), [](const Peaches& p) { return !p.ripe; });
    basket.erase(std::remove_if(basket.begin(), basket.end(), [](const Peaches& p) { return p.ripe; }), basket.end());

    // Use for_each() to print peaches in the basket
    cout << "peaches in the basket" << endl;
    std::for_each(basket.begin(), basket.end(), [](const Peaches& b) {
        b.print();
    });
    cout << endl;

    // Use for_each() to print peaches moved to the peck
    cout << "peaches moved to the peck" << endl;
    std::for_each(peck.begin(), peck.end(), [](const Peaches& p) {
        p.print();
    });

// prints every "space" peach in the peck
const int space = 3;
cout << "\nevery " << space << "'d peach in the peck" << endl;

auto it = peck.cbegin();

// Start at the 3rd peach, so advance the iterator subtracting 1 from space
it = std::next(it, space - 1); 

while (it != peck.cend()) {
    it->print();  // Print the current peach
    
    // Move the iterator ahead by the space
    if (std::distance(it, peck.cend()) <= space) {
        break;  // breaks loop if it excedes the boundery
    }
    it = std::next(it, space);
}




    

    // Putting all small ripe peaches in a jam
    // use a binder to create a functor with configurable max weight
    // accumulate() or count_if() then remove_if()
    const double weightToJam = 10.0;
    double jamWeight = 0;
    auto binder = std::bind(less_than, std::placeholders::_1, weightToJam);

    jamWeight = std::accumulate(basket.begin(), basket.end(), 0.0, [&](double acc, const Peaches& p) {
        if (binder(p)) {
            acc += p.weight;
            return acc;
        }
        else {
            return acc;
        }
    });

    basket.erase(std::remove_if(basket.begin(), basket.end(), binder), basket.end());

    cout << "Weight of jam is: " << jamWeight << endl;

    return 0;
}
