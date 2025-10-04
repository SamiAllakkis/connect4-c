#include <stdio.h>
#include "../include/board.h"

int main(){
	char board[ROWS][COLS];
	char playerA, playerB;
	char player[2];
	int col, turn = 0;
	int win = 0;

	printf("Welcome to Connect Four!\n");

	//Ask players for their desired symbols
	printf("Player A:");
	scanf(" %c", &player[0]);
	while(getchar() != '\n');	//clear leftover input

	printf("Player B:");
	scanf(" %c", &player[1]);
	while(getchar() != '\n');	//clear leftover input

	printf("\n");

	//Creating an empty board
	createBoard(board);
	
	while(!win){
		printBoard(board);
		if (boardFull(board)){
			win = -1;
			break;
		}
		

		if(turn==0){
			printf("Player A, choose a column (1-%d): ", COLS);
		}else{
			printf("Player B, choose a column (1-%d): ", COLS);
		}


		while (1){
			scanf("%d", &col);
			while(getchar() != '\n');	//clear leftover input
			col--;	//0 indexed
			if (dropChecker(board, col, player[turn])){
				break;
			}
			
		}


		win = checkWin(board, player[turn]); //check if curren player won
		if (win){break;}

		turn = 1-turn;	//switching turns
	}

	if (win == -1){
		puts("It's a draw");
	} else if (turn == 0){
		puts("Player A wins!");
	} else {
		puts("Player B wins!");
	}
	

	return 0;
}