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