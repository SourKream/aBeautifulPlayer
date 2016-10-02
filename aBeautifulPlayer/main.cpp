//
//  main.cpp
//  aBeautifulPlayer
//
//  Created by Akshay Singla on 01/10/16.
//  Copyright © 2016 Shantanu Kumar. All rights reserved.
//

#include <stdio.h>
#include "MiniMaxAgent.cpp"
#include <bitset>
#include <vector>
using namespace std;

void printGameState(Game g){
    for (int r=g.gameConfig->BoardSize-1; r>-1; r--){
        for (int c=0; c<g.gameConfig->BoardSize; c++){
            cerr << "[";
            for (int k =g.Heights[g.gameConfig->BoardSize*r + c]-1; k>-1; k--)
                if ((g.Stacks[g.gameConfig->BoardSize*r + c] & (1<<k)) != 0)
                    cerr << "W";
                else
                    cerr << "B";
            cerr << "]\t";
        }
        cerr << endl;
    }
}

void printState(uint64 X){
    for (int i = 4 ; i >=0  ; i--){
        for (int j = 0 ; j < 5; j++){
            int r = i*5 + j;
            if (X & ( 1ULL << r))
                cerr << "1 ";
            else
                cerr << "0 ";
            
        }
        cerr << endl;
    }
}

vector<vector<short> > getSplits (short num){
    vector<vector<short> > splits;
    
    if (num==0){
        vector<short> split;
        splits.push_back(split);
        return splits;
    }
    
    int i = 1;
    while (i<=num){
        vector<vector<short> > A = getSplits(num-i);
        for (int j=0; j<A.size(); j++){
            A[j].push_back(i);
            splits.push_back(A[j]);
        }
        i++;
    }
    return splits;
}

vector<vector<vector<short> > > GenerateAllSlides(short K) {
    vector<vector<vector<short> > > slides(8);
    vector<vector<vector<short> > > slides1(8);
    
    for (short s = 1; s <= K; s++ ){
        slides[s] = getSplits(s);
        for (int i = 0; i < slides[s].size(); i++)
            slides[s][i].push_back(s);
    }
    
    for (int s = 1; s <= 7; s++ ){
        for (int j = 0; j < slides[s].size(); j++){
            slides1[slides[s][j].size()-1].push_back(slides[s][j]);
        }
    }
    
    return slides1;
}

vector<vector<vector<short> > > Slides;
void PrintScores(double ScoresForPlayer1[], double ScoresForPlayer2[]){
    cout << endl;
    cerr << endl;
    cout << "Playing Game(Scores)  White Black \n";
    cout << "FlatStone Score \t" <<   ScoresForPlayer1[0] << "\t" << ScoresForPlayer2[0] << endl;
    cout << "StandingStone Score \t" <<  ScoresForPlayer1[1] << "\t" << ScoresForPlayer2[1] << endl;
    cout << "CapStoneScore Score \t" << ScoresForPlayer1[2] << "\t" << ScoresForPlayer2[2] << endl;
    cout << "CenterScore Score \t" << ScoresForPlayer1[3] << "\t" << ScoresForPlayer2[3] << endl;
    cout << "StackHeightScore Score \t" << ScoresForPlayer1[4] << "\t" << ScoresForPlayer2[4] << endl;
    cout << "InfluenceScore Score \t" << ScoresForPlayer1[5] << "\t" << ScoresForPlayer2[5] << endl;
    cout << "GroupSizeScore Score \t" << ScoresForPlayer1[6] << "\t" << ScoresForPlayer2[6] << endl;
    
    cerr << endl;
    cerr << "Playing Game(Scores)  White Black \n";
    cerr << "FlatStone Score \t" <<   ScoresForPlayer1[0] << "\t" << ScoresForPlayer2[0] << endl;
    cerr << "StandingStone Score \t" <<  ScoresForPlayer1[1] << "\t" << ScoresForPlayer2[1] << endl;
    cerr << "CapStoneScore Score \t" << ScoresForPlayer1[2] << "\t" << ScoresForPlayer2[2] << endl;
    cerr << "CenterScore Score \t" << ScoresForPlayer1[3] << "\t" << ScoresForPlayer2[3] << endl;
    cerr << "StackHeightScore Score \t" << ScoresForPlayer1[4] << "\t" << ScoresForPlayer2[4] << endl;
    cerr << "InfluenceScore Score \t" << ScoresForPlayer1[5] << "\t" << ScoresForPlayer2[5] << endl;
    cerr << "GroupSizeScore Score \t" << ScoresForPlayer1[6] << "\t" << ScoresForPlayer2[6] << endl;
    
}

