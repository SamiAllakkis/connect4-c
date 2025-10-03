#include <stdio.h>
#include "../include/board.h"

int main(){
	char board[ROWS][COLS];
	char playerA, playerB;
	int col, turn = 0;
	int win = 0;

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

	while(!win){
		if(turn==0){
			printf("Player A, choose a column (1-%d): ", COLS);
		}else{
			printf("Player B, choose a column (1-%d): ", COLS);
		}

		scanf("%d", &col);
		while(getchar() != '\n');	//clear leftover input
		col--;	//0 indexed


		if(turn==0){

		}else{

		}


		printBoard(board);
		turn = 1-turn;	//switching turns

	}

	return 0;
}