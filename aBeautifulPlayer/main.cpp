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

vector<vector<vector<int>>> GenerateAllSlides(int K) {
    vector<vector<vector<int>>> slides(8);
    vector<vector<vector<int>>> slides1(8);
    
    for (int s = 1; s <= K; s++ ){
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

vector<vector<vector<int>>> Slides;





int main(){
    Slides = GenerateAllSlides(5);
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
    
    myGame.PlaceMove(myGame.makeMove("Fa1",1));
    myGame.PlaceMove(myGame.makeMove("Fb1",0));
    myGame.PlaceMove(myGame.makeMove("Wc1",1));
    myGame.PlaceMove(myGame.makeMove("Fd1",0));
    myGame.PlaceMove(myGame.makeMove("Fa2",1));
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
    cout << bitset<64>(myGame.WhiteComponents[0]) << endl;

    cout << sizeof(unsigned long long) << endl;
}
