//
//  Game.cpp
//  aBeautifulPlayer
//
//  Created by Akshay Singla on 01/10/16.
//  Copyright © 2016 Shantanu Kumar. All rights reserved.
//

#include <string.h>
#include <iostream>
#include <vector>
#define BLACK 0
#define WHITE 1
#define MAX_SIZE 7
#define MAX_SIZE_SQUARE MAX_SIZE*MAX_SIZE
using namespace std;
typedef unsigned long long uint64;

extern vector<vector<vector<short>>> Slides;

enum MoveType { Place = 0,
    SlideUp,
    SlideDown,
    SlideLeft,
    SlideRight
    
};

enum PieceType {    FlatStone = 0,
    StandingStone,
    Capstone
};

enum Color {    Black = 0,
    White
};

class Piece{
    public :
    PieceType type;
    Color color;
};




class Move{
    public :
    MoveType Movetype;
    Piece piece;
    short int row;
    short int column;
    short int *Drops;
    short int dropLength;
    
    Move(){
        
    }
    
    Move(int r, int c){
        row = r;
        column = c;
    }
    
    Move(int r, int c, Piece p){
        row = r;
        column = c;
        piece = p;
        Movetype = Place;
    }
};


class Config{
    public :
    int BoardSize;
    int Pieces;
    int Capstones;
    
    uint64 Left;
    uint64 Right;
    uint64 Top;
    uint64 Bottom;
    uint64 Edge;
    uint64 BoardMask;
    
    Config(int boardSize){
        BoardSize = boardSize;
        switch(BoardSize){
            case 5:
                Pieces = 21;
                Capstones = 1;
                break;
            case 6:
                Pieces = 30;
                Capstones = 1;
                break;
            case 7:
                Pieces = 40;
                Capstones = 1;
                break;
        }
        Left = 0;
        for (int i = 0 ; i < BoardSize*BoardSize ; i += BoardSize)
            Left |= (1ULL << i);
        
        Right = 0;
        for (int i = BoardSize - 1 ; i < BoardSize*BoardSize ; i += BoardSize)
            Right |= (1ULL << i);
        
        Top = 0;
        for (int i = 0 ; i < BoardSize ; i++)
            Top |= (1ULL << i);
        
        Bottom = 0;
        for (int i = BoardSize*BoardSize - BoardSize ; i < BoardSize*BoardSize ; i++)
            Bottom |= (1ULL << i);
        
        Edge = Top | Left | Bottom | Right;
        
        
        BoardMask = (1ULL << (BoardSize*BoardSize)) - 1;
        
    }
};

class Game{
    public :
    
    short int flats[2];
    short int capstones[2];
    
    uint64 WhitePieces;
    uint64 BlackPieces;
    uint64 CapStones;
    uint64 Standing;
    uint64 Stacks[MAX_SIZE_SQUARE];
    int Heights[MAX_SIZE_SQUARE];
    uint64 WhiteComponents[MAX_SIZE_SQUARE];
    uint64 BlackComponents[MAX_SIZE_SQUARE];
    short int size_cw;
    short int size_cb;
    Config *gameConfig;
    unsigned long moves;
    bool currentPlayer = 0;
    
    Game(int BoardSize){
        WhitePieces = 0;
        BlackPieces = 0;
        CapStones = 0;
        Standing = 0;
        gameConfig = new Config(BoardSize);
        memset(Stacks, 0 , MAX_SIZE_SQUARE*sizeof(uint64));
        memset(Heights, 0 , MAX_SIZE_SQUARE*sizeof(uint64));
        
        flats[0] = gameConfig->Pieces;
        flats[1] = gameConfig->Pieces;
        capstones[0] = gameConfig->Capstones;
        capstones[1] = gameConfig->Capstones;
    }
    
    Game(Game &current){
        WhitePieces = current.WhitePieces;
        BlackPieces = current.BlackPieces;
        CapStones = current.CapStones;
        Standing = current.Standing;
        memcpy(Stacks, current.Stacks, MAX_SIZE_SQUARE*sizeof(uint64));
        memcpy(Heights, current.Heights, MAX_SIZE_SQUARE*sizeof(uint64));
        gameConfig = current.gameConfig;
        currentPlayer = current.currentPlayer;
        moves = current.moves;
    }
    
