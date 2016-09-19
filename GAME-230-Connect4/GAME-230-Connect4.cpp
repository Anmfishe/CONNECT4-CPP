// GAME-230-Connect4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;

void showBoard(char board[][7]) {
	//Double for loop to print out every element in the double array by row
	//Is there a more efficient way to do this?
	for (int i = 0; i < 7; i++ ) {
		for (int j = 0; j < 7; j++) {
			cout << board[j][i] << ' ';
		}
		cout << endl;
	}
}
bool checkVictory(int i, int j, char board[][7]) {
	char target = board[i][j];
	//First lets check if we have a horizontal win.
	//We will do these checks by incrementing a counter
	int horizCounter = 0;
	//Let's search the entire row for a string of 4 target characters
	for (int k = 0; k < 7; k++) {
		//If we found our character, increment one
		if (board[k][j] == target) horizCounter++;
		//If we found one of the other two characters, reset
		else horizCounter = 0;
		//If we reached 4, someone won!
		if (horizCounter == 4) return true;
	}
	//Let's do it again for verticle
	int vertCounter = 0;
	for (int k = 1; k < 7; k++) {
		//This time the column stays the same but the row changes
		if (board[i][k] == target) vertCounter++;
		else vertCounter = 0;
		if (vertCounter == 4) return true;
	}
	//Now for diagonals
	//First we have to find the lowest point on our potential diagonal win
	//For up-right we would do this
	int a, b;
	for (a = i, b = j; a > 0 && b < 7; a--, b++);
	int diagCounter1 = 0;
	for (; a < 7 && b > 0; a++, b--) {
		if (board[a][b] == target) diagCounter1++;
		else diagCounter1 = 0;
		if (diagCounter1 == 4) return true;
	}
	//up left is similar but we change the way we move down
	for (a = i, b = j; a < 7 && b < 7; a++, b++);
	int diagCounter2 = 0;
	//and change the way we move up
	for (; a >= 0  && b > 0; a--, b--) {
		if (board[a][b] == target) diagCounter2++;
		else diagCounter2 = 0;
		if (diagCounter2 == 4) return true;
	}
	return false;
}
int playMove(int player, int col, char board[][7]) {
	//What we have to do here is look for the first non '-' spot in the column and replace it with an x or o
	//Also to note, we have three cases of outcomes here
	//1. The move was successful and there was no victory
	//2. The move was successful and there was a winner
	//3. There was no more room in that column
	//Let's start at the bottom of the column and move up
	for (int i = 6; i > 0; i--) {
		//If we find '-'
		if (board[col][i] == '-') {
			//Replace it with the right character
			if (player == 1) board[col][i] = 'x';
			if (player == 2) board[col][i] = 'o';
			//Check for victory
			if(checkVictory(col, i, board)) return 2;
			//If not, return 1
			else return 1;
		}
	}
	//If we reached this point is means that we exited the for loop and that column was full
	return 3;
}


int main()
{
	//First of all, how many players are there?
	int numPlayers;
	cout << "Let's play connect 4!\n";
	system("pause");
	cout << "\nChoose number of players:\n1 or 2\n";
	//Let's make an infinite for loop waiting for good input
	while (true) {
		//Get the number of players
		cin >> numPlayers;
		//If the number of players is good, break out of this loop
		if (numPlayers == 1 || numPlayers == 2) {
			cout << "\nGreat, let's begin!\n";
			break;
		}
		//If not, clear cin and try again
		//Got these two lines of code from stack overflow
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "\nThat's not 1 or 2 silly, try again\n";
	}
	//Let's make the game board by making a double char array
	char board[7][7];
	//Fill the first row with 1-7
	for (int i = 0; i < 7; i++) {
		//the +49 is to change to the correct char ascii value
		board[i][0] = i + 49;
	}
	//Fill the rest with '-'
	for (int i = 0; i < 7; i++) {
		//Make sure to skip the first row which is filled with numbers
		for (int j = 1; j < 7; j++) {
			board[i][j] = '-';
		}
	}
	//Show the empty board then start the game
	cout << "\nHere is the board!\n";
	showBoard(board);
	//Start a loop depending on number of players
	if (numPlayers == 1) {
		while (true) {
			break;
		}
	}else {
		//The case for two players
		while (true) {
			//col will be the variable for the column they choose to play
			int col = 0;
			//Status will be the check for certain board conditions such as overflow or victory
			int status = 0;
			cout << "\nGo ahead player one\nChoose a row to play\n";
			//Start our good input loop again
			while (true) {
				//wait for some good input
				cin >> col;
				if (col >= 1 && col <= 7) {
					//Decrement col because we are working with arrays
					col--;
					//Play player one's move
					status = playMove(1, col, board);
					//Show the board
					showBoard(board);
					//Break out of our input loop
					if (status == 1 || status == 2) break;
					if (status == 3) cout << "\nThat column is full!\nTry again\n";
				}else {
					//If there was bad input...
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "\nEnter a value between 1 and 7. Try again player one.\n";
				}
			}
			if (status == 2) {
				cout << "\nPlayer one has achieved victory!\n";
				break;
			}
			//reset for player two
			col = 0;
			cout << "\nYour turn player two\nChoose a row to play\n";
			//Do it all again
			while (true) {
				//wait for some good input
				cin >> col;
				if (col >= 1 && col <= 7) {
					//Decrement col because we are working with arrays
					col--;
					//Play player one's move
					status = playMove(2, col, board);
					//Show the board
					showBoard(board);
					//Break out of our input loop
					if (status == 1 || status == 2) break;
					if (status == 3) cout << "\nThat column is full!\nTry again\n";
				}else {
					//If there was bad input...
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "\nEnter a value between 1 and 7. Try again player two.\n";
				}
			}
			if (status == 2) {
				cout << "\nPlayer two has achieved victory!\n";
				break;
			}
		}
	}

	system("PAUSE");
    return 0;
}

