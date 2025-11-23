#include "../include/board.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>
#include <string.h>

// Helper function to find first free row in a column
static int firstFreeRow(char board[ROWS][COLS], int col) {
    for (int r = ROWS - 1; r >= 0; r--) {
        if (board[r][col] == '.') return r;
    }
    return -1;
}

// Check if any player has won (checks entire board)
static int hasAnyoneWon(char board[ROWS][COLS], char player) {
    // Horizontal
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            if (board[r][c] == player && board[r][c+1] == player &&
                board[r][c+2] == player && board[r][c+3] == player)
                return 1;
        }
    }
    
    // Vertical
    for (int c = 0; c < COLS; c++) {
        for (int r = 0; r < ROWS - 3; r++) {
            if (board[r][c] == player && board[r+1][c] == player &&
                board[r+2][c] == player && board[r+3][c] == player)
                return 1;
        }
    }
    
    // Diagonal (\)
    for (int r = 0; r < ROWS - 3; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            if (board[r][c] == player && board[r+1][c+1] == player &&
                board[r+2][c+2] == player && board[r+3][c+3] == player)
                return 1;
        }
    }
    
    // Diagonal (/)
    for (int r = 3; r < ROWS; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            if (board[r][c] == player && board[r-1][c+1] == player &&
                board[r-2][c+2] == player && board[r-3][c+3] == player)
                return 1;
        }
    }
    
    return 0;
}

// Check if board is full (draw)
static int isBoardFull(char board[ROWS][COLS]) {
    for (int c = 0; c < COLS; c++) {
        if (board[0][c] == '.') return 0;
    }
    return 1;
}

// (Place these definitions where your other helpers exist)

// Evaluate a line of 4 cells
static int evaluateLine(char a, char b, char c, char d, char bot, char opp) {
    int score = 0;
    int botCount = 0, oppCount = 0, emptyCount = 0;
    char arr[4] = {a, b, c, d};
    
    for (int i = 0; i < 4; i++) {
        if (arr[i] == bot) botCount++;
        else if (arr[i] == opp) oppCount++;
        else emptyCount++;
    }
    
    if (botCount == 4) score += INF;
    else if (botCount == 3 && emptyCount == 1) score += 100;
    else if (botCount == 2 && emptyCount == 2) score += 10;
    
    if (oppCount == 4) score -= INF;
    else if (oppCount == 3 && emptyCount == 1) score -= 120;
    else if (oppCount == 2 && emptyCount == 2) score -= 10;
    
    return score;
}

// Score the board position (keeps double-threat bonus)
static int scorePosition(char board[ROWS][COLS], char bot, char opp) {
    int score = 0;
    
    // Center column preference
    int centerCol = COLS / 2;
    
    for (int r = 0; r < ROWS; r++) {
        if (board[r][centerCol] == bot) score += 7;
    }
    
    // Horizontal
    #pragma omp parallel for reduction(+:score)
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS - 3; c++)
            score += evaluateLine(board[r][c], board[r][c+1], board[r][c+2], board[r][c+3], bot, opp);
    
    // Vertical
    #pragma omp parallel for reduction(+:score)
    for (int c = 0; c < COLS; c++)
        for (int r = 0; r < ROWS - 3; r++)
            score += evaluateLine(board[r][c], board[r+1][c], board[r+2][c], board[r+3][c], bot, opp);
    
    // Diagonal (\)
    #pragma omp parallel for reduction(+:score)
    for (int r = 0; r < ROWS - 3; r++)
        for (int c = 0; c < COLS - 3; c++)
            score += evaluateLine(board[r][c], board[r+1][c+1], board[r+2][c+2], board[r+3][c+3], bot, opp);
    
    // Diagonal (/)
    #pragma omp parallel for reduction(+:score)
    for (int r = 3; r < ROWS; r++)
        for (int c = 0; c < COLS - 3; c++)
            score += evaluateLine(board[r][c], board[r-1][c+1], board[r-2][c+2], board[r-3][c+3], bot, opp);
    
    // double threat bonus
    for (int c = 0; c < COLS; c++) {
        int r = firstFreeRow(board, c);
        if (r < 0) continue;
        board[r][c] = bot;
        int winCount = 0;
        for (int nc = 0; nc < COLS; nc++) {
            int nr = firstFreeRow(board, nc);
            if (nr < 0) continue;
            board[nr][nc] = bot;
            if (hasAnyoneWon(board, bot)) winCount++;
            board[nr][nc] = '.';
        }
        board[r][c] = '.';
        if (winCount >= 2) score += 480;
    }

    return score;
}

// Minimax with alpha-beta pruning and safe-handling of unsafe moves
static int minimax(char board[ROWS][COLS], int depth, int maximizing,
                   char bot, char opp, int alpha, int beta) {
    if (hasAnyoneWon(board, bot)) return INF + depth;  
    if (hasAnyoneWon(board, opp)) return -INF - depth;  
    if (isBoardFull(board) || depth == 0) return scorePosition(board, bot, opp);

    int moveOrder[COLS] = {3, 2, 4, 1, 5, 0, 6};

    if (maximizing) {
        int maxEval = -INF;
        for (int i = 0; i < COLS; i++) {
            int c = moveOrder[i];
            int r = firstFreeRow(board, c);
            if (r < 0) continue;

            // make move
            board[r][c] = bot;
            // check if this move immediately allows opponent a winning reply
            int allowsOppWin = dropAllowsWin(board, c, opp);
            int eval;
            if (!allowsOppWin) {
                eval = minimax(board, depth - 1, 0, bot, opp, alpha, beta);
            } else {
                // heavy penalty 
                eval = -INF - depth; 
            }
            // undo move
            board[r][c] = '.';

            if (eval > maxEval) maxEval = eval;
            if (maxEval > alpha) alpha = maxEval;
            if (alpha >= beta) break;
        }
        return maxEval;
    } else {
        int minEval = INF;
        for (int i = 0; i < COLS; i++) {
            int c = moveOrder[i];
            int r = firstFreeRow(board, c);
            if (r < 0) continue;

            board[r][c] = opp;
            int eval = minimax(board, depth - 1, 1, bot, opp, alpha, beta);
            board[r][c] = '.';

            if (eval < minEval) minEval = eval;
            if (minEval < beta) beta = minEval;
            if (alpha >= beta) break;
        }
        return minEval;
    }
}

