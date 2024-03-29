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



void debugGame(){
	
	double WhiteScores[] = { 40,  80, 20,  25, 3, 2, 4, 2, 12.5, -7.5, 12.5, -5, 15, -2.5};
 double WhiteScores2[] = {60, 120, 20, 30, 4, 2, 4, 2, 10,   10,   20,   10, 15, 10};
//	double WhiteScores[] = {40, 80, 20, 5, 3, 2, 20, 2, 12.5, -7.5, 12.5, -5, 15, -2.5};
//	double WhiteScores2[] = {60,  120, 20, 30, 4, 2, 4, 2, 10, 10, 20, 10, 15, 10};
	Game myGame(7, 2,WhiteScores,WhiteScores);
	myGame.applyMove(myGame.makeMove("Fa5",true));
	myGame.applyMove(myGame.makeMove("Fe5",true));
	myGame.applyMove(myGame.makeMove("Fb2"));
	myGame.applyMove(myGame.makeMove("Fc3"));
	myGame.applyMove(myGame.makeMove("Fc2"));
	myGame.applyMove(myGame.makeMove("Fb3"));
	myGame.applyMove(myGame.makeMove("Fd2"));
	myGame.applyMove(myGame.makeMove("Fd3"));
	myGame.applyMove(myGame.makeMove("Fa2"));
	myGame.applyMove(myGame.makeMove("1b3-1"));
	myGame.applyMove(myGame.makeMove("1c2+1"));
	myGame.applyMove(myGame.makeMove("Fb3"));
	myGame.applyMove(myGame.makeMove("2c3<2"));
	myGame.applyMove(myGame.makeMove("Fd4"));
	myGame.applyMove(myGame.makeMove("Fd5"));
	myGame.applyMove(myGame.makeMove("Fc4"));
	myGame.applyMove(myGame.makeMove("Fc5"));
	myGame.applyMove(myGame.makeMove("Fe3"));
	myGame.applyMove(myGame.makeMove("Fb5"));
	myGame.applyMove(myGame.makeMove("Fb4"));
	myGame.applyMove(myGame.makeMove("3b3+3"));
	myGame.applyMove(myGame.makeMove("2b2+11"));
	myGame.applyMove(myGame.makeMove("1b3+1"));
	myGame.applyMove(myGame.makeMove("1a5>1"));
	myGame.applyMove(myGame.makeMove("2b4+2"));
	myGame.applyMove(myGame.makeMove("1c4+1"));
	myGame.applyMove(myGame.makeMove("3b5>3"));
	myGame.applyMove(myGame.makeMove("1d4+1"));
	myGame.applyMove(myGame.makeMove("4c5>4"));
	myGame.applyMove(myGame.makeMove("Fc3"));
		//myGame.applyMove(myGame.makeMove("Fa4"));
	printGameState(myGame);
	int Depth = 1;
	MiniMaxAgent player(1,7,150,WhiteScores,WhiteScores2,Depth);
	player.myGame = &myGame;
	player.myGame->currentPlayer = 1;
	cout << "My Move : " << player.getMiniMaxMove() << endl;
	exit(0);
}

int main(int argc, char** argv){
	
	//	srand((1475664597));
	//	cerr << "SRAND KEY : " << time(NULL) << endl;
	
	srand(time(NULL));
	Slides = GenerateAllSlides(7);
		//debugGame();

	double WhiteScores[] = { 40,  80, 20,  15, 3, 2, 4, 2, 12.5, -7.5, 17.5, -5, 15, -2.5};
	double WhiteScores2[] = {60, 120, 20, 30, 4, 2, 4, 2, 10,   10,   20,   10, 15, 10};
	double BlackScores[] = {50, 90, 30, 15, 10, 3, 4, 2, 12.5, -7.5, 12.5, -5, 15, -2.5};
	double BlackScores2[] = {60, 120, 20, 30, 4, 2, 4, 2, 10, 10, 20, 10, 15, 10};
	
	int MaxDepth = 4;
	int p, n;
	double t;
	cin >> p >> n >> t;
	timeval currenttime;
	gettimeofday(&currenttime, NULL);
	if ( p == 1){
		MiniMaxAgent player3(p, n, t,WhiteScores,WhiteScores2, MaxDepth, &currenttime);
		//	cerr << "MiniMax Player With Bits" << endl;
		player3.playFirstMove();
		player3.play();
	}
	else{
		MiniMaxAgent player3(p, n, t,WhiteScores,BlackScores2, MaxDepth, &currenttime);
		//	cerr << "MiniMax Player With Bits" << endl;
		player3.playFirstMove();
		player3.play();
	}
	return 0;
}
