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

vector<vector<vector<short>>> GenerateAllSlides(short K) {
    vector<vector<vector<short>>> slides(8);
    vector<vector<vector<short>>> slides1(8);
    
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

vector<vector<vector<short>>> Slides;

/*
int main(){
    Slides = GenerateAllSlides(5);

///// Slide Test
//    uint64 Z = ((1ULL << 63) | ( 1ULL << 31));
//    
//    cout << __builtin_clzl(Z) << endl;
//    cerr << __builtin_ctzl(1<<12) << endl;
//    for (int s = 1; s <= 7; s++ ){
//        for (int j = 0; j < Slides[s].size(); j++){
//            cerr << Slides[s][j].back() << " ";
//        }
//        cerr << endl;
//    }
//    
//    GetSlides x;
//    GetSlides y;
//    cerr << &x.Slides << endl;
//    cerr << &y.Slides << endl;
//////

//    Game myGame(5, 2);
//    myGame.applyMove(myGame.makeMove("Fc2",true));
//    myGame.applyMove(myGame.makeMove("Fd2",true));
//    myGame.applyMove(myGame.makeMove("Fa1"));
//    myGame.applyMove(myGame.makeMove("Fb1"));
//    myGame.applyMove(myGame.makeMove("Fc1"));
//    myGame.applyMove(myGame.makeMove("Fd1"));
//    myGame.applyMove(myGame.makeMove("Fe1"));
//    myGame.applyMove(myGame.makeMove("Fa2"));
//    myGame.applyMove(myGame.makeMove("Fb2"));
//    myGame.applyMove(myGame.makeMove("Fe2"));
//    myGame.applyMove(myGame.makeMove("Fa3"));
//    myGame.applyMove(myGame.makeMove("Fb3"));
//    myGame.applyMove(myGame.makeMove("Fc3"));
//    myGame.applyMove(myGame.makeMove("Fd3"));
//    myGame.applyMove(myGame.makeMove("Fe3"));
//    myGame.applyMove(myGame.makeMove("Fa4"));
//    myGame.applyMove(myGame.makeMove("Fb4"));
//    myGame.applyMove(myGame.makeMove("Fc4"));
//    myGame.applyMove(myGame.makeMove("Fd4"));
//    myGame.applyMove(myGame.makeMove("Fe4"));
//    myGame.applyMove(myGame.makeMove("Fa5"));
//    myGame.applyMove(myGame.makeMove("Fb5"));
//    myGame.applyMove(myGame.makeMove("Fc5"));
//    myGame.applyMove(myGame.makeMove("Fd5"));
//    myGame.applyMove(myGame.makeMove("Fe5"));
//    myGame.applyMove(myGame.makeMove("1b1+1"));
//    myGame.applyMove(myGame.makeMove("1d2<1"));
//
//    printGameState(myGame);
    
///// Game State Test
//    myGame.printState(myGame.WhitePieces);
//    cout << endl;
//    myGame.printState(myGame.BlackPieces);
//    cout << endl;
//    myGame.printState(myGame.CapStones);
//    cout << endl;
//    myGame.printState(myGame.Standing);
//    cout << endl;
/////
//    myGame.FindComponents();
/////

//    vector<Move> allMoves = myGame.generateAllMoves();
//    allMoves.size();
//    MiniMaxAgent player(2,5,150);
//    player.myGame = &myGame;
//    cout << "My Move : " << player.getMiniMaxMove() << endl;
    
    int p, n, t;
    cin >> p >> n >> t;
    MiniMaxAgent player3(p, n, t);
    cerr << "MiniMax Player" << endl;
    player3.playFirstMove();
    player3.play();

    return 0;
}
*/
int main(){
    Slides = GenerateAllSlides(5);
    
    Game myGame(5, 1);
    printGameState(myGame);
    cerr << endl;
    myGame.applyMove(myGame.makeMove("Fa1"));
    printGameState(myGame);
    cerr << endl;
    myGame.applyMove(myGame.makeMove("Fb1"));
    printGameState(myGame);
    cerr << endl;
    myGame.applyMove(myGame.makeMove("Fc1"));
    printGameState(myGame);
    cerr << endl;
    myGame.applyMove(myGame.makeMove("Fd1"));
    printGameState(myGame);
    cerr << endl;
    myGame.applyMove(myGame.makeMove("Fe1"));
    printGameState(myGame);
    cerr << endl;
    myGame.applyMove(myGame.makeMove("Fa2"));
    printGameState(myGame);
    cerr << endl;
    myGame.applyMove(myGame.makeMove("Fb2"));
    printGameState(myGame);
    cerr << endl;
    myGame.applyMove(myGame.makeMove("Fc2"));
    printGameState(myGame);
    cerr << endl;
    myGame.applyMove(myGame.makeMove("Fd2"));
    printGameState(myGame);
    cerr << endl;
    myGame.applyMove(myGame.makeMove("Fe2"));
    printGameState(myGame);
    cerr << endl;
    myGame.applyMove(myGame.makeMove("Fa3"));
    printGameState(myGame);
    cerr << endl;
    myGame.applyMove(myGame.makeMove("Fb3"));
    printGameState(myGame);
    cerr << endl;
    myGame.applyMove(myGame.makeMove("Fc3"));
    printGameState(myGame);
    cerr << endl;
    myGame.applyMove(myGame.makeMove("Fd3"));
    myGame.applyMove(myGame.makeMove("Fe3"));
    myGame.applyMove(myGame.makeMove("Fa4"));
    myGame.applyMove(myGame.makeMove("Fb4"));
    myGame.applyMove(myGame.makeMove("Fc4"));
    myGame.applyMove(myGame.makeMove("Fd4"));
    myGame.applyMove(myGame.makeMove("Fe4"));
    myGame.applyMove(myGame.makeMove("Fa5"));
    printGameState(myGame);
    cerr << endl;
    myGame.applyMove(myGame.makeMove("Fb5"));
    myGame.applyMove(myGame.makeMove("Fc5"));
    myGame.applyMove(myGame.makeMove("Fd5"));
    myGame.applyMove(myGame.makeMove("Fe5"));
    printGameState(myGame);
    cerr << endl;
    myGame.applyMove(myGame.makeMove("1b1+1"));
        printGameState(myGame);
        cerr << endl;
}
