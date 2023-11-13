#include <iostream>
#include <windows.h>
#include <ctime>
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


	return 0;
}