    void PlaceMove(Move move){
        int i = move.row*gameConfig->BoardSize + move.column;
        Heights[i] += 1;
        Stacks[i] <<=1;
        switch (move.piece.color){
            case Black :
                BlackPieces |= (1ULL << i);
                break;
            case White :
                WhitePieces |= (1ULL << i);
                Stacks[i] |= 1;
                break;
        }
        switch (move.piece.type) {
            case Capstone :
                CapStones |= (1ULL << i);
                break;
            case StandingStone :
                Standing |= (1ULL << i);
                break;
        }
    }
    
    uint64 Expand(uint64 current, uint64 mask){
        uint64 next = current;
        do {
            current = next;
            next |= (current << 1);
            next |= (current >> 1);
            next |= (current >> gameConfig->BoardSize);
            next |= (current << gameConfig->BoardSize);
            next &= mask;
        } while (next !=current);
        return next;
    }
    
    void FindComponents(){
        size_cw = 0;
        uint64 whitePieces = WhitePieces&(~Standing);
        uint64 seen = 0;
        uint64 current;
        uint64 bit;
        
        while ( whitePieces){
            current = whitePieces & (whitePieces-1);
            bit = whitePieces & ~current;
            if ( (bit & seen) == 0){
                uint64 comp = Expand(bit,WhitePieces&(~Standing));
                if ( comp != bit){
                    WhiteComponents[size_cw++] = comp;
                }
                seen |= comp;
            }
            whitePieces = current;
        }
        
        size_cb = 0;
        uint64 blackPieces = BlackPieces&(~Standing);
        seen = 0;
        
        while ( blackPieces){
            current = blackPieces & (blackPieces-1);
            bit = blackPieces & ~current;
            if ( (bit & seen) == 0){
                uint64 comp = Expand(bit,BlackPieces&(~Standing));
                if ( comp != bit){
                    BlackComponents[size_cb++] = comp;
                }
                seen |= comp;
            }
            blackPieces = current;
        }
        return;
    }
    
    int checkIfRoadExists(){
        // -1 : False
        // 0 : Black
        // 1 : White
        // 2 : Both
        bool white = false;
        for (int i =0 ; i <size_cw; i++){
            if ( ((WhiteComponents[i] & gameConfig->Left) & (WhiteComponents[i] & gameConfig->Right)) != 0 || ((WhiteComponents[i] & gameConfig->Top) & (WhiteComponents[i] & gameConfig->Bottom)) != 0){
                white = true;
                break;
            }
        }
        
        bool black = false;
        for (int i =0 ; i <size_cb; i++){
            if ( ((BlackComponents[i] & gameConfig->Left) & (BlackComponents[i] & gameConfig->Right)) != 0 || ((BlackComponents[i] & gameConfig->Top) & (BlackComponents[i] & gameConfig->Bottom)) != 0){
                black = true;
                break;
            }
        }
        if ( black && white)
            return 2;
        else if (black)
            return 0;
        else if (white)
            return 1;
        else
            return -1;
    }
    
    void printState(uint64 X){
        for (int i = 0 ; i < gameConfig->BoardSize ; i++){
            for (int j = 0 ; j < gameConfig->BoardSize; j++){
                int r = i*gameConfig->BoardSize + j;
                if (X & ( 1ULL << r))
                    cerr << "1 ";
                else
                    cerr << "0 ";
                
            }
            cerr << endl;
        }
    }
    
    Move makeMove(string move,int colour){
        Move to_ret;
        switch(move[0]){
            case 'F': to_ret.piece.type = FlatStone;
                break;
            case 'S': to_ret.piece.type = StandingStone;
                break;
            case 'C': to_ret.piece.type = Capstone;
                break;
        }
        to_ret.piece.color = (Color) colour;
        to_ret.column = move[1]-'a';
        to_ret.row = move[2]-'1';
        return to_ret;
        
    }
    
