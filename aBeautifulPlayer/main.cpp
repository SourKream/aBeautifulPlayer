//
//  main.cpp
//  aBeautifulPlayer
//
//  Created by Shantanu Kumar on 23/09/16.
//  Copyright © 2016 Shantanu Kumar. All rights reserved.
//

#include <iostream>
#include <vector>
#include <unordered_set>
#include <random>
using namespace std;

enum Colour {Black, White};
enum PieceType {Flat, Wall, Cap};
enum Direction {Up, Down, Left, Right};

struct Piece{
    Colour colour;
    PieceType type;
    
    void printPiece(){
        switch(colour){
            case White: cerr << "w";
                break;
            case Black: cerr << "b";
                break;
        }
        switch(type){
            case Flat: cerr << "F ";
                break;
            case Wall: cerr << "W ";
                break;
            case Cap: cerr << "C ";
                break;
        }
    }
};

struct Player{
    
    int numFlats;
    int numCaps;
    Colour colour;
    
    Player (int initFlats, int initCaps, Colour playerColour){
        numFlats = initFlats;
        numCaps = initCaps;
        colour = playerColour;
    }
};

struct Game{

    unordered_set<int> emptySquares;
    vector<vector<vector<Piece> > > board;
    vector<Player> players;
    int maxFlats;
    int maxCaps;
    int boardSize;
    int numSquares;
    
    int currentPlayer;
    
    Game (int n){
        boardSize = n;
        numSquares = n*n;
        currentPlayer = 1;
        
        // Initialise board to empty vectors
        vector<vector<Piece> > A;
        vector<Piece> B;
        for (int i=0; i<boardSize; i++)
            A.push_back(B);
        for (int i=0; i<boardSize; i++)
            board.push_back(A);
        
        for (int i=0; i<numSquares; i++)
            emptySquares.insert(i);
        
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
        Player p1(maxFlats, maxCaps, White), p2(maxFlats, maxCaps, Black);
        players.push_back(p1);
        players.push_back(p2);
        
    }
    
    void place(Piece piece, int c, int r){
        board[r][c].push_back(piece);
        emptySquares.erase(get1DIndex(c, r));
    }
    
