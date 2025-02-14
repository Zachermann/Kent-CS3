// sorting apples
// non STL-algorithm code to be replaced by algorthms
// Mikhail Nesterenko
// 09/26/2022


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>

using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector; using std::deque;

struct Apples{
   double weight; // oz
   string color;  // red or green
   void print() const { cout << color << ", " <<  weight << endl; }
};




int main(){
   srand(time(nullptr));
   const double minWeight = 3.;
   const double maxWeight = 8.;

   cout << "Input crate size: ";
   int size;
   cin >> size;

   vector <Apples> crate(size);

   // assign random weight and color to apples in the crate
   // replace with generate()
   std::generate(crate.begin(), crate.end(), [=]() {
      Apples a;
         a.weight = minWeight + static_cast<double>(rand()) /RAND_MAX*(maxWeight - minWeight);
         a.color = rand() % 2 == 1 ? "green" : "red"; 
		return a;});


    // for_each() possibly
   cout << "all apples" << endl;
   std::for_each(crate.begin(), crate.end(), [](const Apples& e) {
    e.print();
   });

   
   cout << "Enter weight to find: ";
   double toFind;
   cin >> toFind;

   // count_if()
   int cnt = std::count_if(crate.cbegin(), crate.cend(), [toFind](Apples a) {return a.weight > toFind;});

   cout << "There are " << cnt << " apples heavier than " 
	<< toFind << " oz" <<  endl;


   // find_if()
   cout << "at positions ";
   auto itr = std::find_if(crate.cbegin(), crate.cend(), [toFind](Apples a) {return a.weight > toFind;});
   while(itr != crate.cend()) {
      cout << itr - crate.cbegin() << ", ";
      itr = std::find_if((itr + 1), crate.cend(), [toFind](Apples a) {return a.weight > toFind;});
   }
   cout << endl;

   // max_element()
     auto heaviestApple = std::max_element(crate.begin(), crate.end(),[](const Apples& a, const Apples& b) {
            return a.weight < b.weight;
        });
    cout << "Heaviest apple weighs: " << heaviestApple->weight << " oz" << endl;


   // for_each() or accumulate()
   double sum;
    std::for_each(crate.cbegin(), crate.cend(), [&sum](Apples a) {sum += a.weight;});
   cout << "Total apple weight is: " << sum << " oz" << endl;


   // transform();
   cout << "How much should they grow: ";
   double toGrow;
   cin >> toGrow;
   std::transform(crate.begin(),crate.end(), crate.begin(), [toGrow](Apples a){Apples temp; temp.color = a.color;
                                                                                 temp.weight = a.weight + toGrow;
                                                                                 return temp;
                                                                                 });

   // remove_if()
   cout << "Input minimum acceptable weight: ";
   double minAccept;
   cin >> minAccept;
   crate.erase(std::remove_if(crate.begin(), crate.end(), [minAccept](Apples &a){return a.weight < minAccept;}), crate.end());

   // removing small apples
   // nested loops, replace with a single loop modification idiom
   crate.erase(std::remove_if(crate.begin(), crate.end(), [=](const Apples& a) {
      return a.weight < minAccept;
   }), crate.end());

// Output the number of removed elements
cout << "removed " << size - crate.size() << " elements" << endl;


   // bubble sort, replace with sort()
    std::sort(crate.begin(), crate.end(), [](Apples a, Apples b){
      return a.weight < b.weight;
      });


   // for_each() possibly
   cout << "sorted remaining apples" << endl;
    std::for_each(crate.begin(), crate.end(), [](const Apples& e) {
        e.print();
    });

    return 0;

}