    void slideMove(Move move){
        int r = move.row;
        int c = move.column;
        int i = r * gameConfig->BoardSize + c;
        uint64 pickupStack = Stacks[i];
        
        int numPieces = move.Drops[move.dropLength];
        
        if (numPieces == Heights[i]){
            WhitePieces &= ~(1ULL << i);
            BlackPieces &= ~(1ULL << i);
        } else {
            if ((pickupStack & (1ULL << numPieces)) != 0){
                WhitePieces |= (1ULL << i);
                BlackPieces &= ~(1ULL << i);
            } else {
                BlackPieces |= (1ULL << i);
                WhitePieces &= ~(1ULL << i);
            }
        }
        
        Stacks[i] >>= numPieces;
        Heights[i] -= numPieces;
        
        
        int dr = 0;
        int dc = 0;
        switch(move.Movetype){
            case SlideUp: dr = 1;
                break;
            case SlideDown: dr = -1;
                break;
            case SlideLeft: dc = -1;
                break;
            case SlideRight: dc = 1;
                break;
        }
        
        int j = 0;
        int piecesToDropHere = 0;
        while (numPieces>0){
            r += dr;
            c += dc;
            i = r * gameConfig->BoardSize + c;
            piecesToDropHere = move.Drops[j++];
            
            Standing &= ~(1ULL << i);
            
            uint64 drop = (pickupStack >> (numPieces - piecesToDropHere)) & ((1ULL << piecesToDropHere) - 1);
            Stacks[i] = (Stacks[i] << piecesToDropHere) | drop;
            Heights[i] += piecesToDropHere;
            
            if ((Stacks[i] & 1) != 0){
                WhitePieces |= (1ULL << i);
                BlackPieces &= ~(1ULL << i);
            } else {
                WhitePieces &= ~(1ULL << i);
                BlackPieces |= (1ULL << i);
            }
            
            numPieces -= piecesToDropHere;
        }
        
        int i_or = move.row * gameConfig->BoardSize + move.column;
        
        if ((CapStones & (1ULL << i_or)) != 0){
            CapStones |= (1ULL << i);
            CapStones &= ~(1ULL << i_or);
        }
        else if ((Standing & (1ULL << i_or)) != 0){
            Standing |= (1ULL << i);
            Standing &= ~(1ULL << i_or);
        }
    }
    
    void applyMove(Move move){
        if (move.Movetype < 1){
            PlaceMove(move);
            if (move.piece.type == FlatStone)
                flats[move.piece.color]--;
            else
                capstones[move.piece.color]--;
        } else {
            slideMove(move);
        }
        currentPlayer ^= 1;
    }
    
    Move GetPossSteps(uint64 filledPieces,int array[]){
        uint64 bit_set = filledPieces & ~(filledPieces & (filledPieces -1));
        short i = __builtin_ctzl(bit_set);
        int r = i/gameConfig->BoardSize;
        int c = i%gameConfig->BoardSize;
        Move move(r,c );
        int PossSteps_ = min(gameConfig->BoardSize - 1, Heights[i]);
        
        //SlideUp
        int PossSteps = min(gameConfig->BoardSize - r -1, PossSteps_);
        uint64 Z = (gameConfig->Left << c) & (Standing | Capstone);
        Z &= ~((bit_set << 1) - 1);
        
        if ( Z != 0){
            int j =  __builtin_ctzl(Z);
            PossSteps = min (PossSteps, (j-i)/gameConfig->BoardSize - 1);
        }
        array[0] = PossSteps;
        
        
        //SlideDown
        PossSteps = min(r, PossSteps_);
        Z = (gameConfig->Left << c) & (Standing | Capstone);
        Z &= (bit_set - 1);
        if ( Z != 0){
            int j =  63 - __builtin_clzl(Z);
            PossSteps = min (PossSteps, (i-j)/gameConfig->BoardSize - 1);
        }
        array[1] = PossSteps;
        
        
        //SlideLeft
        PossSteps = min(c, PossSteps_);
        Z = (gameConfig->Top << (r*gameConfig->BoardSize)) & (Standing | Capstone);
        Z &= (bit_set - 1);
        if ( Z != 0){
            int j =  63 - __builtin_clzl(Z);
            PossSteps = min (PossSteps, i-j - 1);
        }
        array[2] = PossSteps;
        
        // SlideRight
        PossSteps = min(gameConfig->BoardSize - c -1, PossSteps_);
        Z = (gameConfig->Top << (r*gameConfig->BoardSize)) & (Standing | Capstone);
        Z &= ~((bit_set << 1) - 1);
        
        if ( Z != 0){
            int j =  __builtin_ctzl(Z);
            PossSteps = min (PossSteps, (j-i) - 1);
        }
        array[3] = PossSteps;
        
        return move;
    }
    
