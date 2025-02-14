#include <iostream>
#include <vector>
#include <string>
#include <stack>

using std::vector; using std::string;
using std::cout; using std::cin; using std::endl;

// receiver
class Document {
public:

  Document(const vector <string>
           & data={}): data_(data){}

  void insert(int line,
              const string &str) {
    int index = line-1;
    if (index <= data_.size())
       data_.insert
         (data_.begin() + index, str);
    else
      cout << "line out of range" <<
        endl;
  }

  string remove(int line) {
    int index = line-1;
    string deletedLine="";
    if(index < data_.size()){
      deletedLine = data_[index];
      data_.erase(data_.begin() + index);
    }else
      cout << "line out of range" << endl;
    return deletedLine;
  }


  void show() {
    for(int i = 0; i < data_.size(); ++i)
      cout << i + 1 << ". " <<
        data_[i] << endl;
  }

private:
  vector<string> data_;
};

// abstract command
class Command{
public:
  Command(Document *doc) : doc_(doc){}
  virtual void execute() = 0;
  virtual void unexecute() = 0;
  virtual ~Command(){}
protected:
  Document *doc_;
};

// two concrete commands
class InsertCommand : public Command {
public:
  InsertCommand(Document *doc, int line,
                const string &str):
    Command(doc), line_(line),
    str_(str) {}
  void execute() {
    doc_->insert(line_, str_);}
  void unexecute() {doc_->remove(line_);}

private:
  int line_;
  string str_;
};


class EraseCommand : public Command {
public:
  EraseCommand(Document *doc, int line):
    Command(doc), line_(line), str_("") {}
  void execute() {str_ = doc_->remove(line_);}
  void unexecute() {doc_-> insert(line_, str_);}
private:
  int line_;
  string str_;
};
// client
class DocumentWithHistory{
public:
  DocumentWithHistory(const vector<string> &
                      text={}) : doc_(text){}

  void insert(int line, string str) {
    Command *com = new InsertCommand(&doc_, line, str);
    com->execute();
    doneCommands_.push(com);
    string t0 = "insert "+str+" at line " + std::to_string(line);
    stringCommands_.push(t0);

  }

  void erase(int line){
    Command *com = new EraseCommand(&doc_, line);
    com->execute();
    doneCommands_.push(com); // pushing command here
    string t1 = "erase at line " + std::to_string(line);
    stringCommands_.push(t1);
  }

  void undo() {
    if(doneCommands_.size() != 0) {
      Command *com =
        doneCommands_.top();
      doneCommands_.pop();
      stringCommands_.pop();
      com->unexecute();
      delete com; // don't forget to delete command
    }else
      cout << "no commands to undo" << endl;
  }
  void redo(int line){
    std::stack<Command*>temp =  doneCommands_; // donecommands
    std::stack<string>  temps=  stringCommands_; // stringcommands
    if(doneCommands_.size() != 0){
      int i = 1;
      while(i < line){
        temp.pop();
        temps.pop();
        i = i + 1;
      }
      Command* com = temp.top();
      com->execute();
      doneCommands_.push(temp.top());
      stringCommands_.push(temps.top());
      while(!temp.empty()){
        temp.pop();
      }
      delete com;
    }
    else{
      cout << "no commands to redo" << endl;
    }
  }
  void history(){
    std::stack<string> temp = stringCommands_;
    int i = 1;
    if(!temp.empty()){
      while(!temp.empty()){
        cout << i <<". "<<temp.top() << endl;
        i = i + 1;
        temp.pop();
      }
      cout<<"\n"<<endl;
    }else{
      cout << "no commands in history" << endl;
    }
  }

  class Memento *createMemento();
  void rollBack(class Memento *);
  void show() {doc_.show();}

private:
  std::stack<Command*> doneCommands_;
  std::stack<string> stringCommands_;
  Document doc_;
};

class Memento {
public:
  Memento(const DocumentWithHistory& doc): doc_(doc) {}
  const DocumentWithHistory& getState() const {return doc_;}
private:
  const DocumentWithHistory doc_;
};


Memento *DocumentWithHistory::createMemento(){
  //neet to empty the command history
  while(!doneCommands_.empty()){
    doneCommands_.pop();
    stringCommands_.pop();
  }

  // copying the Document itself                                                            \

  return new Memento(*this);
}

void DocumentWithHistory::rollBack(Memento *mem){
  *this = mem->getState();  // copying back                                                 \

}

// invoker
int main() {
  DocumentWithHistory his({
      "Lorem Ipsum is simply dummy text of the printing and typesetting",
        "industry. Lorem Ipsum has been the industry's standard dummy text",
        "ever since the 1500s, when an unknown printer took a galley of",
        "type and scrambled it to make a type specimen book. It has",
        "survived five centuries."});

  int line; string str; char option;

  Memento *checkpoint = nullptr;
  do{
    his.show();
    cout << endl;

    cout << "Enter option (i)nsert line, (e)rase line, (u)ndo last command (c)heckpoint, roll(b)ack, (h)istory, (r)edo Command: ";
    cin >> option;

    switch(option) {
    case 'i':
      cout << "line number to insert: ";
      cin >> line;
      cout << "line to insert: ";
      cin.get();
      getline(cin, str);
      his.insert(line, str);
      break;

    case 'e':
      cout << "line number to remove: ";
      cin >> line;
      his.erase(line);
      break;

    case 'u':
      his.undo();
      break;

    case 'h':
      his.history();
      break;

    case 'r':
      cout << "Enter Command Line to exicute: ";
      cin>>line;
      his.redo(line);
      break;

    case 'c':
      checkpoint = his.createMemento();
      break;

    case 'b':
      if (checkpoint != nullptr){
        his.rollBack(checkpoint);
        delete checkpoint;
      }
      break;
    }
  }while(option =='b' || option == 'i' || option == 'e' || option == 'u' || option =='c' || option =='h' || option =='r');
}
