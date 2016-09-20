// GAME-230-Connect4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;

void showBoard(char board[][7]) {
	//Double for loop to print out every element in the double array by row
	//Is there a more efficient way to do this?
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 8; j++) {
			cout << board[j][i] << ' ';
		}
		cout << endl;
	}
}

bool checkVictory(int i, int j, char board[][7]) {
	char target = board[i][j];
	if (target == '-') return false;
	//First lets check if we have a horizontal win.
	//We will do these checks by incrementing a counter
	int horizCounter = 0;
	//Let's search the entire row for a string of 4 target characters
	for (int k = 1; k < 8; k++) {
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
	for (; a < 8 && b > 0; a++, b--) {
		if (board[a][b] == target) diagCounter1++;
		else diagCounter1 = 0;
		if (diagCounter1 == 4) return true;
	}
	//up left is similar but we change the way we move down
	for (a = i, b = j; a < 8 && b < 7; a++, b++);
	int diagCounter2 = 0;
	//and change the way we move up
	for (; a > 0 && b > 0; a--, b--) {
		if (board[a][b] == target) diagCounter2++;
		else diagCounter2 = 0;
		if (diagCounter2 == 4) return true;
	}
	return false;
}
int removePiece(char board[][7], int player, int i, int j) {
	//Removing a piece is funky but we can do it
	int a, b;
	//Find the character the player is trying to remove
	char target = board[i][j];
	//And make sure the player is trying to remove only their character
	if (player == 2 && target != 'o') return 1;
	if (player == 1 && target != 'x') return 1;
	//If that's all good, then for that column, we need to set the value equal to the value above it
	//to create a stack falling effect
	for (a = i, b = j; b > 1; b--) {
		board[a][b] = board[a][b - 1];
	}
	//Then make sure the top of that column is a '-'
	board[a][1] = '-';
	//Since we shifted every piece in the column, we have to check if anyone has won
	//by checking every piece in that column
	for (int k = 6; k > 1; k--) {
		if (checkVictory(i, k, board)) {
			//If there is a winner, whose piece is it? Return a number accordingly
			if (board[i][k] == 'x') return 2;
			if (board[i][k] == 'o') return 3;
		}
	}
	//Otherwise, carryon and return 0
	return 0;
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
			if (checkVictory(col, i, board)) return 2;
			//If not, return 1
			else return 1;
		}
	}
	//If we reached this point is means that we exited the for loop and that column was full
	return 3;
}


