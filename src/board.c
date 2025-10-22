#include "../include/board.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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
    if(col<0 || col >= COLS) return 0;  //invalid input

    for(int i=ROWS-1; i>=0; i--){
        if(board[i][col]=='.'){
            board[i][col] = player;
            return 1;
        }
    }
    return 0;   //column is full
}

int checkWin(char board[ROWS][COLS], int col, char player) {
    // Find the latest checker
    int row;
    for(int i = 0; i < ROWS; i++){
        if(board[i][col]!='.'){
            row = i;
            break;
        }
    }

    // Check horizontal
    for(int i = 0; i < COLS-3; i++) {
        if(board[row][i] == player &&
            board[row][i + 1] == player &&
            board[row][i + 2] == player &&
            board[row][i + 3] == player)
            return 1;
    }

    // Check vertical
    for(int i = 0; i < ROWS - 3; i++) {
        if(board[i][col] == player &&
            board[i + 1][col] == player &&
            board[i + 2][col] == player &&
            board[i + 3][col] == player)
            return 1;
    }

    // Setup for diagonal check
    int max_diag = ROWS < COLS ? ROWS : COLS;
    char arr[max_diag];
    for(int i=0; i<max_diag; i++){
        arr[i] = 0;
    }

    // Check diagonal (\)
    int i = row;
    int j = col;

    while(i>0 && j>0){
        i--; j--;
    }

    int idx = 0;
    while(i<ROWS && j<COLS){
        arr[idx++] = board[i++][j++];
    }

    for(int i = 0; i < idx - 3; i++) {
        if(arr[i] == player &&
            arr[i+1] == player &&
            arr[i+2] == player &&
            arr[i+3] == player)
            return 1;
    }

    // Check diagonal (/)
    i = row;
    j = col;

    while(i>0 && j<COLS-1){
        i--; j++;
    }

    idx = 0;
    while(i<ROWS && j>=0){
        arr[idx++] = board[i++][j--];
    }

    for(int i = 0; i < idx - 3; i++) {
        if(arr[i] == player &&
            arr[i+1] == player &&
            arr[i+2] == player &&
            arr[i+3] == player)
            return 1;
    }

    return 0;
}

int bot1(char board[ROWS][COLS]){
    int col;
    int midcol = COLS/2;

    //50% chance of choosing the middle column if its not full
    if(board[0][midcol]=='.' && rand()%2==0){
        col = midcol;
    }
    else{
        do{
            col = rand()%COLS;
        }while(board[0][col]!='.');
    }

    sleep(1);  // Bot thinking simulation delay
    return col;
}