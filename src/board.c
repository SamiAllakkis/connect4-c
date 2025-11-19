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

int simulateDropWin(char board[ROWS][COLS], int col, char player){
    if(col < 0 || col>= COLS) return 0; //invalid col
    if(board[0][col] != '.') return 0; //col is full

    if(!dropChecker(board, col, player)) return 0;
    int win = checkWin(board, col, player);

    //undoing the move
    for(int i=0; i<ROWS; i++){
        if(board[i][col]!='.'){
            board[i][col] = '.';
            break;
        }
    }
    return win;
}

int simulateDropDoubleLine(char board[ROWS][COLS], char player){
    for(int c=0; c<COLS; c++){
        if(board[0][c]!='.') continue;

        dropChecker(board, c, player);
        int counter = 0;

        for(int k=0; k<COLS; k++){
            if(simulateDropWin(board, k, player)){
                counter++;
                if(counter>=2){
                    //undoing the move
                    for(int i=0; i<ROWS; i++){
                        if(board[i][c]!='.'){
                            board[i][c] = '.';
                            break;
                        }
                    }
                    return c;
                }
            }
        }

        //undoing the move
        for(int i=0; i<ROWS; i++){
            if(board[i][c]!='.'){
                board[i][c] = '.';
                break;
            }
        }
    }
    return -1;
}

int dropAllowsWin(char board[ROWS][COLS], int col, char player){
    if(board[0][col]!='.') return 1;

    int row;
    for(row= ROWS-1; row>=0; row--){
        if(board[row][col]=='.'){
            board[row][col]='B';
            break;
        }
    }

    int unsafe = 0;

    for(int c=0; c<COLS; c++){
        if(board[0][c]=='.'){
            for(int r = ROWS-1; r>=0; r--){
                if(board[r][c]=='.'){
                    board[r][c]=player;
                    if(checkWin(board, c, player)) unsafe=1;
                    board[r][c]='.';
                    break;
                }
            }
            if(unsafe)break;
        }
    }

    //undo bot move
    board[row][col]='.';
    return unsafe;
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
        if(simulateDropWin(board, c, 'B')) return c;
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

int bot3(char board[ROWS][COLS], int moves, char player){
    int col;
    int midcol = COLS/2;

    //Step 1: Check if we can win with 1 move
    for(int c=0; c<COLS; c++){
        if(simulateDropWin(board, c, 'B')){
            return c;
        }
    }

    //Step 2: Check if opponent can win in 1 move to block
    for(int c=0; c<COLS; c++){
        if(simulateDropWin(board, c, player)){
            return c; 
        } 
    }

    //Step 3: Check for the possible creation of a double line from the opponent to block
    int res = simulateDropDoubleLine(board, player);
    if((res>=0) && !dropAllowsWin(board, res, player)){
        return res;
    }

    //Step 4: Check for the possible creation of a double line 
    res = simulateDropDoubleLine(board, 'B');
    if((res>=0) && !dropAllowsWin(board, res, player)){
        return res;
    }

    //Step 5: playing center
    int centerOptions[] = {midcol, midcol+1, midcol-1, midcol+2, midcol-2, midcol+3, midcol-3};
    for(int i=0; i<7; i++){
        int c = centerOptions[i];
        if(c>=0 && c<COLS && board[0][c]=='.' && !dropAllowsWin(board, c, player)){
            return c;
        }
    }

    //Edge case: Random answer
    do{
       col = rand()%COLS;
    } while(board[0][col]!='.');

    return col;
}