int main()
{
	//First of all, will we have removal mode?
	char removalMode;
	bool rm = false;
	cout << "Let's play connect 4!\n";
	system("pause");
	cout << "\nWould you like to play removal mode?\ny or n\n";
	//Let's make an infinite for loop waiting for good input
	while (true) {
		//Get the removal mode
		cin >> removalMode;
		//If the input is good, break
		if (removalMode == 'y') {
			cout << "\nRemoval Mode on\nGreat, let's begin!\n";
			rm = true;
			break;
		}
		if (removalMode == 'n') {
			cout << "\nRemoval mode off\nGreat, let's begin!\n";
			rm = false;
			break;
		}
		//If not, clear cin and try again
		//Got these two lines of code from stack overflow
		//I will be using them a lot
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "\nEnter y or n to play with removal mode\n";
	}

	//Once we have the removal mode let's make the game board by making a double char array
	char board[8][7];
	//The upper left corner will be empty
	board[0][0] = ' ';
	//Fill the first row with 1-7
	for (int i = 1; i < 8; i++) {
		//the +49 is to change to the correct char ascii value
		board[i][0] = i + 48;
	}
	//Fill in first col with 1-6
	for (int i = 1; i < 7; i++) {
		board[0][i] = i + 48;
	}
	//Fill the rest with '-'
	for (int i = 1; i < 8; i++) {
		//Make sure to skip the first row and col which are filled with numbers
		for (int j = 1; j < 7; j++) {
			board[i][j] = '-';
		}
	}
	//Show the empty board then start the game
	cout << "\nHere is the board!\n";
	showBoard(board);
	//This first while loop will contain all of player one's turn
	while (true) {
		//col will be the variable for the column they choose to play
		char col = 0;
		//Status will be the check for certain board conditions such as overflow or victory
		int status1 = 0;
		int status2 = 0;
		cout << "\nGo ahead player one\nChoose a row to play\n";
		if (rm) cout << "Or hit r to remove a piece\n";
		//Start our good input loop again
		while (true) {
			//This bool signifies if the user put in good input
			//If they did, we will use success to break out of this turn's loop
			bool success = false;
			//Get the input
			cin >> col;
			//If it was between 1 and 7 then we can play that move
			if (col >= '1' && col <= '7') {
				//We subtract by 48 to account for ascii
				col -= 48;
				//Play player one's move
				status1 = playMove(1, col, board);
				//Show the board
				showBoard(board);
				//If the move was successful then break out of the loop. If not, 
				if (status1 == 1 || status1 == 2) success = true;
				//If the column was full, we do nothing and DONT set success to true so we will loop again
				if (status1 == 3) cout << "\nThat column is full!\nTry again\n";
			}
			//If the user entered r and we are in removal mode
			else if (col == 'r' && rm) {
				//start another input loop for a col and row to delete
				while (true) {
					cout << "\nPlayer one has chosen to remove a piece.\nPlease select the column of the piece\n";
					char rcol;
					//Get the col
					cin >> rcol;
					//If the col input was good, get the row
					if (rcol >= '1' && rcol <= '7') {
						cout << "\nNow choose a row\n";
						char rrow;
						cin >> rrow;
						//If the row was also good
						if (rrow <= '6' && rrow >= '1') {
							//account for ascii again
							rcol -= 48;
							rrow -= 48;
							//If we got both good data for row and col, run the function and get the result
							int result = removePiece(board, 1, rcol, rrow);
							//1 means the player tried to remove a piece they couldnt
							if (result == 1) cout << "\nYou can't remove that piece.\n";
							//2 or 3 means someone won so we set that person's status to 2
							//set success to true to break out of this turn
							//And break out of this loop
							if (result == 2) { status1 = 2; showBoard(board); success = true; cout << "\nPiece removed\n"; break; }
							if (result == 3) { status2 = 2; showBoard(board); success = true; cout << "\nPiece removed\n"; break; }
							//0 means it worked but no one won so don't set status to 2
							if (result == 0) { success = true; showBoard(board);  cout << "\nPiece removed\n";  break; }
						}
					}
					//If we didn't break earlier in this removal input loop, we will here after reprinting the board
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "\nTry again\n";
					showBoard(board);
					cout << "\nGo ahead player one\nChoose a row to play\n";
					if (rm) cout << "Or hit r to remove a piece\n";
					break;

				}
			}
			else {
				//If there was bad input at the source
				//We don't want to break here or it would end this turn
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "\nEnter a value between 1 and 7. Try again player one.\n";
				if (rm) cout << "Or hit r to remove a piece\n";
			}
			//If player one did something good, break to the next turn
			if (success) break;
		}
		//but first see if anyone won
		if (status1 == 2) {
			cout << "\nPlayer one has achieved victory!\n";
			break;
		}
		if (status2 == 2) {
			cout << "\nPlayer two has achieved victory!\n";
			break;
		}
		//reset for player two
		col = 0;
		cout << "\nYour turn player two\nChoose a row to play\n";
		if (rm) cout << "Or hit r to remove a piece\n";
		//Do what we just did again, but for player two this time
		while (true) {
			cin >> col;
			bool success = false;
			if (col >= '1' && col <= '7') {
				col -= 48;
				//Play player two's move
				status2 = playMove(2, col, board);
				showBoard(board);
				if (status2 == 1 || status2 == 2) success = true;
				if (status2 == 3) cout << "\nThat column is full!\nTry again\n";
			}
			else if (col == 'r' && rm) {
				//start another input loop for a col and row
				while (true) {
					cout << "\nPlayer one has chosen to remove a piece.\nPlease select the column of the piece\n";
					char rcol;
					cin >> rcol;
					//If rcol was good, get rrow
					if (rcol >= '1' && rcol <= '7') {
						cout << "\nNow choose a row\n";
						char rrow;
						cin >> rrow;
						if (rrow <= '6' && rrow >= '1') {
							rcol -= 48;
							rrow -= 48;
							//If we got both good data for row and col, run the function and get the result
							int result = removePiece(board, 2, rcol, rrow);
							//1 means the player tried to remove a piece they couldnt
							if (result == 1) cout << "\nYou can't remove that piece.\n";
							//2 or 3 means someone won
							if (result == 2) { status1 = 2; showBoard(board); success = true; cout << "\nPiece removed\n"; break; }
							if (result == 3) { status2 = 2; showBoard(board); success = true; cout << "\nPiece removed\n"; break; }
							//0 means it worked but no one won
							if (result == 0) { success = true; showBoard(board);  cout << "\nPiece removed\n";  break; }
						}
					}
					//If we didn't break before, reprint everything
					//Then break again
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "\nTry again\n";
					showBoard(board);
					cout << "\nGo ahead player two\nChoose a row to play\n";
					if (rm) cout << "Or hit r to remove a piece\n";
					break;

				}
			}
			else {
				//If there was bad input...
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "\nEnter a value between 1 and 7. Try again player two.\n";
				if (rm) cout << "Or hit r to remove a piece\n";
			}
			//If success, then end the turn
			if (success) break;
		}
		//Look for a winner
		if (status1 == 2) {
			cout << "\nPlayer one has achieved victory!\n";
			break;
		}
		if (status2 == 2) {
			cout << "\nPlayer two has achieved victory!\n";
			break;
		}
	}


	system("PAUSE");
	return 0;
}