int findBestMoveMinmax_parallel(char board[ROWS][COLS], char bot, char opp, int depth) {
    int moveOrder[COLS] = {3, 2, 4, 1, 5, 0, 6};
    int scores[COLS];
    int legal[COLS]; // 1 if legal else 0

    // init
    for (int i = 0; i < COLS; ++i) {
        scores[i] = -INF;
        legal[i] = 0;
    }

    // Parallel loop across moveOrder indices
    #pragma omp parallel for 
    for (int idx = 0; idx < COLS; ++idx) {
        int c = moveOrder[idx];
        int r = firstFreeRow(board, c);
        if (r < 0) {
            // column full, leave score as -INF
            continue;
        }

        // Make a thread-local copy of the board
        char local[ROWS][COLS];
        memcpy(local, board, sizeof(local));

        // Play the move on local board
        local[r][c] = bot;

        // If move instantly allows opponent to win, assign heavy penalty (same logic as root)
        if (dropAllowsWin(local, c, opp)) {
            // use same penalty you use in serial root
            scores[c] = -INF - depth;
            legal[c] = 1;
            continue;
        }

        // Run minimax on local board (safe, no shared state modified)
        int val = minimax(local, depth - 1, 0, bot, opp, -INF, INF);

        scores[c] = val;
        legal[c] = 1;
    }

    // After parallel region: choose best column among legal moves
    int bestCol = -1;
    int bestScore = -INF;
    for (int i = 0; i < COLS; ++i) {
        int c = moveOrder[i];
        if (!legal[c]) continue;
        if (scores[c] > bestScore) {
            bestScore = scores[c];
            bestCol = c;
        }
    }

    // fallback if none found (shouldn't happen)
    if (bestCol == -1) {
        for (int i = 0; i < COLS; ++i) {
            int c = moveOrder[i];
            if (board[0][c] == '.') { bestCol = c; break; }
        }
    }

    return bestCol;
}

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

    for(int j=0; j<COLS; j++){
        printf("%d ", j+1);
    }
    printf("\n\n");
}

int dropChecker(char board[ROWS][COLS], int col, char player){
    if(col<0 || col >= COLS) return 0;

    for(int i=ROWS-1; i>=0; i--){
        if(board[i][col]=='.'){
            board[i][col] = player;
            return 1;
        }
    }
    return 0;
}

int checkWin(char board[ROWS][COLS], int col, char player) {
    int row;
    for(int i = 0; i < ROWS; i++){
        if(board[i][col]!='.'){
            row = i;
            break;
        }
    }

    for(int i = 0; i < COLS-3; i++) {
        if(board[row][i] == player &&
            board[row][i + 1] == player &&
            board[row][i + 2] == player &&
            board[row][i + 3] == player)
            return 1;
    }

    for(int i = 0; i < ROWS - 3; i++) {
        if(board[i][col] == player &&
            board[i + 1][col] == player &&
            board[i + 2][col] == player &&
            board[i + 3][col] == player)
            return 1;
    }

    int max_diag = ROWS < COLS ? ROWS : COLS;
    char arr[max_diag];
    for(int i=0; i<max_diag; i++){
        arr[i] = 0;
    }

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
    if(col < 0 || col>= COLS) return 0;
    if(board[0][col] != '.') return 0;

    if(!dropChecker(board, col, player)) return 0;
    int win = checkWin(board, col, player);

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

    board[row][col]='.';
    return unsafe;
}

int dropAllowsWinDouble(char board[ROWS][COLS], int col, char player){
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
                    if(simulateDropDoubleLine(board, player) >= 0){
                        unsafe=1;
                    }
                    board[r][c]='.';
                    break;
                }
            }
            if(unsafe)break;
        }
    }

    board[row][col]='.';
    return unsafe;
}

int bot1(char board[ROWS][COLS]){
    int col;
    int midcol = COLS/2;

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
    int col;
    int midcol = COLS/2;

    if(moves<=4){
        if(board[0][midcol]=='.') return midcol;
        if(board[0][midcol+1]=='.') return midcol+1;
        if(board[0][midcol-1]=='.') return midcol-1;
    }

    for(int c=0; c<COLS; c++){
        if(simulateDropWin(board, c, 'B')) return c;
    }

    if(rand()%4 != 0){
        for(int c=0; c<COLS; c++){
            if(board[0][c]=='.'){
                int row = ROWS-1;
                while(row>=0 && board[row][c] != '.') row--;
                if((c>0 && board[row][c-1]=='B') || (c< COLS-1 && board[row][c+1]=='B')) 
                    return c;
            }
        }
    }

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

    //Step 5: Use minimax to find the best move
    int searchDepth = 7;  // Adjust depth based on performance needs (3-4 for fast, 5-6 for strong)
    int bestCol = findBestMoveMinmax_parallel(board, 'B', player, searchDepth);
    if(bestCol >= 0 && board[0][bestCol] == '.' && !dropAllowsWin(board, bestCol, player)){
        return bestCol;
    }

    //Step 6: playing center first as fallback
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
