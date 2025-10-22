#include "../include/board.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    srand(time(NULL));  // seed random once here

	char board[ROWS][COLS];
	char playerA, playerB;
	int col, turn = 0;
	int win = 0;
    int moves=0;   // used for the draw
    int opponent;   //used to know if its human or a bot (and which one)
	printf("Welcome to Connect Four!\n");

    //Ask players wether they want to play against a human or a bot
    printf("Enter 0 if you want to play against a human, and 1 if you want to play agaisnt the easy bot!\n");
    scanf(" %d", &opponent);
    while(getchar() != '\n');   //clear leftover input

	//Ask players for their desired symbols
	printf("Player A:");
	scanf(" %c", &playerA);
	while(getchar() != '\n');	//clear leftover input

    //Setting playerB's name in case of a bot playing
    if(opponent!=0){
        printf("Player B:");
        scanf(" %c", &playerB);
        while(getchar() != '\n');   //clear leftover input
    }
    else{
        playerB = 'B';
    }

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
        if(opponent==0){
            printf("Player B (%c), choose a column (1-%d): ", playerB, COLS);
        }
        else{
            printf("BOT%d (%c), choose a column (1-%d):", opponent, playerB, COLS);
        }
    }

    // Read the column
    if(opponent==0){
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
    }
    else{
        col = bot1(board);
        printf("%d\n", col+1);
    }

    if(turn==0){
        // Player A drops checker
        if(dropChecker(board, col, playerA)){
            moves++;
            printBoard(board);
            if(checkWin(board, col, playerA)){
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
            if(checkWin(board, col, playerB)){
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
