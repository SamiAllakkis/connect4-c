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
    int valid_choices[] = {0, 1, 2, 3};
    int found;

    while (1) {
        printf("Choose your opponent:\n  0 - Play against a human\n  1 - Play against the easy-level bot\n  2 - Play against the medium-level bot\n  3 - Play against the hard-level bot\nEnter your choice: ");
        if (scanf("%d", &opponent) != 1) {
            printf("Invalid input. Please type 0, 1, 2, or 3.\n");
                while (getchar() != '\n'); // clear invalid input
                continue;
        }

        while (getchar() != '\n'); // clear newline

        // check if input is in the list
        found = 0;
        for (int i = 0; i < 4; i++) {
            if (opponent == valid_choices[i]) {
                found = 1;
                break;
            }
        }

        if (found) break; // valid input, exit loop
        printf("Invalid choice. Please enter 0, 1, 2, or 3.\n");
    }

    //Ask players for their desired symbols
    printf("Player A:");
    scanf(" %c", &playerA);

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
        }

        else {
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
            }

            else if(opponent == 1){
                // Bot1 mode
                col = bot1(board);
                printf("Bot1 chose column %d\n", col + 1);
            }
            else if(opponent == 2){
                // Bot2 mode
                col = bot2(board, moves);
                printf("Bot2 chose column %d\n", col + 1);
            }
            else{
                // Bot2 mode
                col = bot3(board, moves, playerA);
                printf("Bot3 chose column %d\n", col + 1);
            }
        }


        if(turn==0){
            // Player A drops checker
            if(col<0 || col>=COLS){
                printf("Invalid column. Choose a number between 1 and %d.\n", COLS);
                continue;
            }
            if(!dropChecker(board, col, playerA)){
                printf("Column %d is full. Try another column.\n", col+1);
                continue;
            }

            moves++;
            printBoard(board);
            if(checkWin(board, col, playerA)){
                printf("\nPlayer A (%c) wins!\n\n", playerA);
                break;
            }
            if(moves == ROWS * COLS){
                printf("It's a tie! The board is full.\n");
                break;
            }
            turn = 1 - turn;  // switch to player B
        }

        else{
            // Player B drops checker
            if(col<0 || col>=COLS){
                printf("Invalid column. Choose a number between 1 and %d.\n", COLS);
                continue;
            }
            if(!dropChecker(board, col, playerB)){
                printf("Column %d is full. Try another column.\n", col+1);
                continue;
            }
            moves++;
            printBoard(board);
            if(checkWin(board, col, playerB)){
                printf("\nPlayer B (%c) wins!\n\n", playerB);
                break;
            }
            if(moves == ROWS * COLS){
                printf("It's a tie! The board is full.\n");
                break;
            }
            turn = 1 - turn;  // switch to player A
        }
    }
    return 0;
}
