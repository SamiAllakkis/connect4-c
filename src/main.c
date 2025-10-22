#include "../include/board.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    srand(time(NULL));  // seed random once here

    char board[ROWS][COLS];
    char playerA, playerB;
    int col, turn = 0;
    int moves=0;   // used for the draw
    int opponent=-1;   //used to know if its human or a bot (and which one)
	printf("Welcome to Connect Four!\n");

    //Ask players whether they want to play against a human or a bot
    int valid_choices[] = {0, 1};
    int found;

    while (1) {
      printf("Enter 0 to play against a human, or 1 to play against the easy bot: ");
        if (scanf("%d", &opponent) != 1) {
         printf("Invalid input. Please type 0 or 1.\n");
          while (getchar() != '\n'); // clear invalid input
          continue;
    }
    while (getchar() != '\n'); // clear newline

    // check if input is in the list
    found = 0;
    for (int i = 0; i < 2; i++) {
        if (opponent == valid_choices[i]) {
            found = 1;
            break;
        }
    }

    if (found) break; // valid input, exit loop
    printf("Invalid choice. Please enter 0 or 1.\n");
}

    while(getchar() != '\n');   //clear leftover input

	//Ask players for their desired symbols
	printf("Player A:");
	scanf(" %c", &playerA);
	while(getchar() != '\n');	//clear leftover input

    //Setting playerB's name in case of a human playing
    if(opponent==0){
        printf("Player B:");
        scanf(" %c", &playerB);
        while(getchar() != '\n');   //clear leftover input
    }
    else{
        playerB = 'B';
        if (playerA == 'B') playerB = 'O';  // Avoid duplicate symbol
    }

	printf("\n");

	//Creating an empty board
	createBoard(board);
	printBoard(board);
        // will exit on breaks
	while(1){
    

    if (turn == 0) {
    // Player A (always human)
    printf("Player A (%c), choose a column (1-%d): ", playerA, COLS);
    if (scanf("%d", &col) != 1) {
        printf("Invalid input.\n");
        while(getchar() != '\n');
        continue;
    }
    while(getchar() != '\n');
    col--;
} else {
    // Player B
    if (opponent == 0) {
        printf("Player B (%c), choose a column (1-%d): ", playerB, COLS);
        if (scanf("%d", &col) != 1) {
            printf("Invalid input.\n");
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');
        col--;
    } else {
        // Bot mode
        col = bot1(board);
        printf("column %d\n", col + 1);
    }
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
return 0;
}
