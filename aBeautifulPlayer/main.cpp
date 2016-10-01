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


int main(){
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
