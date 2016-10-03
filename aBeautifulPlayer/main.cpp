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
void PrintScores(double ScoresForPlayer1[], double ScoresForPlayer2[], bool print = false){
    if (print){
    cout << endl;
    cout << "Playing Game(Scores)  White Black \n";
    cout << "FlatStone Score \t" <<   ScoresForPlayer1[0] << "\t" << ScoresForPlayer2[0] << endl;
    cout << "StandingStone Score \t" <<  ScoresForPlayer1[1] << "\t" << ScoresForPlayer2[1] << endl;
    cout << "CapStoneScore Score \t" << ScoresForPlayer1[2] << "\t" << ScoresForPlayer2[2] << endl;
    cout << "CenterScore Score \t" << ScoresForPlayer1[3] << "\t" << ScoresForPlayer2[3] << endl;
    cout << "StackHeightScore Score \t" << ScoresForPlayer1[4] << "\t" << ScoresForPlayer2[4] << endl;
    cout << "InfluenceScore Score \t" << ScoresForPlayer1[5] << "\t" << ScoresForPlayer2[5] << endl;
    cout << "GroupSizeScore Score \t" << ScoresForPlayer1[6] << "\t" << ScoresForPlayer2[6] << endl;
    
    }
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
    bool currentPlayer = 1;
    vector<vector<double>> Player1AverageScores;//(2,vector<double>(7));
    vector<vector<double>> Player2AverageScores;//(2,vector<double>(7));
    
    int DepthBlack = 4;
    int DepthWhite = 4;
    GameManager(double ScoresForPlayer1[],int depthwhite, double ScoresForPlayer2[],int depthblack){
        double Scores[10];
        myGame = new Game(5,2,Scores);
        int n = 5;
        int t = 1200;
        int p = 1;
        Player1AverageScores.resize(2,vector<double>(7,0));
        Player2AverageScores.resize(2,vector<double>(7,0));
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
        int r;
        while (moves < 400){
            moves ++;
            updateAverageBoard();
            if (currentPlayer) {
                move = player1->PlayMove();
                player2->ApplyMove(move);
            }
            else{
                move = player2->PlayMove();
                player1->ApplyMove(move);
            }
            myGame->applyMove(myGame->makeMove(move));
            currentPlayer ^= 1;
            
//            printGameState(*myGame);
//            printGameState(*(player1->myGame));
//            printGameState(*(player2->myGame));
            if ( (r = myGame->isFinishState()) != -1){
                updateAverageBoard();
                currentPlayer ^= 1;
                updateAverageBoard();
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
        }
        return 0;
    }
    void updateAverageBoard(){
        vector<vector<int>> current = myGame->AnalyzeBoard();
        if ( currentPlayer)
            for (int i = 0 ; i < 2; i++)
                for (int j = 0 ; j < 7 ;j++)
                    Player1AverageScores[i][j] = current[i][j] + 0.9*Player1AverageScores[i][j];
        else
            for (int i = 0 ; i < 2; i++)
                for (int j = 0 ; j < 7 ;j++)
                    Player2AverageScores[i][j] = current[i][j] + 0.9*Player2AverageScores[i][j];
    }
    
    vector<vector<double>> AnalyseBoard(){
        for (int i = 0 ; i < 7 ;i++)
            Player1AverageScores[0][i] = Player2AverageScores[0][i];
       
        return Player1AverageScores;
    }
};


void doReinforcementLearning( int trials,     double ScoresForPlayer1[] ,  double ScoresForPlayer2[] , bool Randomize = false){
    
    
    //    double FlatScore = 6;
    //    double StandingStoneScore = 1;
    //    double CapStoneScore = 2;
    //    double CenterScore = 2;
    //    double StackHeightScore = 2;
    //    double InfluenceScore = 3;
    //    double GroupSizeScore = 0;
    double Initial2[] = {10,10,10,10,10,10,10};
    double Initial1[] = {1,1,1,1,1,1,1};
    for (int i =0 ; i < 7; i++){
        //ScoresForPlayer1[i] = rand()%11;
        Initial1[i] = ScoresForPlayer1[i];
        //ScoresForPlayer2[i] = rand()%11;
        Initial2[i] = ScoresForPlayer2[i];
    }
    if (Randomize){
        for (int i =0 ; i < 7; i++){
            ScoresForPlayer1[i] = rand()%10 + 1;
            Initial1[i] = ScoresForPlayer1[i];
            ScoresForPlayer2[i] = rand()%10 + 1;
            Initial2[i] = ScoresForPlayer2[i];
        }
    }
    int DepthWhite = 3;
    int DepthBlack = 3;
    double* Win;
    double* Lose;
    double* WinAnalysis;
    double* LoseAnalysis;
    int BlackWins = 0;
    int WhiteWins = 0;
    PrintScores(Initial1, Initial2,true);
    vector<vector<double>> BoardAnalysis;
    for (int i =0 ; i < trials; i++){
        double LearningRate = 5.0;
        cout << endl << "Iteration " << i + 1 << endl;
        cerr << endl << "Iteration " << i + 1 << endl;
        PrintScores(ScoresForPlayer1, ScoresForPlayer2,true);
        GameManager Player(ScoresForPlayer1,DepthWhite, ScoresForPlayer2, DepthBlack);
        int ret = Player.Play();
        BoardAnalysis = Player.AnalyseBoard();
        if ( ret < 0){
            BlackWins++;
            LearningRate *= 1.0/ret;
            LearningRate = -LearningRate;
            Win = ScoresForPlayer2;
            Lose = ScoresForPlayer1;
            WinAnalysis = &BoardAnalysis[0][0];
            LoseAnalysis = &BoardAnalysis[1][0];
        }
        else if ( ret > 0){
            WhiteWins++;
            LearningRate *= 1.0/ret;
            Win = ScoresForPlayer1;
            Lose = ScoresForPlayer2;
            WinAnalysis = &BoardAnalysis[1][0];
            LoseAnalysis = &BoardAnalysis[0][0];
        }
        else{
            cout << "Tie Game" << endl;
           // cout << "Black Wins " << BlackWins << " White Wins " << WhiteWins << endl;
            LearningRate = 0.5;
            Win = ScoresForPlayer1;
            Lose = ScoresForPlayer2;
            WinAnalysis = &BoardAnalysis[1][0];
            LoseAnalysis = &BoardAnalysis[0][0];
           // PrintScores(ScoresForPlayer1, ScoresForPlayer2);
           // return;
        }
        for (int j =0 ;j < 7;j++){
            double grad = ((WinAnalysis[j]*Win[j] - LoseAnalysis[j]*Lose[j])/(WinAnalysis[j]*Win[j] + 1))*LearningRate + LearningRate;
         //   double grad1 = (WinAnalysis[j] - LoseAnalysis[j])*LearningRate;
            Lose[j] += grad*Lose[j] + grad;
            Lose[j] = max(0.0,min(30.0,Lose[j]));
        }
    }
    cout << endl;
    PrintScores(Initial1, Initial2,true);
    cout << endl << "Black Wins " << BlackWins << " White Wins " << WhiteWins << endl;
    PrintScores(ScoresForPlayer1, ScoresForPlayer2,true);
}

void debugGame(){
    double WhiteScores[] = {13.6703
        ,1.56367
        ,10.872
        ,3.09076
        ,6.78622
        ,7.14189
        ,12.7884};
    Game myGame(5, 2,WhiteScores);
    myGame.applyMove(myGame.makeMove("Fa5",true));
    myGame.applyMove(myGame.makeMove("Fe5",true));
    myGame.applyMove(myGame.makeMove("Fe4"));
    myGame.applyMove(myGame.makeMove("Fb4"));
    myGame.applyMove(myGame.makeMove("Fd4"));
    myGame.applyMove(myGame.makeMove("Cd3"));
    myGame.applyMove(myGame.makeMove("Cc4"));
    myGame.applyMove(myGame.makeMove("1d3+1"));
    myGame.applyMove(myGame.makeMove("Fb3"));
    myGame.applyMove(myGame.makeMove("1b4-1"));
    myGame.applyMove(myGame.makeMove("Fc5"));
    myGame.applyMove(myGame.makeMove("2d4>2"));
    myGame.applyMove(myGame.makeMove("Fc3"));
    myGame.applyMove(myGame.makeMove("Fb2"));
    myGame.applyMove(myGame.makeMove("Fc2"));
    myGame.applyMove(myGame.makeMove("1b2>1"));
    myGame.applyMove(myGame.makeMove("Fb5"));
    myGame.applyMove(myGame.makeMove("Fb2"));
    myGame.applyMove(myGame.makeMove("Sa3"));
    myGame.applyMove(myGame.makeMove("Fd3"));
    myGame.applyMove(myGame.makeMove("1a3>1"));
    myGame.applyMove(myGame.makeMove("Fa3"));
    myGame.applyMove(myGame.makeMove("Fb4"));
    myGame.applyMove(myGame.makeMove("Fa4"));
    myGame.applyMove(myGame.makeMove("Fd5"));
    myGame.applyMove(myGame.makeMove("Fa2"));
    myGame.applyMove(myGame.makeMove("2b3<2"));
    myGame.applyMove(myGame.makeMove("Fd2"));
    myGame.applyMove(myGame.makeMove("3a3-3"));
    myGame.applyMove(myGame.makeMove("3e4+3"));
    myGame.applyMove(myGame.makeMove("Fa3"));
    myGame.applyMove(myGame.makeMove("1a5>1"));
    myGame.applyMove(myGame.makeMove("Fd4"));
    myGame.applyMove(myGame.makeMove("1a4-1"));
    myGame.applyMove(myGame.makeMove("4a2>4"));
    myGame.applyMove(myGame.makeMove("4e5<112"));
    myGame.applyMove(myGame.makeMove("Fe4"));
    myGame.applyMove(myGame.makeMove("4b5-22"));
    myGame.applyMove(myGame.makeMove("2c5>2"));
    myGame.applyMove(myGame.makeMove("Sa5"));
    myGame.applyMove(myGame.makeMove("Fc5"));
    myGame.applyMove(myGame.makeMove("3b3>12"));
    myGame.applyMove(myGame.makeMove("1c3-1"));
    myGame.applyMove(myGame.makeMove("1d2<1"));
    myGame.applyMove(myGame.makeMove("1b2>1"));
    myGame.applyMove(myGame.makeMove("3b2+111"));
    myGame.applyMove(myGame.makeMove("2c2<2"));
    myGame.applyMove(myGame.makeMove("1b3>1"));
    myGame.applyMove(myGame.makeMove("3c2+3"));

    printGameState(myGame);


    int Depth = 4;
    MiniMaxAgent player(2,5,150,WhiteScores,Depth);
    player.myGame = &myGame;
    cout << "My Move : " << player.getMiniMaxMove() << endl;
//
//    Move x = myGame.makeMove("1b1+1");
//        printGameState(myGame);
//        cerr << endl;
//    myGame.applyMove(x);
//    printGameState(myGame);
//    cerr << endl;
//
//    myGame.applyMove(myGame.makeMove("1a1+1"));
//    myGame.applyMove(myGame.makeMove("1d1+1"));
//    myGame.applyMove(myGame.makeMove("1c1+1"));
//    myGame.applyMove(myGame.makeMove("1e2+1"));
//    myGame.applyMove(myGame.makeMove("1a3+1"));
//    myGame.applyMove(myGame.makeMove("1e4+1"));
//    myGame.applyMove(myGame.makeMove("Fd5"));
//    myGame.applyMove(myGame.makeMove("Fd4"));
//    myGame.applyMove(myGame.makeMove("Fe5"));
//
//    printGameState(myGame);
//    myGame.FindComponents();
//    cout <<  myGame.checkIfRoadExists() << endl;
//
//    myGame.FindComponents();
    exit(0);
}

int main(int argc, char** argv){

    srand(time(NULL));
    
    Slides = GenerateAllSlides(5);
    //debugGame();
    //doReinforcementLearning(stoi(argv[1]));

    double WhiteScores[] = {15,0.507648,12.0281,12.5767,0.602723,10.5355,14.2851};
    double BlackScores[] = {15,0.558291,11.6684,11.0022,0.58952,10.8724,15};
     doReinforcementLearning(40,WhiteScores,BlackScores,true);

//    int MaxDepth = 5;
//    int p, n;
//    double t;
//    cin >> p >> n >> t;
//    timeval currenttime;
//    gettimeofday(&currenttime, NULL);
//    if ( p == 1){
//        MiniMaxAgent player3(p, n, t,WhiteScores, MaxDepth, &currenttime);
//        cerr << "MiniMax Player With Bits" << endl;
//        player3.playFirstMove();
//        player3.play();
//    }
//    else{
//        MiniMaxAgent player3(p, n, t,BlackScores, MaxDepth, &currenttime);
//        cerr << "MiniMax Player With Bits" << endl;
//        player3.playFirstMove();
//        player3.play();
//    }
    return 0;
}
