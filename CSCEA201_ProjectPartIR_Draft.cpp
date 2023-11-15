#include <iostream>
#include <windows.h>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

struct Card {
	char symbol;
	bool isActive;
};

void timer(unsigned int secInput) {
	unsigned int timer = secInput;
	while (timer >= 1) {
		cout << timer;
		Sleep(1000); // = 1 second, iterated by timer
		timer--;
		system("CLS"); // Need a way to clear a line/item and not entire screen
	}
}

void addScore(unsigned int& currentScore, unsigned int& currentTime) {
	double timeBonus = (1.0/static_cast<double>(currentTime));
	currentScore += timeBonus;
}

// FIXME: More systems needed!

int main() {
	int x; // FIXME: Placeholder for testing timer()
	while (true) {
		cout << "Enter number of seconds: ";
		cin >> x;
		if (cin.fail()) {
			cout << "\nThat\'s enough." << endl;
			break;
		}
		timer(x);
	}

    int i, j, k = 0;
    vector<char> alph(26);
    vector<char> listOfLetters;
    char random, letter;
    char letters[4][4];


    //backside of cards
    int disp[4][4] = { {1, 2, 3, 4},
                       {5, 6, 7, 8},
                       {9, 10, 11, 12},
                       {13, 14, 15, 16} };

    
    //alphabet
    for (i = 0; i < 26; i++) {
        alph.at(i) = 'A' + i;
    }
    //getting letters for cards
    for (i = 0; i < 8; i++) {
        srand(time(0));
        random = (rand() % 26);
        letter = alph.at(random);
        //adding to list in pairs
        listOfLetters.push_back(letter);
        listOfLetters.push_back(letter);
        alph.erase(alph.begin() + random);
    }

    //shuffling list so it can be added to array "letters" in random order
    random_shuffle(listOfLetters.begin(), listOfLetters.end());


    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            letters[i][j] = listOfLetters.at(k);
            k++;
        }
    }

    cout << "Thanks for playing our matching game!\nHow to play:\n";
    cout << "Enter a number to see what letter lies behind it. Try and match as many letters as you can!\n"
    
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            cout << disp[i][j] << " ";
        }
        cout << endl;
    }

    //FIXME: need to add in code to align number with index of array disp
    //FIXME: need to add in code that prints disp array with the user-entered numbers being replaced by the cooresponding letter
    //FIXME: need to include code for user errors: i.e. user enters a number that has already been used or does not exist
    //FIXME: need to figure out how to clear out output screen so when user enters a new number, screen is not busy and is up to date

}

	return 0;
}
