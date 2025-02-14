// picking the best quality lemons
// converting vectors to priority queue
// Mikhail Nesterenko
// 2/17/2023


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <queue>

using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector;

struct Lemon {
    double quality;
    friend bool operator<(const Lemon& left, const Lemon& right) {
        return left.quality < right.quality;
    }
};

const int maxBoxSize = 30;
const int highestQuality = 10;

int main() {
    srand(time(nullptr));
    vector <Lemon> box(rand() % maxBoxSize + 1); // random box size

    // random quality assingment
    for (auto& lemon : box)
        lemon.quality = static_cast<double>(rand()) / RAND_MAX * highestQuality;

    cout << "Here are the lemons (best first): ";

    // replace this code with priority queue
    std::priority_queue<Lemon> pq;
    for (const auto& lemon : box){
        pq.push(lemon);
    }
    
    while (!pq.empty()) {
        cout << pq.top().quality << ", ";
        pq.pop();
    }
    cout << endl;
}
