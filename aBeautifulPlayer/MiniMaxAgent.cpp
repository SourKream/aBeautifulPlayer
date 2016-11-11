//
//  MiniMaxAgent.cpp
//  aBeautifulPlayer
//
//  Created by Shantanu Kumar on 01/10/16.
//  Copyright © 2016 Shantanu Kumar. All rights reserved.
//

#include <stdio.h>
#include <sys/time.h>
#include "Game.cpp"

#define INF 50000


#define LOSE_DEPTH 1
#define MID_DEPTH 2
#define MAX_DEPTH 3
struct MiniMaxAgent{
    
    Game* myGame;
    timeval lasttimenoted;
    timeval currenttime;
    int myPlayerNumber;
    string myPlayer;
    int boardSize;
    int CurrentMaxDepth;
    int maxDepthAllowed = 4;
    bool ROAD_WIN = false;
    bool FLAT_WIN = false;
    bool LOST_FLAG = false;
    bool WIN_FLAG = false;
    bool ONE_STEP_FLAG = false;
    bool GO_TO_LOWEST_DEPTH = false;
    short Win_Iterations = 0;
    short Lost_Iterations = 0;
    double timeLeft;
    short int moves;
    
    MiniMaxAgent (int playerNum, int n, double t,double Scores [], int MaxDepthIn,timeval* starttime =NULL ){
        myPlayerNumber = playerNum % 2;
        if (starttime)
            lasttimenoted = *starttime;
        else
            gettimeofday(&lasttimenoted,NULL);
        myPlayer = "Black";
        if (playerNum == 1)
            myPlayer = "White";
        boardSize = n;
        timeLeft = t;
        maxDepthAllowed = MaxDepthIn;
        CurrentMaxDepth = 2;
        moves = 0;
        myGame = new Game(boardSize, myPlayerNumber,Scores);
    }
    
    void playFirstMove(){
        string moveString;
        Move move;
        moves++;
        if (myPlayerNumber == 0){
            cin >> moveString;
            gettimeofday(&lasttimenoted,NULL);
            myGame->applyMove(myGame->makeMove(moveString, true));
            
            vector<Move> allMoves = myGame->generateFirstMove();
            move = allMoves[rand()%allMoves.size()];
            moveString = myGame->getMoveString(move);
            
            myGame->applyMove(move);
            cout << moveString << endl;
            // // cerr << "My Move : " << moveString << endl;
            TimeandDepth();
        }
        
        if (myPlayerNumber == 1){
            
            vector<Move> allMoves = myGame->generateFirstMove();
            move = allMoves[rand()%allMoves.size()];
            moveString = myGame->getMoveString(move);
            
            myGame->applyMove(move);
            // // cerr << "My Move : " << moveString << endl;
            cout << moveString << endl;
            TimeandDepth();
            // // cerr << "Time left : " << timeLeft << endl;
            cin >> moveString;
            gettimeofday(&lasttimenoted,NULL);
            myGame->applyMove(myGame->makeMove(moveString, true));
        }
    }
    
    void play(){
        string move;
        
        if (myPlayerNumber == 0){
            cin >> move;
            gettimeofday(&lasttimenoted, NULL);
            myGame->applyMove(myGame->makeMove(move));
            moves++;
        }
        
        
        while(true){
            move = getMiniMaxMove();
            // // cerr << "My Move as " << myPlayer << "Player :" << move << endl;
            cout << move << endl;
            moves++;
            TimeandDepth();
            myGame->applyMove(myGame->makeMove(move));
            cin >> move;
            // // cerr << timeLeft << endl;
            gettimeofday(&lasttimenoted, NULL);
            myGame->applyMove(myGame->makeMove(move));
            moves++;
        }
        //cout << move << endl;
    }
    
    inline void TimeandDepth(){
        gettimeofday(&currenttime, NULL);
        timeLeft -= currenttime.tv_sec - lasttimenoted.tv_sec;
        timeLeft -= (currenttime.tv_usec - lasttimenoted.tv_usec)/1000000.0;
        if (GO_TO_LOWEST_DEPTH){
            CurrentMaxDepth = 1;
            GO_TO_LOWEST_DEPTH = false;
        }
        else if ( LOST_FLAG || timeLeft < 15){
            CurrentMaxDepth = LOSE_DEPTH;
            LOST_FLAG = false;
        }
        else if (WIN_FLAG || timeLeft < 75)
            CurrentMaxDepth = MID_DEPTH;
        else
            CurrentMaxDepth = MAX_DEPTH;
    }
    
