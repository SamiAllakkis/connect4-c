#include <stdio.h>
#include "../include/board.h"

int main(){
	char board[ROWS][COLS];
	char playerA, playerB;

	printf("Welcome to Connect Four!\n");

	//Ask players for their desired symbols
	printf("Player A:");
	scanf(" %c", &playerA);
	while(getchar() != '\n');	//clear leftover input

	printf("Player B:");
	scanf(" %c", &playerB);
	while(getchar() != '\n');	//clear leftover input

	printf("\n");

	//Creating an empty board
	createBoard(board);
	printBoard(board);

	return 0;
}