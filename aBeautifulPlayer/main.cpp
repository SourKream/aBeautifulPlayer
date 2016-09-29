//
//  main.cpp
//  aBeautifulPlayer
//
//  Created by Shantanu Kumar on 23/09/16.
//  Copyright © 2016 Shantanu Kumar. All rights reserved.
//

#include <iostream>
#include <vector>
#include <unordered_set>
#include <random>
#include <sstream>
#include <stdio.h>
#include <unistd.h>

using namespace std;

#define MAX_BOARD_SIZE 7

stringstream input;
int count1 = 0;
stringstream output;

enum Colour {Black, White};
enum PieceType {Flat, Wall, Cap};
enum Direction {Up = 0, Down, Left, Right};

struct Piece{
    Colour colour;
    PieceType type;
    
    void printPiece(){
        switch(colour){
            case White: cerr << "w";
                break;
            case Black: cerr << "b";
                break;
        }
        switch(type){
            case Flat: cerr << "F ";
                break;
            case Wall: cerr << "W ";
                break;
            case Cap: cerr << "C ";
                break;
        }
    }
};

struct Player{
    
    int numFlats;
    int numCaps;
    Colour colour;
    
    Player (int initFlats, int initCaps, Colour playerColour){
        numFlats = initFlats;
        numCaps = initCaps;
        colour = playerColour;
    }
    
    Player(){
        
    }
};

struct Game{