class GameManager{
    public :
    Game* myGame;
    MiniMaxAgent * player1;
    MiniMaxAgent * player2;
    
    int DepthBlack = 3;
    int DepthWhite = 3;
    GameManager(double ScoresForPlayer1[],int depthwhite, double ScoresForPlayer2[],int depthblack){
        double Scores[10];
        myGame = new Game(5,2,Scores);

        PrintScores(ScoresForPlayer1, ScoresForPlayer2);

        int n = 5;
        int t = 120;
        int p = 1;
        DepthBlack = depthblack;
        DepthWhite = depthwhite;
        player1 = new MiniMaxAgent(p, n, t, ScoresForPlayer1, DepthWhite);
        player2 = new MiniMaxAgent(p+1, n, t, ScoresForPlayer2, DepthBlack);
    }
    
    int Play(){
        string move;
        move = player1->GiveFirstMove();
        myGame->applyMove(myGame->makeMove(move,true));
        player2->ApplyMove(move,true);
        
        move = player2->GiveFirstMove();
        myGame->applyMove(myGame->makeMove(move,true));
        player1->ApplyMove(move,true);
        int moves = 2;
        bool currentPlayer = 1;
        int r;
        while (moves < 200){
            moves ++;
            if (currentPlayer) {
                move = player1->PlayMove();
                player2->ApplyMove(move);
            }
            else{
                move = player2->PlayMove();
                player1->ApplyMove(move);
            }
            myGame->applyMove(myGame->makeMove(move));
//            printGameState(*myGame);
//            printGameState(*(player1->myGame));
//            printGameState(*(player2->myGame));
            if ( (r = myGame->isFinishState()) != -1){
                printGameState(*myGame);
                switch(r){
                    case 0:
                        cout << "Black Wins (Depth = "  << DepthBlack;
                        cerr << "Black Wins (Depth = "  << DepthBlack;
                        break;
                    case 1:
                        cout << "White Wins (Depth = " << DepthWhite;
                        cerr << "White Wins (Depth = " << DepthWhite;
                        break;
                    case 2:
                        cout << "Both Win (DepthWhite = "  << DepthWhite << " DepthBlack = "<< DepthBlack;
                        cerr << "Both Win (DepthWhite = "  << DepthWhite << " DepthBlack = "<< DepthBlack;
                    return r;
                }
                cout << ") in Total Moves : " << moves << endl;
                cerr << ") in Total Moves : " << moves << endl;
                switch(r){
                    case 0:
                        return -moves;
                    case 1:
                        return moves;
                    default :
                        return 0;
                }
            }
            currentPlayer ^= 1;
        }
        return 0;
    }
};


void doReinforcementLearning( int trials){
    double ScoresForPlayer2[] = {10,10,10,10,10,10,10};
    double ScoresForPlayer1[] = {1,1,1,1,1,1,1};
    int DepthWhite = 2;
    int DepthBlack = 3;
    double* Win;
    double* Lose;
    double LearningRate;
    for (int i =0 ; i < trials; i++){
        GameManager Player(ScoresForPlayer1,DepthWhite, ScoresForPlayer2, DepthBlack);
        int ret = Player.Play();
        if ( ret < 0){
            LearningRate = 1.0/ret;
            LearningRate = -LearningRate;
            Win = ScoresForPlayer2;
            Lose = ScoresForPlayer1;
        }
        else if ( ret > 0){
            LearningRate = 1.0/ret;
            Win = ScoresForPlayer2;
            Lose = ScoresForPlayer1;
        }
        else{
            return;
        }
        for (int j =0 ;j < 7;j++){
            double grad = (Win[j] - Lose[j])*LearningRate + LearningRate;
            Win[j] += grad;
            Lose[j] -= grad;
            Win[j] =  max(0.0,min(10.0,Win[j]));
            Lose[j] = max(0.0,min(10.0,Lose[j]));
        }
    }
    PrintScores(ScoresForPlayer1, ScoresForPlayer2);
}

