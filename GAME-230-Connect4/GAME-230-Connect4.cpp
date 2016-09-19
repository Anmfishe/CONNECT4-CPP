// GAME-230-Connect4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;

void showBoard(char board[][7]) {
	for (int i = 0; i < 7; i++ ) {
		for (int j = 0; j < 7; j++) {
			cout << board[j][i] << ' ';
		}
		cout << endl;
	}
}

int main()
{
	int numPlayers;
	cout << "Let's play connect 4!\n";
	system("pause");
	cout << "\nChoose number of players:\n1 or 2\n";
	while (true) {
		cin >> numPlayers;
		if (numPlayers == 1 || numPlayers == 2) {
			cout << "\nGreat, let's begin!\n";
			break;
		}
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "\nThat's not 1 or 2 silly, try again\n";
	}
	char board[7][7];

	for (int i = 0; i < 7; i++) {
		board[i][0] = i + 49;
	}
	for (int i = 0; i < 7; i++) {
		for (int j = 1; j < 7; j++) {
			board[i][j] = '-';
		}
	}
	cout << "\nHere is the board!\n";
	showBoard(board);

	system("PAUSE");
    return 0;
}

