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
bool checkWinner(int s1, int s2) {
	//Two is the magic number that means someone won
	//If both players won then print out it was a draw
	if (s1 == 2 && s2 == 2) {
		cout << "\nThis game is a draw!\n";
		return true;
	}
	//Or see if just one of the players won
	else if (s1 == 2) {
		cout << "\nPlayer one wins!\n";
		return true;
	}
	else if (s2 == 2) {
		cout << "\nPlayer two wins!\n";
		return true;
	}
	else return false;
}
bool checkVictory(int i, int j, char board[][7], bool wrapAround) {
	//CheckVictory will return true if a player won
	//First lets look at the piece we were passed in
	char target = board[i][j];
	//If we were passed in a dash, then obviously no one won
	if (target == '-') return false;
	//These min max values are going to be how we handle the wrap around win condition
	//
	int cmax = 8;
	int rmax = 7;
	int cmin = 0;
	int rmin = 0;
	int a, b;
	//If we have wrap around on, increase the range our for loops are going to loop by an additional 3 loops
	//3 because that's how far a wrap around might go
	if (wrapAround) { cmax += 3; rmax += 3; cmin -= 3; rmin -= 3; }
	//First lets check if we have a horizontal win.
	//We will do these checks by incrementing a counter
	int horizCounter = 0;
	//Let's search the entire row for a string of 4 target characters
	for (int k = 1; k < cmax; k++) {
		//We need this a = k line because if we are in wrap around mode we can't use k
		a = k;
		//If k has set a to an out of bounds value, then we are going to decrement a to create our wrap around effect
		if (a > 7) a -= 7;
		//If we found our character, increment one
		if (board[a][j] == target) horizCounter++;
		//If we found one of the other two characters, reset
		else horizCounter = 0;
		//If we reached 4, someone won!
		if (horizCounter == 4) return true;
	}
	//Let's do it again for verticle
	int vertCounter = 0;
	for (int k = 1; k < rmax; k++) {
		b = k;
		if (b > 6) b -= 6;
		//This time the column stays the same but the row changes
		if (board[i][b] == target) vertCounter++;
		else vertCounter = 0;
		if (vertCounter == 4) return true;
	}
	//Now for diagonals
	//First we have to find the lowest point on our potential diagonal win
	//For up-right we would do this with c and d
	int c, d;
	for (c = i, d = j; c > cmin && d < rmax; c--, d++);
	//Basically using a for loop we found the first 'edge' of the diagonal, whether it be on the column or row
	//Thinking about it now with the way for loops work, this might create an out of bound exception. Hmmmmm.
	int diagCounter1 = 0;
	//Now we move along the diagonal to find a row of four
	for (; c < cmax && d > rmin; c++, d--) {
		//These six lines are accounting for wrap around again
		a = c;
		if (a > 7) a -= 7;
		if (a < 1) a += 7;
		b = d;
		if (b > 6) b -= 6;
		if (b < 1) b += 6;
		if (board[a][b] == target) diagCounter1++;
		else diagCounter1 = 0;
		if (diagCounter1 == 4) return true;
	}
	//up left is similar but we change the way we move down
	for (c = i, d = j; c < cmax && d < rmax; c++, d++);
	int diagCounter2 = 0;
	//and change the way we move up
	for (; c > cmin && d > rmin; c--, d--) {
		a = c;
		if (a < 1) a += 7;
		if (a > 7) a -= 7;
		b = d;
		if (b > 6) b -= 6;
		if (b < 1) b += 6;
		if (board[a][b] == target) diagCounter2++;
		else diagCounter2 = 0;
		if (diagCounter2 == 4) return true;
	}
	//If we have wrap around enabled, then we also have to check dia
	//If we never go to return true on any of these four loops, there was no winner
	return false;
}
int removePiece(char board[][7], int player, int i, int j, bool wrapAround, int &status1, int &status2) {
	//Removing a piece is funky but we can do it
	int a, b;
	//Find the character the player is trying to remove
	char target = board[i][j];
	//And make sure the player is trying to remove only their character
	if (player == 2 && target != 'o' || player == 1 && target != 'x') return 1;
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
		if (checkVictory(i, k, board, wrapAround)) {
			//If there is a winner, whose piece is it? Set their status to the 'victory' status of 2
			if (board[i][k] == 'x') status1 = 2;
			if (board[i][k] == 'o') status2 = 2;
		}
	}
	return 0;
}
int playMove(int player, int col, char board[][7], bool wrapAround) {
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
			if (checkVictory(col, i, board, wrapAround)) return 2;
			//If not, return 1
			else return 1;
		}
	}
	//If we reached this point is means that we exited the for loop and that column was full
	return 3;
}
void playerOnesTurn(char board[][7], bool rm, bool wm, int &numTurns, int &status1, int &status2) {
	while (true) {
		//This bool signifies if the user put in good input
		//If they did, we will use success to break out of this turn's loop
		bool success = false;
		char col;
		//Get the input
		cin >> col;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		//If it was between 1 and 7 then we can play that move
		if (col >= '1' && col <= '7') {
			//We subtract by 48 to account for ascii
			col -= 48;
			//Play player one's move
			status1 = playMove(1, col, board, wm);
			//Show the board
			showBoard(board);
			//If the move was successful then break out of the loop. If not, 
			if (status1 == 1 || status1 == 2) { success = true; numTurns++; }
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
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
						int result = removePiece(board, 1, rcol, rrow, wm, status1, status2);
						//1 means the player tried to remove a piece they couldnt
						if (result == 1) cout << "\nYou can't remove that piece.\n";
						//If result is 0, then the removal worked and we may even have winners
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
}
void playerTwosTurn(char board[][7], bool rm, bool wm, int &numTurns, int &status1, int &status2) {
	while (true) {
		char col;
		cin >> col;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		bool success = false;
		if (col >= '1' && col <= '7') {
			col -= 48;
			//Play player two's move
			status2 = playMove(2, col, board, wm);
			showBoard(board);
			if (status2 == 1 || status2 == 2) { success = true; numTurns++; }
			if (status2 == 3) cout << "\nThat column is full!\nTry again\n";
		}
		else if (col == 'r' && rm) {
			//start another input loop for a col and row
			while (true) {
				cout << "\nPlayer two has chosen to remove a piece.\nPlease select the column of the piece\n";
				char rcol;
				cin >> rcol;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				//If rcol was good, get rrow
				if (rcol >= '1' && rcol <= '7') {
					cout << "\nNow choose a row\n";
					char rrow;
					cin >> rrow;
					if (rrow <= '6' && rrow >= '1') {
						rcol -= 48;
						rrow -= 48;
						//If we got both good data for row and col, run the function and get the result
						int result = removePiece(board, 2, rcol, rrow, wm, status1, status2);
						//1 means the player tried to remove a piece they couldnt
						if (result == 1) cout << "\nYou can't remove that piece.\n";
						//If result is 0, then the removal worked and we may even have winners
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
}

int main()
{
	//This is the main game loop that we will go back to if the player wants to play again
	while (true) {
		//First of all, will we have removal mode?
		char removalMode;
		char wrapMode;
		bool rm = false;
		bool wm = false;
		cout << "Let's play connect 4!\n";
		system("pause");
		cout << "\nWould you like to play removal mode?\ny or n\n";
		//Let's make an infinite for loop waiting for good input
		while (true) {
			//Get the removal mode
			cin >> removalMode;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			//If the input is good, break
			if (removalMode == 'y') {
				cout << "\nRemoval Mode on\n";
				rm = true;
				break;
			}
			if (removalMode == 'n') {
				cout << "\nRemoval mode off\n";
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
		//Now lets do it for wrap-around mode
		cout << "\nWould you like to play wrap-around mode?\ny or n\n";
		while (true) {
			//Get the removal mode
			cin >> wrapMode;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			//If the input is good, break
			if (wrapMode == 'y') {
				cout << "\nWrap-around Mode on\nGreat, let's begin!\n";
				wm = true;
				break;
			}
			if (wrapMode == 'n') {
				cout << "\nWrap-around mode off\nGreat, let's begin!\n";
				wm = false;
				break;
			}
			//If not, clear cin and try again
			//Got these two lines of code from stack overflow
			//I will be using them a lot
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nEnter y or n to play with wrap-around mode\n";
		}

		//Once we have the removal mode and wrap mode let's make the game board by making a double char array
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
		//Let's make the tracker for how many turns have gone by
		int numTurns = 0;
		//Show the empty board then start the game
		cout << "\nHere is the board!\n";
		showBoard(board);
		//This first loop is the core game loop
		while (true) {
			//before we even get into player one's turn, lets check numTurns
			//We will have to do this before player two's turn as well
			//The thing to note is that if there is removal mode, the game is never a draw because players can remove pieces
			if (numTurns == 42 && !rm) { cout << "\nThe board is full!\nThis game is a draw\n"; break; }
			//Status will be the check for certain board conditions such as overflow or victory
			int status1 = 0;
			int status2 = 0;

			cout << "\nGo ahead player one\nChoose a row to play\n";
			if (rm) cout << "Or hit r to remove a piece\n";
			//This is the start of player one's turn
			playerOnesTurn(board, rm, wm, numTurns, status1, status2);

			//See if anyone won
			if (checkWinner(status1, status2)) break;

			//Then check numTurns
			if (numTurns == 42 && !rm) { cout << "\nThe board is full!\nThis game is a draw\n"; break; }
			
			//Now player two's turn
			cout << "\nYour turn player two\nChoose a row to play\n";
			if (rm) cout << "Or hit r to remove a piece\n";
			//Do what we just did again, but for player two this time
			playerTwosTurn(board, rm, wm, numTurns, status1, status2);

			//Look for a winner
			if (checkWinner(status1, status2)) break;
		}
		//Here is where we can play again
		//which is only asked if we reached the end of the second outer most while loop
		cout << "\nThe game has ended, would you like to play again?\ny or n\n";
		while (true) {
			char playAgain;
			cin >> playAgain;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (playAgain == 'y') break;
			else if (playAgain == 'n') return 0;
			else {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "The game has ended, would you like to play again?\ny or n\n";
			}
		}
	}
}

