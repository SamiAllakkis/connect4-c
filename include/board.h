#ifndef BOARD_H
#define BOARD_H

#define ROWS 6
#define COLS 7

void createBoard(char board[ROWS][COLS]);
void printBoard(char board[ROWS][COLS]);
int dropChecker(char board[ROWS][COLS], int col, char player);
int checkWin(char board[ROWS][COLS], int col, char player);
int similuateDropWin(char board[ROWS][COLS], int col, char player);
int similuateDropDoubleLine(char board[ROWS][COLS], char player);
int bot1(char board[ROWS][COLS]);
int bot2(char board[ROWS][COLS], int moves);
int bot3(char board[ROWS][COLS], int moves, char player);

#endif