    vector<Move> generateAllMoves(){
        vector<Move> allMoves;
        
        // Place Moves
        Piece piece;
        piece.color = (Color)currentPlayer;
        uint64 emptyPositions = ((~(WhitePieces | BlackPieces))&gameConfig->BoardMask);
        uint64 bits = 0;
        uint64 bit_set = 0;
        int i = 0;
        while (emptyPositions != 0){
            bits = emptyPositions & (emptyPositions -1);
            bit_set = emptyPositions & ~bits;
            i = __builtin_ctzl(bit_set);
            Move move(i/gameConfig->BoardSize, i%gameConfig->BoardSize, piece);
            if (flats[currentPlayer]>0){
                move.piece.type = FlatStone;
                allMoves.push_back(move);
                move.piece.type = StandingStone;
                allMoves.push_back(move);
            }
            if (capstones[currentPlayer]>0){
                move.piece.type = Capstone;
                allMoves.push_back(move);
            }
            emptyPositions = bits;
        }
        
        // Slide Moves
        uint64 filledPieces = BlackPieces;
        if (currentPlayer)
            filledPieces = WhitePieces;
        
        while (filledPieces != 0){
            int PossStepsInDir[4];
            Move move = GetPossSteps(filledPieces, PossStepsInDir);
            int i = move.row * gameConfig->BoardSize + move.column;
            filledPieces &= (filledPieces-1);
            for (int dir = 0; dir < 4; dir++){
                int PossSteps = PossStepsInDir[dir];
                for (int u = 1 ; u <= PossSteps; u++){
                    for (int t = 0 ; t < Slides[u].size(); t++){
                        if ( Slides[u][t][u] > Heights[i])
                            break;
                        move.dropLength = u;
                        move.Movetype = (MoveType)(1+dir);
                        move.Drops = &Slides[u][t][0];
                        allMoves.push_back(move);
                    }
                }
            }
        }
        
        // Capstone Moves
        filledPieces = BlackPieces;
        if (currentPlayer)
            filledPieces = WhitePieces;
        filledPieces &= CapStones;
        
        while (filledPieces != 0){
            int PossStepsInDir[4];
            Move move = GetPossSteps(filledPieces, PossStepsInDir);
            for (int dir = 0; dir < 4; dir++){
                int u = PossStepsInDir[dir] + 1;
                
                uint64 final_position = 0;
                uint64 bit_set = filledPieces & ~(filledPieces & (filledPieces -1));
                switch (dir){
                    case 0 : final_position = ((bit_set << gameConfig->BoardSize * u) & Standing);
                        break;
                    case 1 : final_position = ((bit_set >> gameConfig->BoardSize * u) & Standing);
                        break;
                    case 2 : if (u<move.column)
                                final_position = ((bit_set >> u) & Standing);
                        break;
                    case 3 : if (u<gameConfig->BoardSize-move.column-1)
                                final_position = ((bit_set << u) & Standing);
                        break;
                }
                
                int i = move.row * gameConfig->BoardSize + move.column;
                if (final_position != 0){
                    for (int t = 0 ; t < Slides[u].size(); t++){
                        if (Slides[u][t][u-1]!=1)
                            continue;
                        if ( Slides[u][t][u] > Heights[i])
                            break;
                        move.dropLength = u;
                        move.Movetype = (MoveType)(1+dir);
                        move.Drops = &Slides[u][t][0];
                        allMoves.push_back(move);
                    }
                }
                
            }
            filledPieces &= (filledPieces-1);
        }

        
        return allMoves;
    }
};