int main(){
    
    Slides = GenerateAllSlides(5);
    doReinforcementLearning(2);

//    Game myGame(5, 2);
//    myGame.applyMove(myGame.makeMove("Fe4",true));
//    myGame.applyMove(myGame.makeMove("Fe1",true));
//    myGame.applyMove(myGame.makeMove("Fd4"));
//    
//    myGame.applyMove(myGame.makeMove("Fb2"));
//    myGame.applyMove(myGame.makeMove("Fb1"));
//    myGame.applyMove(myGame.makeMove("Fa2"));
//    myGame.applyMove(myGame.makeMove("Fd2"));
//    myGame.applyMove(myGame.makeMove("Fc2"));
//    myGame.applyMove(myGame.makeMove("Cd1"));
//    myGame.applyMove(myGame.makeMove("Fa1"));
//    myGame.applyMove(myGame.makeMove("Fb1"));

    //    myGame.applyMove(myGame.makeMove("Fd3"));
    //    myGame.applyMove(myGame.makeMove("Fe3"));
    //    myGame.applyMove(myGame.makeMove("Fa4"));
    //    myGame.applyMove(myGame.makeMove("Fb4"));
    //    myGame.applyMove(myGame.makeMove("Fc4"));
    //    myGame.applyMove(myGame.makeMove("Fd4"));
    //    myGame.applyMove(myGame.makeMove("Fe4"));
    //    myGame.applyMove(myGame.makeMove("Fa5"));
    //
    //    myGame.applyMove(myGame.makeMove("Fb5"));
    //    myGame.applyMove(myGame.makeMove("Fc5"));
    //    myGame.applyMove(myGame.makeMove("Fd5"));
    //    myGame.applyMove(myGame.makeMove("Fe5"));
    //    myGame.applyMove(myGame.makeMove("1b1+1"));
    //    myGame.applyMove(myGame.makeMove("1d2<1"));
    //
//    printGameState(myGame);
    //    Game State Test
    //    myGame.printState(myGame.WhitePieces);
    //    cout << endl;
    //    myGame.printState(myGame.BlackPieces);
    //    cout << endl;
    //    myGame.printState(myGame.CapStones);
    //    cout << endl;
    //    myGame.printState(myGame.Standing);
    //    cout << endl;
    //
    //    myGame.FindComponents();
    //
    
    //    vector<Move> allMoves = myGame.generateAllMoves();
    //    allMoves.size();
//    MiniMaxAgent player(2,5,150);
//    player.myGame = &myGame;
//    cout << "My Move : " << player.getMiniMaxMove() << endl;
    
    //    Move x = myGame.makeMove("1b1+1");
    //        printGameState(myGame);
    //        cerr << endl;
    //    myGame.applyMove(x);
    //    printGameState(myGame);
    //    cerr << endl;
    
    //    myGame.applyMove(myGame.makeMove("1a1+1"));
    //    myGame.applyMove(myGame.makeMove("1d1+1"));
    //    myGame.applyMove(myGame.makeMove("1c1+1"));
    //    myGame.applyMove(myGame.makeMove("1e2+1"));
    //    myGame.applyMove(myGame.makeMove("1a3+1"));
    //    myGame.applyMove(myGame.makeMove("1e4+1"));
    //    myGame.applyMove(myGame.makeMove("Fd5"));
    //    myGame.applyMove(myGame.makeMove("Fd4"));
    //    myGame.applyMove(myGame.makeMove("Fe5"));
    
    //    printGameState(myGame);
    //    myGame.FindComponents();
    //    cout <<  myGame.checkIfRoadExists() << endl;
    //    auto X = myGame.;
    //    Game State Test
    //    myGame.printState(myGame.WhitePieces);
    //    cout << endl;
    //    myGame.printState(myGame.BlackPieces);
    //    cout << endl;
    //    myGame.printState(myGame.CapStones);
    //    cout << endl;
    //    myGame.printState(myGame.Standing);
    //    cout << endl;
    //
    //    myGame.FindComponents();
    //
    //
    //    MiniMaxAgent player(1,5,200);
    //    player.myGame = &myGame;
    //    cout << "My Move : " << player.getMiniMaxMove() << endl;
    
   // srand(time(NULL));
    //    int FlatScore = 6;
//    int StandingStoneScore = 1;
//    int CapStoneScore = 2;
//    int CenterScore = 2;
//    int StackHeightScore = 2;
//    int InfluenceScore = 3;
//    int GroupSizeScore = 0;
//    double Scores[] = {6,1,2,2,2,3,0};
//    int MaxDepth = 5;
//    int p, n, t;
//    cin >> p >> n >> t;
//    MiniMaxAgent player3(p, n, t,Scores, MaxDepth);
//    cerr << "MiniMax Player With Bits" << endl;
//    player3.playFirstMove();
//    player3.play();
    
    return 0;
}
