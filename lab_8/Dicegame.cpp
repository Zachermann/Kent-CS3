#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>

using std::cout; using std::endl; using std::string; using std::cin;

// template for any game where players take 
// turns to make moves
// and there is a winner
class Game{
public:
   Game():playersCount_(0), movesCount_(0), playerWon_(noWinner){}

   // template method
   void playGame(const int playersCount = 0) {
      playersCount_ = playersCount;
      movesCount_=0;

      initializeGame();

      for(int i=0; !endOfGame(); i = (i+1) % playersCount_ ){
	     makeMove(i);
	     if (i==playersCount_-1) 
	        ++movesCount_; 
      }

      printWinner();
   }

   virtual ~Game(){}

protected:
   // primitive operations
   virtual void initializeGame() = 0;
   virtual void makeMove(int player) = 0;
   virtual void printWinner() = 0;
   virtual bool endOfGame() { return playerWon_ != noWinner;} // this is a hook
                   // returns true if winner is decided
   static const int noWinner=-1;

   int playersCount_;
   int movesCount_;
   int playerWon_;
};

class Dice : public Game {
public:
    // initialize players
    void initializeGame() {
        playersCount_ = 2;
    }

    void makeMove(int player) {
        string out;
        if (!pass[player]) {
            int roll = 0;
            if (movesCount_ != 3) {
                if (player == 0) { // if player is computer
                    out = "Computer rolled: ";

                    for (int i = 0; i < numDice; ++i) {
                        int die = rand() % 6 + 1; // dice roll
                        roll += die;

                        out += (std::to_string(die) + ' ');
                    }

                    if (max[player] < roll) {
                        max[player] = roll;
                    }

                    out += ("= " + std::to_string(roll) + ", computer's highest score = " + std::to_string(max[player]));
                    cout << out << endl;
                    int passed = rand() % 3; // 1/3 chance for computer to pass
                    if (passed == 0) {
                        pass[player] = true;
                    }


                }
                else { // players turn
                    out = "You rolled: ";

                    for (int i = 0; i < numDice; ++i) {
                        int die = rand() % 6 + 1; // dice roll
                        roll += die;

                        out += (std::to_string(die) + ' ');
                    }

                    if (max[player] < roll) {
                        max[player] = roll;
                    }

                    out += ("= " + std::to_string(roll) + ", your highest score = " + std::to_string(max[player]));
                    cout << out << endl;

                    string passed;
                    cout << "Roll again? [y/n]";
                    cin >> passed;
                    if (passed.compare("n") == 0) {
                        pass[player] = true;
                    }
                }
            }
        }
        else { // player has passed
            if (player == 0) {
                out = "Computer rolled: passed, computer's highest score = " + std::to_string(max[player]);
                cout << out << endl;
            }
            else {
                out = "You rolled: passed, your highest score = " + std::to_string(max[player]);
                cout << out << endl;
            }
        }

        

        if (movesCount_ == 3) {
            if (max[0] >= max[1]) {
                playerWon_ = 0;
            }
            else {
                playerWon_ = 1;
            }
        }
    }

    void printWinner() {
        if (playerWon_ == 0) {
            cout << "you lose" << endl;
        }
        else {
            cout << "you win" << endl;
        }
    }

private:
    int max[2] = {0, 0}; // array of player max scores

    bool pass[2] = { false, false }; // array of player status

    static const int numDice = 5;
    static const int numReroll = 3;
};

int main() {
   srand(time(nullptr));

   Game* gp = nullptr;
   gp = new Dice;
   gp->playGame();
   delete gp;
}
