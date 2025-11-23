// use flags -DSERVER for server and -DCLIENT for client
// use flag -DBOT to play against bot
#include "../include/board.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef SERVER
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 9001
#endif

#ifdef CLIENT
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 9001
#endif

#ifdef SERVER
int start_server() {
    int sockfd, clientfd;
    struct sockaddr_in serv, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(PORT);
    bind(sockfd, (struct sockaddr*)&serv, sizeof(serv));
    listen(sockfd, 1);
    printf("Waiting for a client to connect...\n");
    socklen_t clilen = sizeof(cli);
    clientfd = accept(sockfd, (struct sockaddr*)&cli, &clilen);
    printf("Client connected!\n");
    return clientfd;
}
#endif

#ifdef CLIENT
int start_client(const char* server_ip) {
    int sockfd;
    struct sockaddr_in serv;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    inet_pton(AF_INET, server_ip, &serv.sin_addr);
    printf("Connecting to server...\n");
    connect(sockfd, (struct sockaddr*)&serv, sizeof(serv));
    printf("Connected to server!\n");
    return sockfd;
}
#endif

#ifdef SERVER
void send_int(int sock, int value) { send(sock, &value, sizeof(value), 0); }
int recv_int(int sock) { int val; recv(sock, &val, sizeof(val), 0); return val; }
#endif

#ifdef CLIENT
void send_int(int sock, int value) { send(sock, &value, sizeof(value), 0); }
int recv_int(int sock) { int val; recv(sock, &val, sizeof(val), 0); return val; }
#endif

int main() {
    srand(time(NULL));
    char board[ROWS][COLS];
    char playerA = 'A', playerB = 'B';
    int col, turn = 0, moves = 0;

    createBoard(board);
    printBoard(board);

#ifdef BOT
    int opponent = -1;

    int valid_choices[] = {1, 2, 3};
    int found;

    printf("Choose your bot difficulty:\n");
    printf("  1 - Easy bot\n");
    printf("  2 - Medium bot\n");
    printf("  3 - Hard bot\n");

    while (1) {
        printf("Enter your choice: ");
        if (scanf("%d", &opponent) != 1) {
            printf("Invalid input. Please type 1, 2, or 3.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        found = 0;
        for (int i = 0; i < 3; i++) {
            if (opponent == valid_choices[i]) {
                found = 1;
                break;
            }
        }

        if (found) break;
        printf("Invalid choice. Please enter 1, 2, or 3.\n");
    }
#elif defined(SERVER) || defined(CLIENT)
    int sock;
#endif


#ifdef SERVER
    sock = start_server();
#elif defined(CLIENT)
    char server_ip[16];
    printf("Enter server IP: ");
    scanf("%15s", server_ip);
    sock = start_client(server_ip);
#endif

    while (1) {
        char currentPlayer = (turn == 0) ? playerA : playerB;

#ifdef BOT
        if (turn == 1) {
            // Use selected bot difficulty
            if (opponent == 1) {
                col = bot1(board);
                printf("Bot1 chose column %d\n", col + 1);
            } else if (opponent == 2) {
                col = bot2(board, moves);
                printf("Bot2 chose column %d\n", col + 1);
            } else {
                col = bot3(board, moves, playerA);
                printf("Bot3 chose column %d\n", col + 1);
            }
        } else {
            printf("Player %c, choose a column (1-%d): ", currentPlayer, COLS);
            if (scanf("%d", &col) != 1) { while(getchar() != '\n'); continue; }
            while(getchar() != '\n');
            col -= 1; // adjust for 0-based indexing
        }
#elif defined(SERVER)
        if (turn == 0) {
            // server human
            printf("Player %c, choose a column (1-%d): ", currentPlayer, COLS);
            if (scanf("%d", &col) != 1) { while(getchar() != '\n'); continue; }
            while(getchar() != '\n');
            col -= 1; // adjust for 0-based indexing
            send_int(sock, col);
        } else {
            // receive move from client
            printf("Waiting for client move...\n");
            col = recv_int(sock);
            printf("Client chose column %d\n", col + 1);
        }
#elif defined(CLIENT)
        if (turn == 0) {
            // receive move from server
            printf("Waiting for server move...\n");
            col = recv_int(sock);
            printf("Server chose column %d\n", col + 1);
        } else {
            // client human
            printf("Player %c, choose a column (1-%d): ", currentPlayer, COLS);
            if (scanf("%d", &col) != 1) { while(getchar() != '\n'); continue; }
            while(getchar() != '\n');
            col -= 1; // adjust for 0-based indexing
            send_int(sock, col);
        }
#else
        // Local two-player mode
        printf("Player %c, choose a column (1-%d): ", currentPlayer, COLS);
        if (scanf("%d", &col) != 1) { while(getchar() != '\n'); continue; }
        while(getchar() != '\n');
        col -= 1; // adjust for 0-based indexing
#endif

        if (dropChecker(board, col, currentPlayer)) {
            moves++;
            printBoard(board);
            if (checkWin(board, col, currentPlayer)) {
                printf("Player %c wins!\n", currentPlayer);
#ifdef SERVER
                close(sock);
#endif
#ifdef CLIENT
                close(sock);
#endif
                break;
            }
            if (moves == ROWS*COLS) {
                printf("It's a tie! Board full.\n");
#ifdef SERVER
                close(sock);
#endif
#ifdef CLIENT
                close(sock);
#endif
                break;
            }
            turn = 1 - turn;
        } else {
            printf("Column %d is full. Try another column.\n", col+1);
        }
    }
    return 0;
}
