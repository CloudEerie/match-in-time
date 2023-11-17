#include <iostream>
#include <windows.h>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <thread>
using namespace std;

struct Card {
    string backNumber;
    char frontLetter;
    bool isActive;
};

vector<Card> disp[4][4];
unsigned int userScore = 0;

void timer(unsigned int secInput) {
    unsigned int timer = secInput;
    unsigned int digitOutput = log10(timer) + 1;
    while (timer >= 1) {
        cout << setw(digitOutput) << timer << flush; // Preventing format error when timer goes down a digit
        Sleep(1000); // = 1 second, iterated by timer
        cout << '\r';
        timer--;
    }
    cout << endl;
}

void printTimePlaceholder() {
    int x; // Placeholder for testing timer()
    while (true) {
        cout << "Enter number of seconds: ";
        cin >> x;
        if (cin.fail()) {
            cout << "\nThat\'s enough." << endl;
            break;
        }
        timer(x);
    }
}

void addScore(unsigned int& currentScore, unsigned int& currentTime) {
    double timeBonus = (1.0/static_cast<double>(currentTime));
    currentScore += timeBonus;
}

void inputGuess(vector<Card> disp[4][4]) {
    string strInput1, strInput2;
    int guess1, guess2;

    cout << "Enter the numbers of two cards to flip them. If the letters match, you win points!" << endl;

    cout << "Enter the first card's number: ";
    cin >> strInput1;
    guess1 = stoi(strInput1);

    cout << "Enter the second card's number: ";
    cin >> strInput2;
    guess2 = stoi(strInput2);

    if (cin.fail() || guess1 < 1 || guess1 > 16 || guess2 < 1 || guess2 > 16) {
        cout << "\nThe cards are only numbered from 1-16!" << endl;
        inputGuess(disp);
    }
    else {
        int i1 = (guess1 - 1) / 4;
        int j1 = (guess1 - 1) % 4; // Trying to prevent out-of-bounds for disp
        int i2 = (guess2 - 1) / 4;
        int j2 = (guess2 - 1) % 4;

        if ((disp[i1][j1]).frontLetter == (disp[i2][j2]).frontLetter) { // Check for match
            if (!((disp[i1][i2]).isActive) || !((disp[i2][j2]).isActive)) {
                cout << "\nYou\'ve already used that card!" << endl;
                inputGuess(disp);
            }
            cout << "\nCongratulations! The letters match." << endl;
            (disp[i1][j1]).isActive = false;
            (disp[i2][j2]).isActive = false;
            addScore(userScore, 1);
        }
        else {
            cout << "\nSorry, the letters do not match." << endl;
            inputGuess(disp);
        }
    }
}

int main() {
    vector<char> alph(26);
    vector<char> listOfLetters;
    int k = 0;
    char random, letter;
    char letters[4][4];


    //backside of cards
    string allBackNums[16] = {"01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16"};
    int counter = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            (disp[i][j]).backNumber = allBackNums[counter];
            counter++;
        }
    }
    counter = 0;

    //alphabet
    for (int i = 0; i < 26; i++) {
        alph.at(i) = 'A' + i;
    }
    //getting letters for cards
    srand(time(0));
    for (int i = 0; i < 8; i++) {
        random = (rand() % alph.size());
        letter = alph.at(random);
        //adding to list in pairs
        listOfLetters.push_back(letter);
        listOfLetters.push_back(letter);
        alph.erase(alph.begin() + random);
    }

    //shuffling list so it can be added to array "letters" in random order
    random_shuffle(listOfLetters.begin(), listOfLetters.end());

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            (disp[i][j]).frontLetter = listOfLetters.at(k);
            k++;
        }
    }

    cout << "Thanks for playing our matching game!\nHow to play:\n";
    cout << "Enter a number to see what letter lies behind it. Try and match as many letters as you can!\n";

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << (disp[i][j]).backNumber << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << (disp[i][j]).frontLetter << " ";
        }
        cout << endl;
    }

    //FIXME: need to add in code to align number with index of array disp
    //FIXME: need to add in code that prints disp array with the user-entered numbers being replaced by the corresponding letter
    //FIXME: need to figure out how to clear out output screen so when user enters a new number, screen is not busy and is up to date

    return 0;
}
