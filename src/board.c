#include <stdio.h>
#include "../include/board.h"

void createBoard(char board[ROWS][COLS]){
	for(int i=0; i<ROWS; i++){
		for(int j=0; j<COLS; j++){
			board[i][j]='.';
		}
	}
}

void printBoard(char board[ROWS][COLS]){
	for(int i=0; i<ROWS; i++){
		for(int j=0; j<COLS; j++){
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}

	//Printing the column numbers
	for(int j=0; j<COLS; j++){
		printf("%d ", j+1);
	}
	printf("\n\n");
}

int dropChecker(char board[ROWS][COLS], int col, char player){
	if(col<0 || col >= COLS) return 0;	//invalid input

	for(int i=ROWS-1; i>=0; i--){
		if(board[i][col]=='.'){
			board[i][col] = player;
			return 1;
		}
	}
	return 0;	//column is full
}
