#include <stdio.h>
#include "../include/board.h"

int main(){
	char board[ROWS][COLS];
	char playerA, playerB;
	int col, turn = 0;
	int win = 0;
        int moves=0;   // used for the draw
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
        // Player A's turn
        printf("Player A (%c), choose a column (1-%d): ", playerA, COLS);
    }else{
        // Player B's turn
        printf("Player B (%c), choose a column (1-%d): ", playerB, COLS);
    }

    // Read the column
    if(scanf("%d", &col) != 1){
        printf("Invalid input. Please enter a number between 1 and %d.\n", COLS);
        while(getchar() != '\n');  // clear buffer
        continue;
    }
    while(getchar() != '\n');      // clear buffer
    col--;  // convert to 0-based index

    // Validate column range
    if(col < 0 || col >= COLS){
        printf("Invalid column. Choose a column between 1 and %d.\n", COLS);
        continue;
    }

    if(turn==0){
        // Player A drops checker
        if(dropChecker(board, col, playerA)){
            moves++;
            printBoard(board);
            if(checkWin(board, playerA)){
                printf("Player A (%c) wins!\n", playerA);
                break;
            }
            if(moves == ROWS * COLS){
                printf("It's a tie! The board is full.\n");
                break;
            }
            turn = 1 - turn;  // switch to player B
        }else{
            printf("Column %d is full. Try another column.\n", col+1);
        }
    }else{
        // Player B drops checker
        if(dropChecker(board, col, playerB)){
            moves++;
            printBoard(board);
            if(checkWin(board, playerB)){
                printf("Player B (%c) wins!\n", playerB);
                break;
            }
            if(moves == ROWS * COLS){
                printf("It's a tie! The board is full.\n");
                break;
            }
            turn = 1 - turn;  // switch to player A
        }else{
            printf("Column %d is full. Try another column.\n", col+1);
        }
    }
}

}
