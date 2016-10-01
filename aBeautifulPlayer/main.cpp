//
//  main.cpp
//  aBeautifulPlayer
//
//  Created by Akshay Singla on 01/10/16.
//  Copyright © 2016 Shantanu Kumar. All rights reserved.
//

#include <stdio.h>
#include "Game.cpp"
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


int main(){
    Slides = GenerateAllSlides(5);
    
    uint64 Z = ((1ULL << 63) | ( 1ULL << 31));
    
    cout << __builtin_clzl(Z) << endl;
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
//
    Game myGame(5);
    
    myGame.PlaceMove(myGame.makeMove("Fc3",1));
    myGame.PlaceMove(myGame.makeMove("Fa3",0));
    myGame.PlaceMove(myGame.makeMove("Fa3",0));
    myGame.PlaceMove(myGame.makeMove("Fa3",0));
    myGame.PlaceMove(myGame.makeMove("Fa3",0));
    myGame.PlaceMove(myGame.makeMove("Fa3",1));
    myGame.PlaceMove(myGame.makeMove("Fc5",0));
    //myGame.PlaceMove(myGame.makeMove("Sd3",1));
    //myGame.PlaceMove(myGame.makeMove("Sa3",1));
    myGame.PlaceMove(myGame.makeMove("Sc1",1));
    myGame.PlaceMove(myGame.makeMove("Ce5",0));

    myGame.printState(myGame.WhitePieces);
    cout << endl;
    myGame.printState(myGame.BlackPieces);
    cout << endl;
    myGame.printState(myGame.CapStones);
    cout << endl;
    myGame.printState(myGame.Standing);
    cout << endl;
    
    myGame.FindComponents();
    
    myGame.generateAllMoves();
    cout << bitset<64>(myGame.WhiteComponents[0]) << endl;

    cout << sizeof(unsigned long long) << endl;
}
