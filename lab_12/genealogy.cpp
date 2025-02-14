// traversing genealogical tree using Composite and Visitors
// Mikhail Nesterenko
// 10/28/2015

#include <iostream>
#include <string>
#include <vector>

using std::cout; using std::endl;
using std::string;
using std::vector;
using std::cin;

class Person{ // component
public:
  Person(string firstName,
         Person *spouse,
         Person *father,
         Person *mother):
    firstName_(firstName), spouse_(spouse),
    father_(father), mother_(mother){}
  string getFirstName(){return firstName_;}
  Person *getSpouse(){return spouse_;}
  void setSpouse(Person *spouse){spouse_=spouse;}
  Person *getFather(){return father_;}

  virtual void accept(class PersonVisitor *)=0;
  virtual ~Person(){}
private:
  string firstName_;
  Person *spouse_;
  Person *father_;
  Person *mother_;
};

// composites

// man has a last name
class Man: public Person{
public:
  Man(string lastName,
      string firstName,
      Person *spouse,
      Person *father, Person *mother):
    lastName_(lastName),
    Person(firstName, spouse, father, mother){}
  string getLastName(){return lastName_;}
  void accept(class PersonVisitor *visitor) override;
private:
  string lastName_;
};

// woman has a list of children
class Woman: public Person{
public:
  Woman(vector<Person *> children,
        string firstName,
        Person *spouse,
        Person *father, Person *mother):
    children_(children),
    Person(firstName, spouse, father, mother){}
  const vector<Person *> & getChildren()
  {return children_;}
  void setChildren(const vector<Person *> &children)
  {children_ = children;}
  void accept(class PersonVisitor *visitor) override;
private:
  vector<Person *> children_;
};

// abstract visitor
class PersonVisitor{
public:
  virtual void visit(Man*)=0;
  virtual void visit(Woman*)=0;
  virtual ~PersonVisitor(){}
};

// composite methods that define traversal
void Man::accept(PersonVisitor *visitor)
{visitor->visit(this);}

void Woman::accept(PersonVisitor *visitor){
  // children traversal through mother only
  // father's children are not traversed so as not
  // to traverse them twice: for mother and father
  visitor->visit(this);
  // traversing descendants
  for(auto child : children_)
    child->accept(visitor);
}

// concrete visitors
// the last name for a man is stored in object
// the last name a woman is determined by her
// spouse if she is married
// or by her father if she is not

class NamePrinter: public PersonVisitor{
public:
  void visit(Man *m) override {
    cout << m->getFirstName() << " "
         << m->getLastName() << endl;
  }
  void visit(Woman *w) override {
    cout << w->getFirstName() << " ";
    if (w->getSpouse() != nullptr)
      cout << static_cast<Man *>
        (w->getSpouse())->getLastName();
    else if (w->getFather() != nullptr)
      cout << static_cast<Man *>
        (w->getFather())->getLastName();
    else
      cout << "Doe";
    cout << endl;
  }
};


class ChildrenPrinter: public PersonVisitor{
public:
  void visit(Man *m) override {
    cout << m->getFirstName() << ": ";
      Woman *spouse = static_cast<Woman *>
        (m->getSpouse());
      if(spouse != nullptr)
        printNames(spouse->getChildren());
      cout << endl;
  }
  void visit(Woman *w) override {
    cout << w->getFirstName() << ":-";
    if(w->getSpouse() != nullptr){
      Man *spouse = static_cast<Man *>(w->getSpouse());
      cout << "Mans name = " << spouse->getFirstName() << endl;
      vector<Person *> children = w->getChildren();
      for(const auto c: children){
        cout << c->getFirstName() << ", ";
        if(c->getSpouse() != nullptr){
          cout <<"Husband's name = " <<c->getSpouse()->getFirstName() << endl;
        }
      }
    }
    //printNames(w->getChildren());
    //cout << endl;
  }
private:
  void printNames(const vector<Person *> &children){
    for(const auto c: children)
      cout << c->getFirstName() << ", ";
  }
};

class MarriageAdvice: public PersonVisitor{
public:

  //  string person1 = "James Smith";
  //string person2 = "Maryj";
  //string str;

