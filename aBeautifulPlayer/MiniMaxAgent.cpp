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
    
    void play(){
        string move;
        
        if (myPlayerNumber == 2){
            cin >> move;
            myGame->applyMove(myGame->makeMove(move));
        }
        
        
        while(true){
            move = getMiniMaxMove();
            myGame->applyMove(myGame->makeMove(move));
            cerr << "My Move : " << move << endl;
            cout << move << endl;
            
            cin >> move;
            myGame->applyMove(myGame->makeMove(move));
        }
        //cout << move << endl;
    }
    
    string getMiniMaxMove(){
        
        vector<Move> allMoves = myGame->generateAllMoves();
        int maxStateValue = -2000;
        int alpha = -2000, beta = 2000;
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
        
        cerr << "Max State Value was  : " << maxStateValue << endl;
        return myGame->getMoveString(bestMove);
    }
    
    int MiniMaxSearch (Game &gameState, bool maximize, int depth, int alpha, int beta){
        //cout << depth<< endl;
        
        int winner = gameState.isFinishState();
        if ((winner!=-1)||(depth>4))
            return gameState.getStateValue();
        
        vector<Move> allMoves = gameState.generateAllMoves();
        // cout << allMoves.size() << endl;
        int bestValue = 0;
        if (maximize)
            bestValue = -2000;
        else
            bestValue = 2000;
        
        for (int i=0; i<allMoves.size(); i++){
            Game nextState(gameState);
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
