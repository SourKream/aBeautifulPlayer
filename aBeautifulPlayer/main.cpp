//
//  main.cpp
//  aBeautifulPlayer
//
//  Created by Shantanu Kumar on 23/09/16.
//  Copyright © 2016 Shantanu Kumar. All rights reserved.
//

#include <iostream>
#include <vector>
using namespace std;

enum Colour {Black, White};
enum PieceType {Flat, Wall, Cap};
enum Direction {Up, Down, Left, Right};

struct Piece{
    Colour colour;
    PieceType type;
    
    void printPiece(){
        switch(colour){
            case White: cout << "w";
                break;
            case Black: cout << "b";
                break;
        }
        switch(type){
            case Flat: cout << "F ";
                break;
            case Wall: cout << "W ";
                break;
            case Cap: cout << "C ";
                break;
        }
    }
};

struct Player{
    
    int numFlats;
    int numCaps;
    
    Player (int initFlats, int initCaps){
        numFlats = initFlats;
        numCaps = initCaps;
    }
};

struct Game{
    
    vector<vector<vector<Piece>>> board;
    vector<Player> players;
    int maxFlats;
    int maxCaps;
    int boardSize;
    
    int currentPlayer;
    
    Game (int n){
        boardSize = n;
        currentPlayer = 0;
        
        // Initialise board to empty vectors
        vector<vector<Piece>> A;
        vector<Piece> B;
         for (int i=0; i<boardSize; i++)
            A.push_back(B);
        for (int i=0; i<boardSize; i++)
            board.push_back(A);
        
        // Set params of board size
        switch(boardSize){
            case 5: maxFlats = 21;
                maxCaps = 1;
                break;
            case 6: maxFlats = 30;
                maxCaps = 1;
                break;
            case 7: maxFlats = 40;
                maxCaps = 1;
                break;
        }

        // Initialise Players
        Player p1(maxFlats, maxCaps), p2(maxFlats, maxCaps);
        players.push_back(p1);
        players.push_back(p2);
        
    }
    
    void Place(Piece piece, int c, int r){
        board[r][c].push_back(piece);
    }
    
    void Move(int numPieces, int c, int r, Direction dir, vector<int> drops){
        vector<Piece> pickupStack;
        for (int i=0; i<numPieces; i++){
            pickupStack.push_back(board[r][c].back());
            board[r][c].pop_back();
        }
        int dr = 0;
        int dc = 0;
        switch(dir){
            case Up: dr = -1;
                break;
            case Down: dr = 1;
                break;
            case Right: dc = 1;
                break;
            case Left: dc = -1;
                break;
        }
        int i = 0;
        while(numPieces>0){
            r += dr;
            c += dc;
            for (int j=0; j<drops[i]; j++){
                board[r][c].push_back(pickupStack.back());
                pickupStack.pop_back();
                numPieces--;
            }
            i++;
        }
    }
    
    void ApplyMove (string move){
        if (isalpha(move[0])){
            Piece piece;
            if (currentPlayer == 0)
                piece.colour = White;
            else
                piece.colour = Black;
            switch(currentPlayer){
                case 0: piece.colour = White;
                    break;
                case 1: piece.colour = Black;
                    break;
            }
            switch(move[0]){
                case 'F': piece.type = Flat;
                    break;
                case 'W': piece.type = Wall;
                    break;
                case 'C': piece.type = Cap;
                    break;
            }
            Place(piece, move[1]-'a', move[2]-'1');
        } else {
            Direction dir = Up;
            vector<int> drops;

            switch (move[3]){
                case '-': dir = Up;
                    break;
                case '+': dir = Down;
                    break;
                case '>': dir = Right;
                    break;
                case '<': dir = Left;
                    break;
            }
            
            for (int i=4; i<move.size(); i++)
                drops.push_back(move[i]-'0');
            
            Move(move[0]-'0', move[1]-'a', move[2]-'1', dir, drops);
        }
        currentPlayer += 1;
        currentPlayer %= 2;
    }
    
    void printGameState(){
        for (int i=boardSize-1; i>-1; i--){
            for (int j=0; j<boardSize; j++){
                cout << "[";
                for (int k=0; k<board[i][j].size(); k++)
                    board[i][j][k].printPiece();
                cout << "]\t";
            }
            cout << endl;
        }
    }
    
};

int main(int argc, const char * argv[]) {
    
    Game myGame(5);
    myGame.ApplyMove("Fa1");
    myGame.printGameState();
    cout << "----------------";
    myGame.ApplyMove("Wb1");
    myGame.printGameState();
    cout << "----------------";
    myGame.ApplyMove("Ca2");
    myGame.printGameState();
    cout << "----------------";
    myGame.ApplyMove("1b1<1");
    myGame.printGameState();
    cout << "----------------";
    myGame.ApplyMove("1a2-1");
    myGame.printGameState();
    cout << "----------------";
    myGame.ApplyMove("3a1>111");
    myGame.printGameState();
    cout << "----------------";
    myGame.ApplyMove("1c1+1");
    myGame.printGameState();
    cout << "----------------";
    
    return 0;
}