  MarriageAdvice(string str1,string str2):str1_(str1),str2_(str2) {}
  void visit(Man *m) override {

    //    str = m->getFirstName()+" "+m->getLastName();
    //    cout << str; // james smith for now
    //    cout << str1_;
    //    cout << str2_;

    /*
    if(m->getSpouse() != nullptr){
    cout<< m->getFirstName() <<" " << m->getLastName()<<endl;
      Woman *spouse = static_cast<Woman *>(m->getSpouse());
      cout << spouse->getFirstName() << endl;
      for(const auto c: spouse->getChildren())
      cout << c->getFirstName() << endl;
  }


    if(person1 == person2){ //cannot marry himself/herself
    cout<<" Cannot marry ";
    cout<< person2 <<endl;
    }
    //person 1
    else if(m->getSpouse() != nullptr){ //cannot marry if first person is already married
    cout <<" Cannot marry ";
    cout<< person2 <<endl;
    }
    else if(!(static_cast<Woman *>(m->getSpouse())->getChildren()).empty()){ //checking for his children if his children exist
      Woman *spouse = static_cast<Woman *>(m->getSpouse());
      //cout << spouse->getFirstName() << endl;
      for(const auto c: spouse->getChildren()){
        if(person2 == c->getFirstName()){
	cout << " Cannot marry ";
	cout<< person2 <<endl;
          break;
        }
        else{ //  if the child does not exist then can marry
	cout << " Can marry ";
	cout << person2 <<endl;
          break;
        }
        break;
      }
    }
    else if(m->getFather()->getFirstName() == person2){// cannot marry father

    cout << " Cannot marry ";
    cout<< person2 <<endl;
    }
    */



  }

  void visit(Woman *w) override {
    //cout << w->getFirstName() << endl;
    if((w->getSpouse() != nullptr)){  // w is the first woman in the tree
      Man *spouse = static_cast<Man *>(w->getSpouse()); // spouse is the husband of the first woman
      if(str1_==str2_){  // checking if person is marrying him/her - self
        cout<<"They cannot get married because you cannot marry yourself."<<endl;
      }
      else if((static_cast<Man *>(w->getSpouse()) != nullptr) && (str1_ == w->getFirstName())){//if we are checking for MARY the first woman then
        cout<<"They cannot get married because "<< str1_ <<" is already married."<<endl; // if the woman is already married then she cannot marry again
      }
      else if((spouse->getSpouse() != nullptr) && (str1_==spouse->getFirstName())){ // if marry's husband is married and he is the first person
        cout <<"They cannot get married because "<<str1_ <<" is already married."<<endl; // if the guy is already married
      }
      else if((spouse->getSpouse() != nullptr) && (str2_==spouse->getFirstName())){ // if marry's husband is married and he is the first person

        cout <<"They cannot get married because "<<str2_ <<" is already married."<<endl;// if the guy is already married

      }
      else if((static_cast<Man *>(w->getSpouse()) != nullptr) && (str2_ == w->getFirstName())){//if we are checking for MARY the first woman then

        cout<<"They cannot get married because "<< str2_ <<" is already married"<<endl; // if the woman is already married then she cannot marry again

      }
      else{
        cout<<str1_<<" can marry "<<str2_<<endl;
      }

      //cout << spouse->getFirstName() <<" "<<spouse->getLastName()<<endl; // printing the 1st husband
      vector<Person *> children = w->getChildren();
      for(const auto c: children){ // going through all children
        //cout<< c->getFirstName() << endl; // printing all the child
        Man *spouse1 = static_cast<Man *>(c->getSpouse()); // spouse is the husband of the first woman
        //      if(str1_==str2_){
        //cout<<"They cannot get married because you cannot marry yourself."<<endl;
        //}
        //      cout<<"xxxxxxxx"<<c->getFirstName()<<endl;

	if((static_cast<Man *>(c->getSpouse()) != nullptr) && (str1_ == c->getFirstName())){//if we are checking for MARY the first woman then

          cout<<"They cannot get married because "<< str1_ <<" is already married."<<endl; // if the woman is already married then she cannot marry again

        }
        else if(str1_==str2_){
          cout<<"They cannot get married because you cannot marry yourself."<<endl;
        }
        else if((spouse1->getSpouse() != nullptr) && (str1_==spouse1->getFirstName())){ // if marry's husband is married and he is the first person

          cout <<"They cannot get married because "<<str1_ <<" is already married."<<endl; // if the guy is already married

        }
        else if((spouse1->getSpouse() != nullptr) && (str2_==spouse1->getFirstName())){ // if marry's husband is married and he is the first person

          cout <<"They cannot get married because "<<str2_ <<" is already married."<<endl;// if the guy is already married

        }
        else if((static_cast<Man *>(c->getSpouse()) != nullptr) && (str2_ == c->getFirstName())){//if we are checking for MARY the first woman then

          cout<<"They cannot get married because "<<str2_ <<" is already married"<<endl; // if the woman is already married then she cannot marry again

        }
        else if(c->getFather() != nullptr && str2_==c->getFather()->getFirstName() && str1_==c->getFirstName() ){//example robert is trying to marry his father so robers is str1 and str2 is james

  cout<<"They cannot get married because "<<str1_<<" is " << str2_<<"'s son."<<endl;
        }
        else{
          cout<<str1_<<" can marry "<<str2_<<endl;
        }
        break;
      }
      //printNames(w->getChildren());
      //cout << endl;
    }
  }
private:
  string str1_;
  string str2_;
};

