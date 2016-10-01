//
//  Game.cpp
//  aBeautifulPlayer
//
//  Created by Akshay Singla on 01/10/16.
//  Copyright © 2016 Shantanu Kumar. All rights reserved.
//

#include "Game.hpp"

#define BLACK 0
#define WHITE 1
#define MAX_SIZE 7

enum MoveType { PlaceFlat = 0,
                PlaceStanding,
                PlaceCapstone,
                SlideLeft,
                SlideRight,
                SlideUp,
                SlideDown
            };

class Move{
    MoveType MoveType;
    short int row;
    short int column;
    short int Drops[MAX_SIZE] = {};
    short int dropLength;
};

class Config{
    int BoardSize;
    int Pieces;
    int Capstones;
    
    unsigned long long Left;
    unsigned long long Right;
    unsigned long long Top;
    unsigned long long Bottom;
    unsigned long long Edge;
    unsigned long long BoardMask;
    
    Config(int BoardSize){
        
    }
};

class Game{
    unsigned long long WhitePieces;
    unsigned long long BlackPieces;
    unsigned long long CapStones;
    unsigned long long Standing;
    unsigned long long Stacks[MAX_SIZE*MAX_SIZE] = {};
    unsigned long long Heights[MAX_SIZE*MAX_SIZE] = {};
    
    unsigned long moves;
    bool currentPlayer = 0;
    
    Game(){
        WhitePieces = 0;
        BlackPieces = 0;
        CapStones = 0;
        Standing = 0;
    }
    
    void applyMove(Move move){
        
    }
    
    
    
    
    
};
