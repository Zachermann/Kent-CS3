#include <iostream>
#include <cstdlib>
#include <ctime>   

using std::cout;
using std::endl;
using std::cin;

// abstract body 
class Fill {
public:
    Fill(char borderChar, char internalChar): borderChar_(borderChar), internalChar_(internalChar) {}
    virtual char getBorder() = 0;
    virtual char getInternal() = 0;
    virtual ~Fill() {}

protected:
    char borderChar_;
    char internalChar_;
};

// concrete body for Hollow paint
class Hollow : public Fill {
public:
    Hollow(char borderChar): Fill(borderChar, ' ') {}
    char getBorder() override { return borderChar_; }
    char getInternal() override { return internalChar_; }
    ~Hollow() override {}
};

// concrete body for Filled paint
class Filled : public Fill {
public:
    Filled(char fillChar): Fill(fillChar, fillChar) {}
    char getBorder() override { return borderChar_; }
    char getInternal() override { return internalChar_; }
    ~Filled() override {}
};

// concrete body for FullyFilled paint
class FullyFilled : public Filled {
public:
    FullyFilled(char borderChar, char internalChar): Filled(borderChar) {
        internalChar_ = internalChar;
    }
    char getBorder() override { return borderChar_; }
    char getInternal() override { return internalChar_; }
    ~FullyFilled() override {}
};

// concrete body for RandomFilled paint
class RandomFilled : public Fill {
public:
    RandomFilled(char char1, char char2): Fill(char1, char2) {
        srand(static_cast<unsigned int>(time(0))); //random generator
    }

    char getBorder() override {
        // Randomly decide between borderChar_ and internalChar_
        return (rand() % 2) == 0 ? borderChar_ : internalChar_;
    }

    char getInternal() override {
        // Randomly decide between borderChar_ and internalChar_
        return (rand() % 2) == 0 ? borderChar_ : internalChar_;
    }

    ~RandomFilled() override {}
};

// abstract handle
class Figure {
public:
    Figure(int size, Fill* fill) : size_(size), fill_(fill) {}
    virtual void draw() = 0;
    virtual ~Figure() {}

protected:
    int size_;
    Fill* fill_;
};

// concrete handle for Square
class Square : public Figure {
public:
    Square(int size, Fill* fill) : Figure(size, fill) {}
    void draw() override;
};

void Square::draw() {
    for (int i = 0; i < size_; ++i) {
        for (int j = 0; j < size_; ++j)
            if (i == 0 || j == 0 || i == size_ - 1 || j == size_ - 1)
                cout << fill_->getBorder();
            else
                cout << fill_->getInternal();
        cout << endl;
    }
}

int main() {
    // Demonstration of all four paint classes
    Fill* hollowPaint = new Hollow('&');
    Fill* filledPaint = new Filled('@');
    Fill* fullyFilledPaint = new FullyFilled('#', '*');
    Fill* randomFilledPaint = new RandomFilled('$', '%');

    Figure* square1 = new Square(6, hollowPaint);
    Figure* square2 = new Square(6, filledPaint);
    Figure* square3 = new Square(6, fullyFilledPaint);
    Figure* square4 = new Square(6, randomFilledPaint);

    cout << "Hollow Paint:" << endl;
    square1->draw();
    cout << endl;

    cout << "Filled Paint:" << endl;
    square2->draw();
    cout << endl;

    cout << "Fully Filled Paint:" << endl;
    square3->draw();
    cout << endl;

    cout << "Random Filled Paint:" << endl;
    square4->draw();
    cout << endl;
    return 0;
}