class MaidenPrinter: public PersonVisitor{
public:
  void visit(Man *m) override {
    if(m->getSpouse() != nullptr){
      cout << static_cast<Woman *>
        (m->getSpouse())->getFirstName();
      cout << " " << m->getLastName() << endl;

    }

    Woman *spouse = static_cast<Woman *>
      (m->getSpouse());
    if(spouse != nullptr){
      printNames(spouse->getChildren());
      cout << " " << m->getLastName() << endl;
    }
  }

  void visit(Woman *w) override {
    cout << w->getFirstName() << " ";
    if (w->getFather() != nullptr){
      cout << static_cast<Man *>
        (w->getFather())->getLastName();
    }
    else
      cout << "Doe";
    cout << endl;
  }
private:
  void printNames(const vector<Person *> &children){
    for(const auto c: children)
      cout << c->getFirstName();
  }
};


int main(){
  // first generation
  Man *js = new Man("Smith", "James",
                    nullptr, nullptr, nullptr);

  Woman *ms = new Woman({}, "Mary", nullptr, nullptr, nullptr);  // no last name
  ms->setSpouse(js); // mary gets married to james smith
  js->setSpouse(ms); // james smith gets married to mary

  // second generation
  Woman *ps = new Woman({}, "Patricia",  // no last name
                        nullptr, js, ms); // parents
  Man *wj = new Man("Johnson", "William", //
                    nullptr, nullptr, nullptr);
  ps->setSpouse(wj); wj->setSpouse(ps);

  // mary's kids
  vector<Person *> marysKids  = {ps,  // patricia smith
                                 new Man("Smith", "Robert", nullptr, js, ms), // robert smith
                                 new Woman({}, "Linda", nullptr, js, ms)}; // linda smith
  ms->setChildren(marysKids);
  // third generation
  Man *mj = new Man("Johnson", "Michael", nullptr, wj, ps);
  vector<Person *> patsKids   = {mj,
                                 new Woman({}, "Barbara", nullptr, wj, ps)};
  ps->setChildren(patsKids);

  Woman *jj = new Woman({}, "Jennifer", nullptr, nullptr, nullptr);
  vector<Person *> jensKids = {new Woman({}, "Susan", nullptr, mj ,jj)};

  jj->setSpouse(mj); // marrying jennifer adn michael jhonson
  mj->setSpouse(jj); // marrying michael jhonson to jennifer
  jj->setChildren(jensKids); // giving jennifer a kid called susan


  // defining two visitors
  ChildrenPrinter *cp = new ChildrenPrinter;
  NamePrinter *np = new NamePrinter;
  MaidenPrinter *mp = new MaidenPrinter;

  // executing the traversal with the composite
  // and the specific visitor
  cout << "CHILDREN LIST\n";
  ms->accept(cp);

  cout << "\nNAME LIST\n";
  ms->accept(np);

  cout << "\nMaiden Name List\n";
  ms->accept(mp);

  cout << "Enter 2 names : " <<endl;
  string str1,str2;
  cin>>str1;
  cin>>str2;
  MarriageAdvice *ma = new MarriageAdvice(str1,str2);

  cout << "\nMarriage Advice\n" << endl;
  ms->accept(ma);
}
