
#include <iostream>
#include <string>
#include <queue>
#include <list>
#include <cstdlib>
using std::cin; using std::cout; using std::endl; using std::string; using std::queue; using std::list;

class State;

// context
class Process {
public:
	Process();

	// behaviors
	void admitted();
	void dispatch();
	void interrupt();
	void wait();
	void exit();
	void comptletion();

	string report();

	Process* clone() { return new Process; }

	int getID() {
		return ID_;
	}

	// part of implementation of state pattern
	void changeState(State* s) { state_ = s; }
	virtual ~Process() { delete state_; };
private:
	State* state_;
	int ID_;
	static int nextID_;
};

// initalizing static variable
int Process::nextID_ = 0;

// abstract state
class State {
public:
	// provides default implementation
	virtual void admitted(Process*) {};
	virtual void dispatch(Process*) {};
	virtual void interrupt(Process*) {};
	virtual void wait(Process*) {};
	virtual void exit(Process*) {};
	virtual void completion(Process*) {};

	virtual string report() = 0;

	void changeState(Process* p, State* s) {
		p->changeState(s);
	}
};

// states
class New : public State {
public:
	static State* instance() {
		static State* onlyInstance = new New;
		return onlyInstance;
	}

	// valid transitions
	void admitted(Process*) override;

	// invalid transitions
	void dispatch(Process*) override { cout << "invalid transistion" << endl; }
	void interrupt(Process*) override { cout << "invalid transistion" << endl; }
	void wait(Process*) override { cout << "invalid transistion" << endl; }
	void exit(Process*) override { cout << "invalid transistion" << endl; }
	void completion(Process*) override { cout << "invalid transistion" << endl; }

	string report() override { return "new"; }
};

class Ready : public State {
public:
	static State* instance() {
		static State* onlyInstance = new Ready;
		return onlyInstance;
	}

	// valid transitions
	void dispatch(Process*) override;

	// invalid transitions
	void admitted(Process*) override { cout << "invalid transistion" << endl; }
	void interrupt(Process*) override { cout << "invalid transistion" << endl; }
	void wait(Process*) override { cout << "invalid transistion" << endl; }
	void exit(Process*) override { cout << "invalid transistion" << endl; }
	void completion(Process*) override { cout << "invalid transistion" << endl; }

	string report() override { return "ready"; }
};

class Running : public State {
public:
	static State* instance() {
		static State* onlyInstance = new Running;
		return onlyInstance;
	}

	// valid transitions
	void interrupt(Process*) override;
	void wait(Process*) override;
	void exit(Process*) override;

	// invalid transitions
	void admitted(Process*) override { cout << "invalid transistion" << endl; }
	void dispatch(Process*) override { cout << "invalid transistion" << endl; }
	void completion(Process*) override { cout << "invalid transistion" << endl; }

	string report() override { return "running"; }
};

class Waiting : public State {
public:
	static State* instance() {
		static State* onlyInstance = new Waiting;
		return onlyInstance;
	}

	// valid transitions
	void completion(Process*) override;

	// invalid transitions
	void admitted(Process*) override { cout << "invalid transistion" << endl; }
	void dispatch(Process*) override { cout << "invalid transistion" << endl; }
	void interrupt(Process*) override { cout << "invalid transistion" << endl; }
	void wait(Process*) override { cout << "invalid transistion" << endl; }
	void exit(Process*) override { cout << "invalid transistion" << endl; }

	string report() override { return "waiting"; }
};

class Terminated : public State {
public:
	static State* instance() {
		static State* onlyInstance = new Terminated;
		return onlyInstance;
	}

	// valid transitions
	// none

	// invalid transitions
	void admitted(Process*) override { cout << "invalid transistion" << endl; }
	void dispatch(Process*) override { cout << "invalid transistion" << endl; }
	void interrupt(Process*) override { cout << "invalid transistion" << endl; }
	void wait(Process*) override { cout << "invalid transistion" << endl; }
	void exit(Process*) override { cout << "invalid transistion" << endl; }
	void completion(Process*) override { cout << "invalid transistion" << endl; }

	string report() override { return "terminated"; }
};

// state transitions member functions
// New
void New::admitted(Process* p) {
	changeState(p, Ready::instance());
}

// Ready
void Ready::dispatch(Process* p) {
	changeState(p, Running::instance());
}

// Running
void Running::interrupt(Process* p) {
	changeState(p, Ready::instance());
}

void Running::wait(Process* p) {
	changeState(p, Waiting::instance());
}

void Running::exit(Process* p) {
	changeState(p, Terminated::instance());
}

// Waiting
void Waiting::completion(Process* p) {
	changeState(p, Ready::instance());
}

// Process member functions
Process::Process() {
	ID_ = nextID_;
	nextID_++;
	state_ = New::instance();
}

// handles/behaviors
void Process::admitted() { state_->admitted(this); }
void Process::dispatch() { state_->dispatch(this); }
void Process::interrupt() { state_->interrupt(this); }
void Process::wait() { state_->wait(this); }
void Process::exit() { state_->exit(this); }
void Process::comptletion() { state_->completion(this); }
string Process::report() { return state_->report(); }

int main(){
	queue<Process*> readyQ;
	queue<Process*> runningQ;
	queue<Process*> newQ;
	list<Process*> waiting;
	
	// prototype process
	Process* protoProcess = new Process;

	// creates new processes from prototype and pushes them into the new queue
	for (int i = 0; i < 4; ++i) {
		newQ.push(protoProcess->clone());
		cout << "process " << newQ.back()->getID() << " is " << newQ.back()->report() << endl;
	}

	cout << endl;

	// moving all processes from new queue to ready queue
	while (!newQ.empty()) {
		readyQ.push(newQ.front());
		newQ.pop();
		readyQ.back()->admitted();
		cout << "process " << readyQ.back()->getID() << " is " << readyQ.back()->report() << endl;
	}

	// runs while there's still a process that isnt terminated
	while (!readyQ.empty() || !runningQ.empty() || !waiting.empty()) {
		// only moves from ready to running if running is empty. Only one process running at a time
		if (runningQ.empty()) {
			runningQ.push(readyQ.front());
			readyQ.pop();
			runningQ.front()->dispatch();
			cout << "process " << runningQ.back()->getID() << " is " << runningQ.back()->report() << endl;
		}
		// randomly deciding how the run resulted
		int runResult = rand() % 3;
		// interrupt
		if (runResult == 0) {
			readyQ.push(runningQ.front());
			runningQ.pop();
			readyQ.back()->interrupt();
			cout << "process " << readyQ.back()->getID() << " is " << readyQ.back()->report() << endl;
		}
		// wait
		else if (runResult == 1) {
			waiting.push_back(runningQ.front());
			runningQ.pop();
			waiting.back()->wait();
			cout << "process " << waiting.back()->getID() << " is " << waiting.back()->report() << endl;
		}
		// exit
		else if (runResult == 2) {
			runningQ.front()->exit();
			cout << "process " << runningQ.front()->getID() << " is " << runningQ.front()->report() << endl;
			runningQ.pop();
		}

		if (!waiting.empty()) {
			// 50/50 chance of i/o completing
			int waitResult = rand() % 2;

			// complete
			if (waitResult == 0) {
				// first process in list is completed
				readyQ.push(waiting.front());
				waiting.pop_front();
				readyQ.back()->comptletion();
				cout << "process " << readyQ.back()->getID() << " is " << readyQ.back()->report() << endl;
			}
		}

	}
}
