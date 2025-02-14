#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include <memory> // for std::shared_ptr
#include <map>

// for timed sleep
#include <thread>
#include <chrono>


using std::cout; using std::cin; using std::endl;
using std::string;

// Abstract flyweight
class Logo {
public:
    virtual string getLogo() const = 0;
};

// Concrete flyweight
class AirlineLogo : public Logo {
public:
    explicit AirlineLogo(const string& airlineName) : airlineName_(airlineName) {}
    string getLogo() const override {
        // Convert airline name to uppercase for logo
        string logo = airlineName_;
        std::transform(logo.begin(), logo.end(), logo.begin(), ::toupper);
        return logo;
    }
private:
    string airlineName_;
};

// Flyweight factory
class LogoFactory {
public:
    static std::shared_ptr<Logo> getLogo(const string& airlineName) {
        auto it = logos_.find(airlineName);
        if (it == logos_.end()) {
            // If logo doesn't exist, create and store it
            logos_[airlineName] = std::make_shared<AirlineLogo>(airlineName);
            return logos_[airlineName];
        }
        return it->second;
    }
private:
    static std::map<string, std::shared_ptr<Logo>> logos_;
};

std::map<string, std::shared_ptr<Logo>> LogoFactory::logos_;

// Abstract mediator
class Controller {
public:
    virtual void join(class Flight*) = 0;
    virtual void leave(class Flight*) = 0;
    virtual void broadcast() = 0;
    virtual void observe() = 0;
};

// Concrete mediator
// Tower, runway clearance
class Tower : public Controller {
public:
    void join(class Flight*) override;
    void leave(class Flight*) override;
    void broadcast() override;
    void observe() override;
private:
    std::set<class Flight*> waiting_;
};

// Abstract colleague
class Flight {
public:
    Flight(Controller *controller, const string& airlineName)
        : controller_(controller), status_(Status::waiting),
          logo_(LogoFactory::getLogo(airlineName)) {
        task_ = rand() % 2 ? Task::taxiing : Task::approaching;
        controller_->join(this);
    }
    void receive(const string &msg){
        if (msg.find(flightNo_) != string::npos || msg.find("all") != string::npos){
            if(msg.find("clear") != string::npos){
                cout << flightNo_ << " roger that, ";
                cout << (task_ == Task::taxiing ? "taking off" : "landing") << endl;
                cout << "Logo: " << logo_->getLogo() << endl; // Print logo after clearance
                status_ = Status::cleared;
            } else if(msg.find("status") != string::npos) {
                cout << flightNo_
                    << (status_ == Status::waiting ? " waiting to " : " cleared to ")
                    << (task_ == Task::taxiing ? "take off" : "land") << endl;
            } else
                cout << "Tower, this is " << flightNo_ << " please repeat." << endl;
        }
    }
    bool isCleared() const {return status_ == Status::cleared;}
    void proceed(){
        std::this_thread::sleep_for(std::chrono::seconds(rand()%3+1)); // wait a while    
        cout << flightNo_
            << (task_ == Task::taxiing ? " took off" : " landed")
            << ". Have a good day!" << endl;
        controller_->leave(this);
    }

protected:
    string flightNo_;
    Controller *controller_;
    std::shared_ptr<Logo> logo_; // Flyweight for logo
    enum class Task {taxiing, approaching};  Task task_;
    enum class Status {waiting, cleared};    Status status_;
};

// Concrete colleagues
class Airbus : public Flight {
public:
    Airbus(Tower *tower, const string& airlineName) : Flight(tower, airlineName) {
        cout << flightNo_ << " requesting "
            << (task_ == Task::taxiing ? "takeoff" : "landing") << endl;
    }
};

class Boeing : public Flight {
public:
    Boeing(Tower *tower, const string& airlineName) : Flight(tower, airlineName) {
        cout << flightNo_ << " requesting "
            << (task_ == Task::taxiing ? "takeoff" : "landing") << endl;
    }
};


// Member functions for Tower
void Tower::broadcast() {
    cout << "Tower: ";
    string msg;
    getline(cin, msg);
    if (!msg.empty())
        for (auto f : waiting_) f->receive(msg);
}

void Tower::observe() {
    auto findCleared = [](Flight *f){return f->isCleared();};
    
    auto toProceed = std::find_if(waiting_.begin(),waiting_.end(), findCleared);
    
    while (toProceed != waiting_.end()){ // found a cleared flight
        (*toProceed) -> proceed();
        toProceed = std::find_if(waiting_.begin(), waiting_.end(), findCleared);
        if (toProceed != waiting_.end())
            cout << "MAYDAY! MAYDAY! MAYDAY! " << endl; // more than a single flight cleared
    }
}

void Tower::join(Flight *f) {
    waiting_.insert(f);
}

void Tower::leave(Flight *f) {
    waiting_.erase(f);
    delete f;
}

int main() {
    srand(time(nullptr));
    Tower jfk;

    new Boeing(&jfk, "Delta");
    new Airbus(&jfk, "AirFrance");
    new Boeing(&jfk, "United");
    new Airbus(&jfk, "KLM");

    while (true) {
        jfk.broadcast();
        jfk.observe();
        if (rand() % 2) {
            if (rand() % 2)
                new Boeing(&jfk, "JetBlue");
            else
                new Airbus(&jfk, "Lufthansa");
        }
    }
}
