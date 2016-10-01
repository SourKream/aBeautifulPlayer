//
//  MiniMaxAgent.cpp
//  aBeautifulPlayer
//
//  Created by Shantanu Kumar on 01/10/16.
//  Copyright © 2016 Shantanu Kumar. All rights reserved.
//

#include <stdio.h>
#include "Game.cpp"

struct MiniMaxAgent{
    
    Game* myGame;
    int myPlayerNumber;
    int boardSize;
    int timeLimit;
    
    MiniMaxAgent (int playerNum, int n, int t){
        myPlayerNumber = playerNum;
        boardSize = n;
        timeLimit = t;
        myGame = new Game(boardSize, myPlayerNumber);
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
        string move;
        
        if (myPlayerNumber == 2){
            cin >> move;
            myGame->applyMove(myGame->makeMove(move));
        }
        
        
        while(true){
            move = getMiniMaxMove();
            myGame->applyMove(myGame->makeMove(move));
            cerr << "I am playing : " << move << endl;
            cout << move << endl;
            
            cin >> move;
            myGame->applyMove(myGame->makeMove(move));
        }
        //cout << move << endl;
    }
    
    string getMiniMaxMove(){
        
        vector<Move> allMoves = myGame->generateAllMoves();
        int maxStateValue = -2000;
        int alpha = -1000, beta = 1000;
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
            
            if (beta <= alpha)
                break;
        }
        
        return myGame->getMoveString(bestMove);
    }
    
    int MiniMaxSearch (Game gameState, bool maximize, int depth, int alpha, int beta){
        //cout << depth<< endl;
        //  count1++;
        int winner = gameState.isFinishState();
        if ((winner!=-1)||(depth>3))
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