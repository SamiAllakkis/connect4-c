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

int similuateDrop(char board[ROWS][COLS], int col){
    if(col < 0 || col>= COLS) return 0; //invalid col
    if(board[0][col] != '.') return 0; //col is full

    if(!dropChecker(board, col, 'B')) return 0;
    int win = checkWin(board, col, 'B');

    //undoing the move
    for(int i=0; i<ROWS; i++){
        if(board[i][col]!='.'){
            board[i][col] = '.';
            break;
        }
    }
    return win;
}

int bot1(char board[ROWS][COLS]){
    //usleep(500000);  // sleep for 0.5 seconds to simulate bot thinikng
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

    return col;
}

int bot2(char board[ROWS][COLS], int moves){
    //usleep(500000);  // sleep for 0.5 seconds to simulate bot thinikng
    int col;
    int midcol = COLS/2;

    //Early in game, playing center
    if(moves<=4){
        if(board[0][midcol]=='.') return midcol;
        if(board[0][midcol+1]=='.') return midcol+1;
        if(board[0][midcol-1]=='.') return midcol-1;
    }

    //Mid-Game Strategy
    //Step 1: Win if possible
    for(int c=0; c<COLS; c++){
        if(similuateDrop(board, c)) return c;
    }

    //Step 2: Add randomness ot make the bot unpredictable
    if(rand()%4 != 0){
        //Step 3: Double-line strategy
        for(int c=0; c<COLS; c++){
            if(board[0][c]=='.'){
                int row = ROWS-1;
                while(row>=0 && board[row][c] != '.') row--;
                if((c>0 && board[row][c-1]=='B') || (c< COLS-1 && board[row][c+1]=='B')) 
                    return c;
            }
        }
    }

    //Step 3: Random answer
    do{
       col = rand()%COLS;
    } while(board[0][col]!='.');


    return col;
}