    string getMiniMaxMove(){
        Move allMoves[1000];
        int size_all_moves = myGame->generateAllMoves(allMoves);

        int maxStateValue = -INF;
        int alpha = -INF, beta = INF;
        Move* bestMove = &allMoves[0];
     //   // // cerr << "Minimax with Depth " << CurrentMaxDepth << " At Move number" << moves << endl;


        for (int i=0; i<size_all_moves; i++){
            Game nextState = *myGame;
            nextState.applyMove(allMoves[i]);

            int value = MiniMaxSearch(nextState, false, 1, alpha, beta);
            if (value > maxStateValue){
                maxStateValue = value;
                bestMove = &allMoves[i];
            }
            alpha = max(alpha, maxStateValue);
            if (maxStateValue == ROAD_REWARD ){
                ROAD_WIN = true;
                break;
            }
            if (maxStateValue == ROAD_REWARD/2 ){
                FLAT_WIN = true;
            }
            if (beta < alpha)
                break;
        }
        
        if (maxStateValue < -ROAD_REWARD/2 + 1){
            LOST_FLAG = 1;
            if (Lost_Iterations++ >= 5){
                GO_TO_LOWEST_DEPTH = true;
                LOST_FLAG = 0;
                Lost_Iterations = 0;
            }
        }
        else{
            Lost_Iterations = 0;
        }
        
        if (ROAD_WIN || FLAT_WIN){
            WIN_FLAG = true;
            for (int i=0; i<size_all_moves; i++){
                Game nextState = *myGame;
                nextState.applyMove(allMoves[i]);
                int r = nextState.isFinishState();
                if (r == (myPlayerNumber)){
                    // cerr << "1 Move Win/Loss" << endl;
                    bestMove = &allMoves[i];
                    break;
                }
                if (!ROAD_WIN && FLAT_WIN && r == myPlayerNumber + 4 ){
                     // cerr << "direct flat win" << endl;
                    bestMove = &allMoves[i];
                    break;
                }
            }
        }
        
        if (WIN_FLAG && Win_Iterations++ >= 5){
            // cerr << "GOING TO LOWEST DEPTH" << endl;
            GO_TO_LOWEST_DEPTH = true;
            WIN_FLAG = false;
            Win_Iterations = 0;
        }
        else{
            Win_Iterations = 0;
        }
        
        ROAD_WIN =false;
        FLAT_WIN = false;
        //// // cerr << "ONE_STEP_FLAG : " << ONE_STEP_FLAG << endl << endl;
         cerr << "Max State Value was  : " << maxStateValue << endl;
        return myGame->getMoveString(*bestMove);
    }
    
    int MiniMaxSearch (Game &gameState, bool maximize, int depth, int alpha, int beta){

        int winner = gameState.isFinishState();
        if (winner != -1){
            if (winner == myPlayerNumber)
                return ROAD_REWARD;
            else if (winner + 4 == myPlayerNumber)
                return ROAD_REWARD/2;
            else if (winner == (myPlayerNumber + 1)%2)
                return -ROAD_REWARD;
            return -ROAD_REWARD/2;
        }
        
        if (depth>CurrentMaxDepth)
            return gameState.getStateValue();
        
        Move allMoves[1000];
        int size_all_moves = gameState.generateAllMoves(allMoves);
        int bestValue = INF;
        if (maximize)
            bestValue = -INF;
        
        for (int i=0; i<size_all_moves; i++){
            Game nextState(gameState);
            nextState.applyMove(allMoves[i]);
            int value = MiniMaxSearch(nextState, !maximize, depth+1, alpha, beta);
            if (maximize){
                bestValue = max(bestValue, value);
                alpha = max(alpha, bestValue);
                if ((beta < alpha) || (bestValue == ROAD_REWARD))
                    break;
            }
            else {
                bestValue = min(bestValue, value);
                beta = min(beta, bestValue);
                if ((beta < alpha) || ((bestValue == -ROAD_REWARD)))
                    break;
            }
        }
        
        return bestValue;
    }
    
};
