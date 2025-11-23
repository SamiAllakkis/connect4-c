# connect4-c
A console-based Connect 4 game written in C. Play against friends or challenge built-in bots with different strategies. 

How to Compile
1. Local 2-player game
gcc-15 -o connect4 src/main.c src/board.c -Iinclude -fopenmp

2. Play against a bot
gcc-15 -o connect4 src/main.c src/board.c -Iinclude -fopenmp -DBOT

3. Server mode
gcc-15 -o server src/main.c src/board.c -Iinclude -fopenmp -DSERVER

4. Client mode
gcc-15 -o client src/main.c src/board.c -Iinclude -fopenmp -DCLIENT

How to Run
1. 2. Local or Bot
./connect4

3. Server
./server

4. Client
./client
