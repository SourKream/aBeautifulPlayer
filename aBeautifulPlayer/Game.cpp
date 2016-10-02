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

extern vector<vector<vector<short> > > Slides;


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
    MoveType Movetype = Place;
    Piece piece;
    short int row;
    short int column;
    short int Drops[8];
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
    uint64 InfluenceMasks[MAX_SIZE_SQUARE];
    double FlatScore = 6;
    double StandingStoneScore = 1;
    double CapStoneScore = 2;
    double CenterScore = 2;
    double StackHeightScore = 2;
    double InfluenceScore = 3;
    double GroupSizeScore = 0;
    
    inline uint64 Expand(uint64 current){
        uint64 next = current;
        for (int i = 0 ; i < 2; i ++){
            current = next;
            next |= (current << 1) & ~(Left);
            next |= (current >> 1) & ~(Right);
            next |= (current >> BoardSize);
            next |= (current << BoardSize);
            next ^= current;
        }
        next &= BoardMask;
        return next;
    }
    
    
    Config(int boardSize, double Scores[]){
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
        
        for (int i = 0 ; i < BoardSize*BoardSize; i++)
            InfluenceMasks[i] = Expand(1ULL << i);
        
        FlatScore = Scores[0];
        StandingStoneScore = Scores[1];
        CapStoneScore = Scores[2];
        CenterScore = Scores[3];
        StackHeightScore = Scores[4];
        InfluenceScore = Scores[5];
        GroupSizeScore = Scores[6];
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
    
    bool currentPlayer = 1;
    bool myPlayerNumber;

    Game(int BoardSize, int playerNumber,double Scores []){
        WhitePieces = 0;
        BlackPieces = 0;
        CapStones = 0;
        Standing = 0;
        gameConfig = new Config(BoardSize, Scores);
        memset(Stacks, 0 , MAX_SIZE_SQUARE*sizeof(uint64));
        memset(Heights, 0 , MAX_SIZE_SQUARE*sizeof(uint64));
        flats[0] = gameConfig->Pieces;
        flats[1] = gameConfig->Pieces;
        capstones[0] = gameConfig->Capstones;
        capstones[1] = gameConfig->Capstones;
        
        myPlayerNumber = playerNumber % 2;
    }
    
    Game(Game &current){
        
        WhitePieces = current.WhitePieces;
        BlackPieces = current.BlackPieces;
        CapStones = current.CapStones;
        Standing = current.Standing;
        
        memcpy(Stacks, current.Stacks, MAX_SIZE_SQUARE*sizeof(uint64));
        memcpy(Heights, current.Heights, MAX_SIZE_SQUARE*sizeof(int));
        memcpy(WhiteComponents, current.WhiteComponents, MAX_SIZE_SQUARE*sizeof(uint64));
        memcpy(BlackComponents, current.BlackComponents, MAX_SIZE_SQUARE*sizeof(uint64));
        
        gameConfig = current.gameConfig;
        currentPlayer = current.currentPlayer;
        size_cb = current.size_cb;
        size_cw = current.size_cw;
        
        myPlayerNumber = current.myPlayerNumber;
        
        memcpy(flats, current.flats, 2*sizeof(short int));
        memcpy(capstones, current.capstones, 2*sizeof(short int));
    }
    
    inline int Popcount( unsigned long long x) {
        // bit population count, see
        // http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        if (x == 0) {
            return 0;
        }
        x -= (x >> 1) & 0x5555555555555555;
        x = ((x>>2)&0x3333333333333333 )+ (x&0x3333333333333333);
        x += x >> 4;
        x &= 0x0F0F0F0F0F0F0F0F;
        x *= 0x0101010101010101;
        return (x >> 56);
    }
    
    void printGameState(){
        for (int r=gameConfig->BoardSize-1; r>-1; r--){
            for (int c=0; c<gameConfig->BoardSize; c++){
                cerr << "[";
                for (int k =Heights[gameConfig->BoardSize*r + c]-1; k>-1; k--)
                    if ((Stacks[gameConfig->BoardSize*r + c] & (1<<k)) != 0)
                        cerr << "W";
                    else
                        cerr << "B";
                cerr << "]\t";
            }
            cerr << endl;
        }
    }
    

    
    Move makeMove(string move, bool onOpponent = false){
        
        Move moveOut;
        if (isalpha(move[0])){
            moveOut.Movetype = Place;
            moveOut.piece.color = (Color)(onOpponent ^ currentPlayer);
            switch(move[0]){
                case 'F': moveOut.piece.type = FlatStone;
                    break;
                case 'S': moveOut.piece.type = StandingStone;
                    break;
                case 'C': moveOut.piece.type = Capstone;
                    break;
            }
            moveOut.column = move[1]-'a';
            moveOut.row = move[2]-'1';
        } else if (isdigit(move[0])) {
            int k=0;
            
            switch (move[3]){
                case '+': moveOut.Movetype = SlideUp;
                    break;
                case '-': moveOut.Movetype = SlideDown;
                    break;
                case '>': moveOut.Movetype = SlideRight;
                    break;
                case '<': moveOut.Movetype = SlideLeft;
                    break;
            }
            int sum = 0;
            for (int i=4; i<move.size(); i++){
                moveOut.Drops[k++] = move[i]-'0';
                sum += moveOut.Drops[k-1];
            }
            moveOut.Drops[k] = sum;
            moveOut.column = move[1]-'a';
            moveOut.row = move[2]-'1';
            //moveOut.Drops = &drops[0];
            moveOut.dropLength = k;
        }
        
        return moveOut;
    }
    
    string getMoveString (Move moveIn){
        if (moveIn.Movetype<1){
            string move;
            switch (moveIn.piece.type){
                case FlatStone: move = 'F';
                    break;
                case StandingStone: move = 'S';
                    break;
                case Capstone: move = 'C';
                    break;
            }
            move += (moveIn.column + 'a');
            move += (moveIn.row + '1');
            return move;
        }
        else{
            
            string move;
            move = (moveIn.Drops[moveIn.dropLength]+'0');
            move += (moveIn.column + 'a');
            move += (moveIn.row + '1');
            if (moveIn.Movetype == SlideUp)
                move += '+';
            else if ( moveIn.Movetype == SlideDown)
                move += '-';
            else if (moveIn.Movetype == SlideRight)
                move += '>';
            else if (moveIn.Movetype == SlideLeft)
                move += '<';
            else{
                cerr << "SOMETHING HAPPENED" << endl;
                exit(0);
            }
            for (int i=0; i<moveIn.dropLength; i++)
                move += (moveIn.Drops[i] + '0');
            
            return move;
        }
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
        if (move.Movetype == SlideUp)
            dr = 1;
        else if ( move.Movetype == SlideDown)
            dr = -1;
        else if (move.Movetype == SlideRight)
            dc = 1;
        else if (move.Movetype == SlideLeft)
            dc = -1;
        else{
            cerr << "SOMETHING HAPPENED 2" << endl;
            exit(0);
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
            case FlatStone :
                break;
        }
    }
    
    void applyMove(Move move){
        if (move.Movetype < 1){
            PlaceMove(move);
            if (move.piece.type != Capstone)
                flats[move.piece.color]--;
            else
                capstones[move.piece.color]--;
        } else {
            slideMove(move);
        }
        FindComponents();
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
        uint64 Z = (gameConfig->Left << c) & (Standing | CapStones);
        Z &= ~((bit_set << 1) - 1);
        
        if ( Z != 0){
            int j =  __builtin_ctzl(Z);
            PossSteps = min (PossSteps, (j-i)/gameConfig->BoardSize - 1);
        }
        array[0] = PossSteps;
        
        
        //SlideDown
        PossSteps = min(r, PossSteps_);
        Z = (gameConfig->Left << c) & (Standing | CapStones);
        Z &= (bit_set - 1);
        if ( Z != 0){
            int j =  63 - __builtin_clzl(Z);
            PossSteps = min (PossSteps, (i-j)/gameConfig->BoardSize - 1);
        }
        array[1] = PossSteps;
        
        
        //SlideLeft
        PossSteps = min(c, PossSteps_);
        Z = (gameConfig->Top << (r*gameConfig->BoardSize)) & (Standing | CapStones);
        Z &= (bit_set - 1);
        if ( Z != 0){
            int j =  63 - __builtin_clzl(Z);
            PossSteps = min (PossSteps, i-j - 1);
        }
        array[2] = PossSteps;
        
        // SlideRight
        PossSteps = min(gameConfig->BoardSize - c -1, PossSteps_);
        Z = (gameConfig->Top << (r*gameConfig->BoardSize)) & (Standing | CapStones);
        Z &= ~((bit_set << 1) - 1);
        
        if ( Z != 0){
            int j =  __builtin_ctzl(Z);
            PossSteps = min (PossSteps, (j-i) - 1);
        }
        array[3] = PossSteps;
        
        return move;
    }
    
    vector<Move> generateAllMoves(){
        vector<Move> allMoves(1000);
        int K = 0;
        
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
                allMoves[K++] = move;
                // allMoves.push_back(move);
                move.piece.type = StandingStone;
                allMoves[K++] = move;
                // allMoves.push_back(move);
            }
            if (capstones[currentPlayer]>0){
                move.piece.type = Capstone;
                allMoves[K++] = move;
                // allMoves.push_back(move);
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
                        memcpy(&move.Drops[0],&Slides[u][t][0], 8*sizeof(short) );
                        // move.Drops = &Slides[u][t][0];
                        // allMoves.push_back(move);
                        allMoves[K++] = move;
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
                        memcpy(&move.Drops[0],&Slides[u][t][0], 8*sizeof(short) );
                        // allMoves.push_back(move);
                        allMoves[K++] = move;
                    }
                }
                
            }
            filledPieces &= (filledPieces-1);
        }
        
        random_shuffle(allMoves.begin(),allMoves.begin() + K -1);
        return {allMoves.begin(),allMoves.begin() + K - 1};
    }
    
    vector<Move> generateFirstMove(){
        vector<Move> moves;
        Piece piece;
        piece.color = (Color)(!currentPlayer);
        piece.type = FlatStone;
        uint64 emptyPositions = ((~(WhitePieces | BlackPieces))&gameConfig->BoardMask);
        uint64 mask = (1ULL) | (1ULL << (gameConfig->BoardSize -1)) | (1ULL << (gameConfig->BoardSize*(gameConfig->BoardSize -1))) | (1ULL << (gameConfig->BoardSize*(gameConfig->BoardSize) -1));
        emptyPositions &= mask;
        uint64 bits = 0;
        uint64 bit_set = 0;
        int i = 0;
        
        while (emptyPositions != 0){
            bits = emptyPositions & (emptyPositions -1);
            bit_set = emptyPositions & ~bits;
            i = __builtin_ctzl(bit_set);
            Move move(i/gameConfig->BoardSize, i%gameConfig->BoardSize, piece);
            moves.push_back(move);
            emptyPositions = bits;
        }
        
        return moves;
    }
    
    int isFinishState(){
        // Return winner player number if finish state
        // Return -1 if not finish state
        
        int winner = checkIfRoadExists();
        if (winner!=-1)
            return winner;
        
        if ((flats[0]==0)||(flats[1]==0)){
            int whiteFlats = Popcount(WhitePieces & ~(Standing | CapStones));
            int blackFlats = Popcount(BlackPieces & ~(Standing | CapStones));
            
            if (blackFlats > whiteFlats)
                return 0;
            else if (blackFlats < whiteFlats)
                return 1;
            else
                return (flats[0]>0)?0:1;
        }
        
        return -1;
    }
    
    inline uint64 Expand(uint64 current, uint64 mask){
        uint64 next = current;
        do {
            current = next;
            next |= (current << 1) & ~(gameConfig->Left);
            next |= (current >> 1) & ~(gameConfig->Right);
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
            if ( ((WhiteComponents[i] & gameConfig->Left) != 0 && (WhiteComponents[i] & gameConfig->Right) != 0) || ((WhiteComponents[i] & gameConfig->Top) != 0 && (WhiteComponents[i] & gameConfig->Bottom) !=0)){
                white = true;
                break;
            }
        }
        
        bool black = false;
        for (int i =0 ; i <size_cb; i++){
            if ( ((BlackComponents[i] & gameConfig->Left) != 0 && (BlackComponents[i] & gameConfig->Right) != 0) || ((BlackComponents[i] & gameConfig->Top) != 0 && (BlackComponents[i] & gameConfig->Bottom) != 0) ){
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
    
    int getStateValue(){

        int winner = checkIfRoadExists();
        if (winner != -1){
            if (winner == myPlayerNumber)
                return 1000;
            return -1000;
        }
        //Normal Scores
        double score = Popcount(WhitePieces & ~(Standing|CapStones))*gameConfig->FlatScore;
        score -= Popcount(BlackPieces & ~(Standing|CapStones))*gameConfig->FlatScore;
        score += Popcount(WhitePieces & (CapStones))*gameConfig->CapStoneScore;
        score -= Popcount(BlackPieces & (CapStones))*gameConfig->CapStoneScore;
        score += Popcount(WhitePieces & (Standing))*gameConfig->StandingStoneScore;
        score -= Popcount(BlackPieces & (Standing))*gameConfig->StandingStoneScore;
        score += Popcount(WhitePieces & ~(gameConfig->Edge))*gameConfig->CenterScore;
        score -= Popcount(BlackPieces & ~(gameConfig->Edge))*gameConfig->CenterScore;
        
        uint64 filledPieces = WhitePieces;
        uint64 bit_set;
        // Captured Stack Scores
        short i;
        while (filledPieces != 0){
            bit_set = filledPieces & ~(filledPieces & (filledPieces -1));
            i = __builtin_ctzl(bit_set);
            score += (Heights[i] - Popcount(Stacks[i] & ((1 << Heights[i]) - 1)))*gameConfig->StackHeightScore;
            score += Popcount(WhitePieces & gameConfig->InfluenceMasks[i])*gameConfig->InfluenceScore;
            filledPieces = filledPieces & (filledPieces -1);
        }
        
        filledPieces = BlackPieces;
            
        while (filledPieces != 0){
            bit_set = filledPieces & ~(filledPieces & (filledPieces -1));
            i = __builtin_ctzl(bit_set);
            score -= Popcount((Stacks[i] & ((1 << Heights[i]) - 1)))*gameConfig->StackHeightScore;
            score -= Popcount(BlackPieces & gameConfig->InfluenceMasks[i])*gameConfig->InfluenceScore;
            filledPieces = filledPieces & (filledPieces -1);
        }
        
        
        // Group Connected Component Scores
        for (i =0 ; i < size_cw ; i++){
            score += Popcount(WhiteComponents[i])*gameConfig->GroupSizeScore;
        }
        
        for (i =0 ; i < size_cb ; i++){
            score -= Popcount(BlackComponents[i])*gameConfig->GroupSizeScore;
        }
        int Integer_Score = (int) score;
        return (myPlayerNumber==1)?Integer_Score:-Integer_Score;
        
    }
    
    vector<vector<int>> AnalyzeBoard(){
        FindComponents();
        vector<vector<int>> to_Ret(2,vector<int>(7));


        to_Ret[1][0] = Popcount(WhitePieces & ~(Standing|CapStones));
        to_Ret[0][0] = Popcount(BlackPieces & ~(Standing|CapStones));
        
        to_Ret[1][1] = Popcount(WhitePieces & (Standing));
        to_Ret[0][1] = Popcount(BlackPieces & (Standing));
        
        to_Ret[1][2] = Popcount(WhitePieces & (CapStones));
        to_Ret[0][2] = Popcount(WhitePieces & (CapStones));

        to_Ret[1][3] = Popcount(WhitePieces & ~(gameConfig->Edge));
        to_Ret[0][3] = Popcount(BlackPieces & ~(gameConfig->Edge));
        
        uint64 filledPieces = WhitePieces;
        uint64 bit_set;
        // Captured Stack Scores
        to_Ret[1][4] = 0;
        to_Ret[1][5] = 0;
        short i;
        while (filledPieces != 0){
            bit_set = filledPieces & ~(filledPieces & (filledPieces -1));
            i = __builtin_ctzl(bit_set);
            to_Ret[1][4] += (Heights[i] - Popcount((Stacks[i] & ((1 << Heights[i]) - 1))));
            to_Ret[1][5] += Popcount(WhitePieces & gameConfig->InfluenceMasks[i]);
            filledPieces = filledPieces & (filledPieces -1);
        }
        
        filledPieces = BlackPieces;
        to_Ret[0][4] = 0;
        to_Ret[0][5] = 0;
        while (filledPieces != 0){
            bit_set = filledPieces & ~(filledPieces & (filledPieces -1));
            i = __builtin_ctzl(bit_set);
            to_Ret[0][4] += Popcount((Stacks[i] & ((1 << Heights[i]) - 1)));
            to_Ret[0][5] += Popcount(BlackPieces & gameConfig->InfluenceMasks[i]);
            filledPieces = filledPieces & (filledPieces -1);
        }
        
        to_Ret[1][6] = 0;
        // Group Connected Component Scores
        for (i =0 ; i < size_cw ; i++){
            to_Ret[1][6] += Popcount(WhiteComponents[i]);
        }
        to_Ret[0][6] = 0;
        for (i =0 ; i < size_cb ; i++){
            to_Ret[0][6]  += Popcount(BlackComponents[i]);
        }


        
        return to_Ret;
        
    }
    
};