    void moveStack(int numPieces, int c, int r, Direction dir, vector<int> drops){
        if (numPieces == board[r][c].size())
            emptySquares.insert(get1DIndex(c, r));
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
            if (numPieces == 1)
                if (board[r][c].size()>1)
                    if (board[r][c].back().type == Wall)
                        board[r][c].back().type = Flat;
            for (int j=0; j<drops[i]; j++){
                board[r][c].push_back(pickupStack.back());
                pickupStack.pop_back();
                numPieces--;
            }
            emptySquares.erase(get1DIndex(c, r));
            i++;
        }
    }
    
    void applyMove (string move){
        if (isalpha(move[0])){
            Piece piece;
            piece.colour = players[currentPlayer].colour;
            switch(currentPlayer){
                case 0: piece.colour = White;
                    break;
                case 1: piece.colour = Black;
                    break;
            }
            switch(move[0]){
                case 'F': piece.type = Flat;
                    break;
                case 'S': piece.type = Wall;
                    break;
                case 'C': piece.type = Cap;
                    break;
            }
            place(piece, move[1]-'a', move[2]-'1');
            if (move[0]=='C')
                players[currentPlayer].numCaps--;
            else
                players[currentPlayer].numFlats--;
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
            
            moveStack(move[0]-'0', move[1]-'a', move[2]-'1', dir, drops);
        }
        currentPlayer += 1;
        currentPlayer %= 2;
    }
    
    //////////////////////////////////////////////////////
    // Utility Function
    int get1DIndex (int c, int r){
        return boardSize * c + r;
    }
    
    int get1DIndex (string s){
        return boardSize * (s[0]-'a') + (s[1]-'1');
    }

    pair<int,int> get2DIndex (int index){
        pair<int, int> A(index/boardSize,index%boardSize);
        return A;
    }

    pair<int,int> get2DIndex (string s){
        return get2DIndex(get1DIndex(s));
    }

    string getStringIndex (int index){
        string s;
        s += ('a' + index/boardSize);
        s += ('1' + index%boardSize);
        return s;
    }

    string getStringIndex (int c, int r){
        return getStringIndex(get1DIndex(c, r));
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
    
    string getMoveString (int numPieces, int c, int r, Direction dir, vector<int> drops){
        string move;
        move = (numPieces+'0');
        move += getStringIndex(c,r);
        switch (dir){
            case Up: move += '-';
                break;
            case Down: move += '+';
                break;
            case Right: move += '>';
                break;
            case Left: move += '<';
                break;
        }
        for (int i=0; i<drops.size(); i++)
            move += (drops[i] + '0');
        
        return move;
    }
    //////////////////////////////////////////////////////

    vector<string> generateAllMoves(){
        vector<string> moves;
        
        // Place Moves
        if (players[currentPlayer].numFlats>0)
            for (auto i=emptySquares.begin(); i!=emptySquares.end(); i++){
                string pos = getStringIndex(*i);
                moves.push_back("F" + pos);
                moves.push_back("S" + pos);
            }
        if (players[currentPlayer].numCaps>0)
            for (auto i=emptySquares.begin(); i!=emptySquares.end(); i++){
                string pos = getStringIndex(*i);
                moves.push_back("C" + pos);
            }

        // Stack Moves
        for (int i=0; i<numSquares; i++){
            if (emptySquares.find(i) == emptySquares.end()){
                pair<int,int> cr = get2DIndex(i);
                int c = cr.first;
                int r = cr.second;
                if (board[r][c].back().colour == players[currentPlayer].colour){
                    for (int numPieces=1; numPieces<=min((int)board[r][c].size(), boardSize); numPieces++){
                        vector<vector<int> > splits = getSplits(numPieces);
                        for (int j=0; j<splits.size(); j++){
                            if (isValidStackMove(numPieces, c, r, Up, splits[j]))
                                moves.push_back(getMoveString(numPieces, c, r, Up, splits[j]));
                            if (isValidStackMove(numPieces, c, r, Down, splits[j]))
                                moves.push_back(getMoveString(numPieces, c, r, Down, splits[j]));
                            if (isValidStackMove(numPieces, c, r, Left, splits[j]))
                                moves.push_back(getMoveString(numPieces, c, r, Left, splits[j]));
                            if (isValidStackMove(numPieces, c, r, Right, splits[j]))
                                moves.push_back(getMoveString(numPieces, c, r, Right, splits[j]));
                        }
                    }
                }
            }
        }
        
        return moves;
    }
    
    vector<string> generateFirstMove(){
        vector<string> moves;
        if (players[currentPlayer].numFlats>0)
            for (auto i=emptySquares.begin(); i!=emptySquares.end(); i++){
                string pos = getStringIndex(*i);
                moves.push_back("F" + pos);
            }
        return moves;
    }
    
    bool isValidStackMove(int numPieces, int c, int r, Direction dir, vector<int> drops){
        // TODO : There may be an error here
        
        int rr = r;
        int cc = c;
        
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
        
        int wallInWay = 0;
        while(wallInWay<drops.size()){
            r += dr;
            c += dc;
            if (r < 0 || r >= boardSize || c < 0 || c >= boardSize)
                return false;
            if (board[r][c].size() != 0)
                if (board[r][c].back().type != Flat)
                    break;
            wallInWay++;
        }
        
        if (wallInWay == drops.size())
            return true;
        
        if (wallInWay == drops.size()-1)
            if (drops.back()==1)
                if (board[rr][cc].back().type == Cap)
                    if (board[r][c].back().type != Cap)
                        return true;

        return false;
    }
    
    void printGameState(){
        for (int i=boardSize-1; i>-1; i--){
            for (int j=0; j<boardSize; j++){
                cerr << "[";
                for (int k=0; k<board[i][j].size(); k++)
                    board[i][j][k].printPiece();
                cerr << "]\t";
            }
            cerr << endl;
        }
    }
    
};

struct RandomAgent{
    
    Game* myGame;
    int myPlayerNumber;
    int boardSize;
    int timeLimit;

    RandomAgent (int playerNum, int n, int t){
        myPlayerNumber = playerNum;
        boardSize = n;
        timeLimit = t;
        myGame = new Game(boardSize);
    }
    
    void playFirstMove(){
        string move;
        if (myPlayerNumber == 2){
            cin >> move;
            myGame->applyMove(move);
        }
        
        vector<string> allMoves = myGame->generateFirstMove();
        move = allMoves[rand()%allMoves.size()];
        myGame->applyMove(move);
        cout << move << endl;

        if (myPlayerNumber == 0){
            cin >> move;
            myGame->applyMove(move);
        }
        myGame->currentPlayer = 0;
    }
    
    void play(){
        string move;

        if (myPlayerNumber == 2){
            cin >> move;
            myGame->applyMove(move);
        }

        while(true){
            vector<string> allMoves = myGame->generateAllMoves();
            cerr << "Possible Moves : " << allMoves.size() << endl;
            move = allMoves[rand()%allMoves.size()];
            myGame->applyMove(move);
            cout << move << endl;

            cin >> move;
            myGame->applyMove(move);
        }
    }
    
};

int main(int argc, const char * argv[]) {

    int p, n, t;
    cin >> p >> n >> t;
    RandomAgent player(p, n, t);
    player.playFirstMove();
    player.play();

    return 0;
}