    vector<Piece> board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    bool myPieces[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    bool opponentPieces[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    bool OccupiedBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    
    Player players[2];
    int FlatsOnTop[2];
    int maxFlats;
    int maxCaps;
    int boardSize;
    int numSquares;
    
    bool currentPlayer;
    
    Game (int n, int myPlayerNumber){
        
        boardSize = n;
        numSquares = n*n;
        currentPlayer = myPlayerNumber - 1;
        
        // Set params of board size
        switch(boardSize){
            case 5: maxFlats = 21;
                maxCaps = 1;
                break;
            case 6: maxFlats = 30;
                maxCaps = 1;
                break;
            case 7: maxFlats = 40;
                maxCaps = 1;
                break;
        }

        // Initialise Players
        Player p1(maxFlats, maxCaps, White), p2(maxFlats, maxCaps, Black);
        if (myPlayerNumber == 1){
            players[0] = p1;
            players[1] = p2;
        } else {
            players[0] = p2;
            players[1] = p1;
        }
    }
    
    void place(Piece piece, int c, int r){
        board[r][c].push_back(piece);
        OccupiedBoard[r][c] = 1;
        FlatsOnTop[0] -= myPieces[r][c];
        FlatsOnTop[1] -= opponentPieces[r][c];
        if (piece.type != Wall){
            if ( players[0].colour == piece.colour){
                myPieces[r][c] = 1;
                FlatsOnTop[0] += 1;
                opponentPieces[r][c]= 0;
            }
            else{
                opponentPieces[r][c] = 1;
                FlatsOnTop[1] += 1;
                myPieces[r][c] = 0;
            }
        }
        
    }
    
    void moveStack(int numPieces, int c, int r, Direction dir, vector<int> drops){
        
        vector<Piece> pickupStack;
        for (int i=0; i<numPieces; i++){
            pickupStack.push_back(board[r][c].back());
            board[r][c].pop_back();
        }
        
        FlatsOnTop[0] -= myPieces[r][c];
        FlatsOnTop[1] -= opponentPieces[r][c];
        if (board[r][c].size() == 0){
            OccupiedBoard[r][c] = 0;
            myPieces[r][c] = 0;
            opponentPieces[r][c] = 0;
        }
        else{
            if (board[r][c].back().colour == players[0].colour){
                myPieces[r][c] = 1;
                opponentPieces[r][c] = 0;
                FlatsOnTop[0] += 1;
            }
            else{
                myPieces[r][c] = 0;
                opponentPieces[r][c] = 1;
                FlatsOnTop[1] += 1;
            }
        }

        
        int dr = 0;
        int dc = 0;
        switch(dir){
            case Up: dr = -1;
                break;
            case Down: dr = 1;
                break;
            case Right: dc = 1;
                break;
            case Left: dc = -1;
                break;
        }
        int i = 0;
        
        while(numPieces>0){
            r += dr;
            c += dc;
            if (pickupStack.back().type == Cap)
                if (board[r][c].size()>0 && board[r][c].back().type == Wall)
                        board[r][c].back().type = Flat;
            for (int j=0; j<drops[i]; j++){
                place(pickupStack.back(),c,r);
                pickupStack.pop_back();
                numPieces--;
            }
            i++;
        }
    }
    
    void applyMove (string move){
        if (isalpha(move[0])){
            Piece piece;
            piece.colour = players[currentPlayer].colour;
            switch(move[0]){
                case 'F': piece.type = Flat;
                    break;
                case 'S': piece.type = Wall;
                    break;
                case 'C': piece.type = Cap;
                    break;
            }
            place(piece, move[1]-'a', move[2]-'1');
            if (move[0]=='C')
                players[currentPlayer].numCaps--;
            else
                players[currentPlayer].numFlats--;
            currentPlayer ^= 1;
        } else if (isnumber(move[0])) {
            Direction dir = Up;
            vector<int> drops;

            switch (move[3]){
                case '-': dir = Up;
                    break;
                case '+': dir = Down;
                    break;
                case '>': dir = Right;
                    break;
                case '<': dir = Left;
                    break;
            }
            
            for (int i=4; i<move.size(); i++)
                drops.push_back(move[i]-'0');
            
            moveStack(move[0]-'0', move[1]-'a', move[2]-'1', dir, drops);
            currentPlayer ^= 1;
        }
    }
    
    void applyMoveOnOpponent (string move){
        Piece piece;
        piece.colour = players[!currentPlayer].colour;
        switch(move[0]){
            case 'F': piece.type = Flat;
                break;
            case 'S': piece.type = Wall;
                break;
            case 'C': piece.type = Cap;
                break;
        }
        place(piece, move[1]-'a', move[2]-'1');
        if (move[0]=='C')
            players[1-currentPlayer].numCaps--;
        else
            players[1-currentPlayer].numFlats--;

        currentPlayer ^= 1;
    }
    
    //////////////////////////////////////////////////////
    // Utility Function
    int get1DIndex (int c, int r){
        return boardSize * c + r;
    }
    
    int get1DIndex (string s){
        return boardSize * (s[0]-'a') + (s[1]-'1');
    }

    pair<int,int> get2DIndex (int index){
        pair<int, int> A(index/boardSize,index%boardSize);
        return A;
    }

    pair<int,int> get2DIndex (string s){
        return get2DIndex(get1DIndex(s));
    }

    string getStringIndex (int index){
        string s;
        s += ('a' + index/boardSize);
        s += ('1' + index%boardSize);
        return s;
    }

    string getStringIndex (int c, int r){
        return getStringIndex(get1DIndex(c, r));
    }
    
    vector<vector<int> > getSplits (int num){
        vector<vector<int> > splits;

        if (num==0){
            vector<int> split;
            splits.push_back(split);
            return splits;
        }
        
        int i = 1;
        while (i<=num){
            vector<vector<int> > A = getSplits(num-i);
            for (int j=0; j<A.size(); j++){
                A[j].push_back(i);
                splits.push_back(A[j]);
            }
            i++;
        }
        
        return splits;
    }
    
    string getMoveString (int numPieces, int c, int r, int dir, vector<int> drops){
        string move;
        move = (numPieces+'0');
        move += getStringIndex(c,r);
        switch (dir){
            case Up: move += '-';
                break;
            case Down: move += '+';
                break;
            case Right: move += '>';
                break;
            case Left: move += '<';
                break;
        }
        for (int i=0; i<drops.size(); i++)
            move += (drops[i] + '0');
        
        return move;
    }
    //////////////////////////////////////////////////////

    vector<string> generateAllMoves(){
        vector<string> moves;
        
        // Place Moves
        if (players[currentPlayer].numFlats>0){
            for (int i = 0 ; i < boardSize ; i++){
                for (int j = 0 ; j < boardSize ; j++){
                    if (OccupiedBoard[i][j] == 0){
                        string pos = getStringIndex(j,i);
                        moves.push_back("F" + pos);
                        moves.push_back("S" + pos);
                    }
                }
            }
        }
        
        if (players[currentPlayer].numCaps>0){
            for (int i = 0 ; i < boardSize ; i++){
                for (int j = 0 ; j < boardSize ; j++){
                    if (OccupiedBoard[i][j] == 0){
                        string pos = getStringIndex(j,i);
                        moves.push_back("C" + pos);
                    }
                    
                }
            }
        }

        // Stack Moves
        for (int r = 0 ; r < boardSize ; r++){
            for (int c = 0 ; c <boardSize  && OccupiedBoard[r][c]; c++ ){
                if (board[r][c].back().colour == players[currentPlayer].colour){
                    for (int numPieces=1; numPieces<=min((int)board[r][c].size(), boardSize); numPieces++){
                        vector<vector<int> > splits = getSplits(numPieces);
                        
                        for (int j=0; j<splits.size(); j++){
                            for (int dir = 0 ; dir < 4 ; dir++){
                            if (isValidStackMove(numPieces, c, r, dir, splits[j]))
                                moves.push_back(getMoveString(numPieces, c, r, dir, splits[j]));
                            }
                        }
                    }
                }
            }
        }
        return moves;
    }
    
    vector<string> generateFirstMove(){
        vector<string> moves;
        for (int r = 0 ; r < boardSize ; r++){
            for (int c = 0 ; c <boardSize  && !OccupiedBoard[r][c]; c++ ){
                string pos = getStringIndex(c,r);
                moves.push_back("F" + pos);
            }
        }
        return moves;
    }
    
    bool isValidStackMove(int numPieces, int c, int r, int dir, vector<int> drops){
        int rr = r;
        int cc = c;
        
        int dr = 0;
        int dc = 0;
        switch(dir){
            case Up: dr = -1;
                break;
            case Down: dr = 1;
                break;
            case Right: dc = 1;
                break;
            case Left: dc = -1;
                break;
        }
        int r_lim = r + (((int)drops.size())*dr);
        int c_lim = c + (((int)drops.size())*dc);
        if (r_lim < 0 || r_lim >= boardSize || c_lim < 0 || c_lim >= boardSize)
            return false;
        
        int wallInWay = 0;
        while(wallInWay<drops.size()){
            r += dr;
            c += dc;
           
            if (board[r][c].size() != 0)
                if (board[r][c].back().type != Flat)
                    break;
            wallInWay++;
        }
        
        if (wallInWay == drops.size())
            return true;
        
        if (wallInWay == drops.size()-1)
            if (drops.back()==1)
                if (board[rr][cc].back().type == Cap)
                    if (board[r][c].back().type != Cap)
                        return true;

        return false;
    }
    
    bool checkRoadPresent (int playerNumber){
        vector<vector<int> > Graph;
        vector<int> A;
        for (int i=0; i<numSquares; i++)
            Graph.push_back(A);

        for (int r=0; r<boardSize; r++){
            for (int c=0; c<boardSize; c++){
                if (board[r][c].size()>0){
                    if (board[r][c].back().colour == players[playerNumber].colour){
                        if (board[r][c].back().type != Wall){
                            if (r > 0)
                                if (board[r-1][c].size()>0)
                                    if (board[r-1][c].back().colour == players[playerNumber].colour && board[r-1][c].back().type != Wall)
                                        Graph[get1DIndex(c, r)].push_back(get1DIndex(c, r-1));
                            if (r < boardSize-1)
                                if (board[r+1][c].size()>0)
                                    if (board[r+1][c].back().colour == players[playerNumber].colour && board[r+1][c].back().type != Wall)
                                        Graph[get1DIndex(c, r)].push_back(get1DIndex(c, r+1));
                            if (c > 0)
                                if (board[r][c-1].size()>0)
                                    if (board[r][c-1].back().colour == players[playerNumber].colour && board[r][c-1].back().type != Wall)
                                        Graph[get1DIndex(c, r)].push_back(get1DIndex(c-1, r));
                            if (c < boardSize-1)
                                if (board[r][c+1].size()>0)
                                    if (board[r][c+1].back().colour == players[playerNumber].colour && board[r][c+1].back().type != Wall)
                                        Graph[get1DIndex(c, r)].push_back(get1DIndex(c+1, r));
                        }
                    }
                }
            }
        }

        bool roadFound = false;
        for (int r=0; r<boardSize and !roadFound; r++){
            unordered_set<int> explored;
            DFS(Graph, get1DIndex(0, r), explored);
            for (int r2=0; r2<boardSize; r2++)
                if (explored.find(get1DIndex(boardSize-1, r2))!=explored.end()){
                    roadFound = true;
                    break;
                }
        }
        for (int c=0; c<boardSize and !roadFound; c++){
            unordered_set<int> explored;
            DFS(Graph, get1DIndex(c, 0), explored);
            for (int c2=0; c2<boardSize; c2++)
                if (explored.find(get1DIndex(c2, boardSize-1))!=explored.end()){
                    roadFound = true;
                    break;
                }
        }
        
        return roadFound;
    }
    
    void DFS (vector<vector<int> > Graph, int node, unordered_set<int> &explored){
        explored.insert(node);
        for (int i=0; i<Graph[node].size(); i++){
            if (explored.find(Graph[node][i])==explored.end()){
                DFS(Graph, Graph[node][i], explored);
            }
        }
    }
    
    int isFinishState(){
        // Return winner player number if finish state
        // Return -1 if not finish state
        
        if (checkRoadPresent(!currentPlayer))
            return !currentPlayer;
        
        if (checkRoadPresent(currentPlayer))
            return currentPlayer;
        
        if ((players[0].numFlats==0)||(players[1].numFlats==0)){
            if (FlatsOnTop[0] - players[0].numCaps > FlatsOnTop[1] - players[1].numCaps)
                return 0;
            else if (FlatsOnTop[0] - players[0].numCaps < FlatsOnTop[1] - players[1].numCaps)
                return 1;
            else
                return (players[0].numFlats>0)?0:1;
        }
        
        return -1;
    }
    
    int getStateValue(){
        if (checkRoadPresent(0))
            return 100;
        if (checkRoadPresent(1))
            return -100;
        
        int countFlats[2] = {};
        for (int i=0; i<boardSize; i++)
            for (int j=0; j<boardSize; j++)
                if (board[i][j].size()>0)
                    if (board[i][j].back().type!=Wall)
                        countFlats[board[i][j].back().colour]++;

        return countFlats[players[0].colour] - players[0].numFlats - countFlats[players[1].colour] + players[1].numFlats;
    }
    
    void printGameState(){
        for (int i=boardSize-1; i>-1; i--){
            for (int j=0; j<boardSize; j++){
                cerr << "[";
                for (int k=0; k<board[i][j].size(); k++)
                    board[i][j][k].printPiece();
                cerr << "]\t";
            }
            cerr << endl;
        }
    }
    
};

struct RandomAgent{
    
    Game* myGame;
    int myPlayerNumber;
    int boardSize;
    int timeLimit;

    RandomAgent (int playerNum, int n, int t){
        myPlayerNumber = playerNum;
        boardSize = n;
        timeLimit = t;
        myGame = new Game(boardSize, myPlayerNumber);
    }
    
    void playFirstMove(){
        string move;
        if (myPlayerNumber == 2){
            cin >> move;
            myGame->currentPlayer = 1;
            myGame->applyMoveOnOpponent(move);
            
            vector<string> allMoves = myGame->generateFirstMove();
            move = allMoves[rand()%allMoves.size()];
            myGame->applyMoveOnOpponent(move);
            cout << move << endl;
            
        }
        
        if (myPlayerNumber == 1){
            
            vector<string> allMoves = myGame->generateFirstMove();
            move = allMoves[rand()%allMoves.size()];
            myGame->currentPlayer = 0;
            myGame->applyMoveOnOpponent(move);
            cout << move << endl;
            
            cin >> move;
            myGame->applyMoveOnOpponent(move);
        }
    }
    
    void play(){
        string move;

        if (myPlayerNumber == 2){
            cin >> move;
            myGame->applyMove(move);
        }

        while(true){
            vector<string> allMoves = myGame->generateAllMoves();
            move = allMoves[rand()%allMoves.size()];
            myGame->applyMove(move);
            cout << move << endl;
            cerr << "I Played : " << move << endl;
            
            cin >> move;
            myGame->applyMove(move);
        }
    }
    
};

struct DFSAgent{
    
    Game* myGame;
    int myPlayerNumber;
    int boardSize;
    int timeLimit;
    
    DFSAgent (int playerNum, int n, int t){
        myPlayerNumber = playerNum;
        boardSize = n;
        timeLimit = t;
        myGame = new Game(boardSize,myPlayerNumber);
    }
    
    void playFirstMove(){
        string move;
        if (myPlayerNumber == 2){
            input >> move;
            myGame->currentPlayer = 1;
            myGame->applyMoveOnOpponent(move);

            vector<string> allMoves = myGame->generateFirstMove();
            move = allMoves[rand()%allMoves.size()];
            myGame->applyMoveOnOpponent(move);
            output << move << endl;
            
        }
        
        if (myPlayerNumber == 1){

            vector<string> allMoves = myGame->generateFirstMove();
            move = allMoves[rand()%allMoves.size()];
            myGame->currentPlayer = 0;
            myGame->applyMoveOnOpponent(move);
            output << move << endl;

            input >> move;
            myGame->applyMoveOnOpponent(move);
        }
    }
    
    void play(){
        string move;
        
        if (myPlayerNumber == 2){
            input >> move;
            myGame->applyMove(move);
        }
        

        while(true){
            move = getDFSMove();
            myGame->applyMove(move);
            output << move << endl;
            
            input >> move;
            myGame->applyMove(move);
        }
    }
    
    string getDFSMove(){

        vector<string> allMoves = myGame->generateAllMoves();
        
        int maxDepth = 1;
        while(true){
            cerr << "Trying Depth " << maxDepth << endl;
            for (int i=0; i<allMoves.size(); i++){
                Game nextState = *myGame;
                nextState.applyMove(allMoves[i]);
                if (DFS(nextState, maxDepth)){
                    return allMoves[i];
                }
            }
            maxDepth++;
        }
        return "";
    }
    
    bool DFS (Game gameState, int maxDepth){
        
        vector<Game> DFSStack;
        DFSStack.push_back(gameState);
        
        while(DFSStack.size()>0){
            
            gameState = DFSStack.back();
            DFSStack.pop_back();
            
            int winner = gameState.isFinishState();
            if (winner!=-1){
                if (winner==myGame->currentPlayer)
                    return true;
                return false;
            }
            
            vector<string> allMoves = gameState.generateAllMoves();
            if (DFSStack.size() < maxDepth * 50)
                for (int i=0; i<allMoves.size(); i++){
                    Game nextState = gameState;
                    nextState.applyMove(allMoves[i]);
                    DFSStack.push_back(nextState);
                }
        }
        
        return false;
    }
    
};

struct MiniMaxAgent{
    
    Game* myGame;
    int myPlayerNumber;
    int boardSize;
    int timeLimit;
    
    MiniMaxAgent (int playerNum, int n, int t){
        myPlayerNumber = playerNum;
        boardSize = n;
        timeLimit = t;
        myGame = new Game(boardSize,myPlayerNumber);
    }
    
    void playFirstMove(){
        string move;
        if (myPlayerNumber == 2){
            cin >> move;
            myGame->applyMoveOnOpponent(move);
            
            vector<string> allMoves = myGame->generateFirstMove();
            move = allMoves[rand()%allMoves.size()];
            myGame->applyMoveOnOpponent(move);
            cout << move << endl;
            
        }
        
        if (myPlayerNumber == 1){
            
            vector<string> allMoves = myGame->generateFirstMove();
            move = allMoves[rand()%allMoves.size()];
            myGame->applyMoveOnOpponent(move);
            cout << move << endl;
            
            cin >> move;
            myGame->applyMoveOnOpponent(move);
        }
    }
    
    void play(){
        string move;// = getMiniMaxMove();
        
        if (myPlayerNumber == 2){
            cin >> move;
            myGame->applyMove(move);
        }
        
        
        while(true){
            move = getMiniMaxMove();
            myGame->applyMove(move);
            cerr << "I am playing : " << move << endl;
            cout << move << endl;
            
            cin >> move;
            myGame->applyMove(move);
        }
        //cout << move << endl;
    }
    
    string getMiniMaxMove(){
        
        vector<string> allMoves = myGame->generateAllMoves();
        int maxStateValue = -2000;
        int alpha = -1000, beta = 1000;
        string bestMove = "YOLO";
        
        for (int i=0; i<allMoves.size(); i++){
            Game nextState = *myGame;
            nextState.applyMove(allMoves[i]);
            
            int value = MiniMaxSearch(nextState, false, 1, alpha, beta);
            if (value > maxStateValue){
                maxStateValue = value;
                bestMove = allMoves[i];
            }
            alpha = max(alpha, maxStateValue);
            
            if (beta <= alpha)
                break;
        }
        
        if (bestMove=="YOLO"){
            cerr << "Here : "  << allMoves.size() << "OKay?"<< endl;
        }
        return bestMove;
    }
    
    int MiniMaxSearch (Game gameState, bool maximize, int depth, int alpha, int beta){
       //cout << depth<< endl;
      //  count1++;
        int winner = gameState.isFinishState();
        if ((winner!=-1)||(depth>2))
            return gameState.getStateValue();
        
        vector<string> allMoves = gameState.generateAllMoves();
       // cout << allMoves.size() << endl;
        int bestValue = 0;
        if (maximize)
            bestValue = -200;
        else
            bestValue = 200;
        
        for (int i=0; i<allMoves.size(); i++){
            Game nextState = gameState;
            nextState.applyMove(allMoves[i]);
            int value = MiniMaxSearch(nextState, !maximize, depth+1, alpha, beta);
            if (maximize){
                bestValue = max(bestValue, value);
                alpha = max(alpha, bestValue);
                if (beta <= alpha)
                    break;
            }
            else {
                bestValue = min(bestValue, value);
                beta = min(beta, bestValue);
                if (beta < alpha)
                    break;
            }
        }

        return bestValue;
    }
    
};


int main(int argc, const char * argv[]) {

    int p, n, t;
    cin >> p >> n >> t;
//    p = 2;
//    n = 5;
//    t= 120;
//  //  RandomAgent player1(p, n, t);
//    Game game(5,p);

    MiniMaxAgent player2(p, n, t);
   // player2.myGame = &game;

    
//    game.applyMove("Fd5");
//    game.applyMove("Sd4");
//    game.applyMove("Fe5");
//    game.applyMove("Sc3");
//    game.printGameState();
//    
//    player2.play();
//    
//    cout << "Count : " << count1 << endl;
    
//    if (fork() == 0){
//
//        player2.play();
//    }
//    else{
         MiniMaxAgent player3(p, n, t);
//        player3.playFirstMove();
//        player3.play();
//    }
    if (argc == 2){
        if (argv[1][0] == '1'){
            cerr << "DFS Player" << endl;
            player2.playFirstMove();
            player2.play();
        } else if (argv[1][0] == '2'){
            cerr << "MiniMax Player" << endl;
            player3.playFirstMove();
            player3.play();
        }
    }
/*
    game.applyMove("Sa3");
    game.applyMove("Fc4");
    game.currentPlayer = 0;
    
    game.applyMove("Fd3");
    game.applyMove("Fe3");
    game.applyMove("Fe4");
    game.applyMove("Fc1");
    game.applyMove("Fe1");
    game.applyMove("Cb1");
    game.applyMove("Fe2");
    game.applyMove("1a3+1");
    game.printGameState();
    cout << "----------------\n";

    DFSAgent player(1,5,200);
    cout << "My Move : " << player.getDFSMove() << endl;
*/
    return 0;
}
