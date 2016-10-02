//
//  MiniMaxAgent.cpp
//  aBeautifulPlayer
//
//  Created by Shantanu Kumar on 01/10/16.
//  Copyright © 2016 Shantanu Kumar. All rights reserved.
//

#include <stdio.h>
#include "Game.cpp"

#define INF 5000

struct MiniMaxAgent{
    
    Game* myGame;
    int myPlayerNumber;
    string myPlayer;
    int boardSize;
    int timeLimit;
    int maxDepth = 4;
    
    MiniMaxAgent (int playerNum, int n, int t,double Scores [], int MaxDepthIn){
        myPlayerNumber = playerNum;
        myPlayer = "Black";
        if (playerNum == 1)
            myPlayer = "White";
        boardSize = n;
        timeLimit = t;
        maxDepth = MaxDepthIn;
        myGame = new Game(boardSize, myPlayerNumber,Scores);
    }
    
    void ApplyMove(string moveString, bool opponent = false){
        myGame->applyMove(myGame->makeMove(moveString, opponent));
    }
    
    string GiveFirstMove(){
        string moveString;
        Move move;
        vector<Move> allMoves = myGame->generateFirstMove();
        move = allMoves[rand()%allMoves.size()];
        moveString = myGame->getMoveString(move);
        cerr << "My First Move as " << myPlayer << " Player:" << moveString << endl;
        myGame->applyMove(move);
        return moveString;
    }
    
    void playFirstMove(){
        string moveString;
        Move move;
        
        if (myPlayerNumber == 2){
            cin >> moveString;
            myGame->applyMove(myGame->makeMove(moveString, true));
            
            vector<Move> allMoves = myGame->generateFirstMove();
            move = allMoves[rand()%allMoves.size()];
            moveString = myGame->getMoveString(move);
            
            myGame->applyMove(move);
            cerr << "My Move : " << moveString << endl;
            cout << moveString << endl;
            
        }
        
        if (myPlayerNumber == 1){
            
            vector<Move> allMoves = myGame->generateFirstMove();
            move = allMoves[rand()%allMoves.size()];
            moveString = myGame->getMoveString(move);
            
            myGame->applyMove(move);
            cerr << "My Move : " << moveString << endl;
            cout << moveString << endl;
            
            cin >> moveString;
            myGame->applyMove(myGame->makeMove(moveString, true));
        }
    }
    
    string PlayMove(){
        string move;

        move = getMiniMaxMove();
        myGame->applyMove(myGame->makeMove(move));

        cerr << "My Move as " << myPlayer << " Player :" << move << endl;
        return move;
    }
    
    void play(){
        string move;
        
        if (myPlayerNumber == 2){
            cin >> move;
            myGame->applyMove(myGame->makeMove(move));
        }
        
        
        while(true){
            move = getMiniMaxMove();
            cerr << "My Move as " << myPlayer << "Player :" << move << endl;
            cout << move << endl;
            myGame->applyMove(myGame->makeMove(move));
            cin >> move;
            myGame->applyMove(myGame->makeMove(move));
        }
        //cout << move << endl;
    }
    
    string getMiniMaxMove(){
        
        vector<Move> allMoves = myGame->generateAllMoves();
        int maxStateValue = -INF;
        int alpha = -INF, beta = INF;
        Move bestMove;
        
        for (int i=0; i<allMoves.size(); i++){
            Game nextState = *myGame;
            nextState.applyMove(allMoves[i]);
            
            int value = MiniMaxSearch(nextState, false, 1, alpha, beta);
            if (value > maxStateValue){
                maxStateValue = value;
                bestMove = allMoves[i];
            }
            alpha = max(alpha, maxStateValue);
            
            if (beta < alpha)
                break;
        }
        
       // cerr << "Max State Value was  : " << maxStateValue << endl;
        return myGame->getMoveString(bestMove);
    }
    
    int MiniMaxSearch (Game &gameState, bool maximize, int depth, int alpha, int beta){
        //cout << depth<< endl;
        
        int winner = gameState.isFinishState();
        if ((winner!=-1)||(depth>maxDepth))
            return gameState.getStateValue();
        
        vector<Move> allMoves = gameState.generateAllMoves();
        // cout << allMoves.size() << endl;
        int bestValue = INF;
        if (maximize)
            bestValue = -INF;
        
        for (int i=0; i<allMoves.size(); i++){
            Game nextState(gameState);
            nextState.applyMove(allMoves[i]);
            int value = MiniMaxSearch(nextState, !maximize, depth+1, alpha, beta);
            if (maximize){
                bestValue = max(bestValue, value);
                alpha = max(alpha, bestValue);
                if (beta < alpha)
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
