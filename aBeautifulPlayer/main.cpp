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
	double WhiteScores[] = {6,1,5,7,6,7,4,6,6,4,1,6,5};
	Game myGame(7, 2,WhiteScores);
	
	/*
	 myGame.applyMove(myGame.makeMove("Fe1",true));
	 myGame.applyMove(myGame.makeMove("Fe5",true));
	 myGame.applyMove(myGame.makeMove("Fd2"));
	 myGame.applyMove(myGame.makeMove("Fd1"));
	 myGame.applyMove(myGame.makeMove("Fd4"));
	 myGame.applyMove(myGame.makeMove("1d1+1"));
	 myGame.applyMove(myGame.makeMove("Fc2"));
	 myGame.applyMove(myGame.makeMove("Fd3"));
	 myGame.applyMove(myGame.makeMove("Fe3"));
	 myGame.applyMove(myGame.makeMove("1d3>1"));
	 myGame.applyMove(myGame.makeMove("Cd3"));
	 myGame.applyMove(myGame.makeMove("Cc4"));
	 myGame.applyMove(myGame.makeMove("Fc3"));
	 myGame.applyMove(myGame.makeMove("1c4>1"));
	 myGame.applyMove(myGame.makeMove("1d3>1"));
	 myGame.applyMove(myGame.makeMove("Fc1"));
	 myGame.applyMove(myGame.makeMove("3e3-12"));
	 myGame.applyMove(myGame.makeMove("1c1+1"));
	 myGame.applyMove(myGame.makeMove("Fe3"));
	 myGame.applyMove(myGame.makeMove("Fe4"));
	 myGame.applyMove(myGame.makeMove("1e5-1"));
	 myGame.applyMove(myGame.makeMove("2d4>2"));
	 myGame.applyMove(myGame.makeMove("1e2<1"));
	 myGame.applyMove(myGame.makeMove("2c2>2"));
	 myGame.applyMove(myGame.makeMove("Sc2"));
	 myGame.applyMove(myGame.makeMove("3d2>3"));
	 myGame.applyMove(myGame.makeMove("3e1+3"));
	 myGame.applyMove(myGame.makeMove("Fd4"));
	 myGame.applyMove(myGame.makeMove("Fc4"));
	 myGame.applyMove(myGame.makeMove("1d4<1"));
	 myGame.applyMove(myGame.makeMove("Fb2"));
	 myGame.applyMove(myGame.makeMove("Fd4"));
	 myGame.applyMove(myGame.makeMove("Fb3"));
	 myGame.applyMove(myGame.makeMove("Fb4"));
	 myGame.applyMove(myGame.makeMove("1c3+1"));
	 myGame.applyMove(myGame.makeMove("1b4>1"));
	 myGame.applyMove(myGame.makeMove("Sc3"));
	 myGame.applyMove(myGame.makeMove("4c4<22"));
	 myGame.applyMove(myGame.makeMove("Sc4"));
	 myGame.applyMove(myGame.makeMove("Fa3"));
	 myGame.applyMove(myGame.makeMove("1c4<1"));
	 myGame.applyMove(myGame.makeMove("Sc4"));
	 myGame.applyMove(myGame.makeMove("Fa2"));
	 myGame.applyMove(myGame.makeMove("Sd3"));
	 myGame.applyMove(myGame.makeMove("256"));
	 myGame.applyMove(myGame.makeMove("Fb1"));
	 myGame.applyMove(myGame.makeMove("Fd5"));
	 myGame.applyMove(myGame.makeMove("Fb5"));
	 myGame.applyMove(myGame.makeMove("1a3>1"));
	 myGame.applyMove(myGame.makeMove("4e2<31"));
	 myGame.applyMove(myGame.makeMove("1d2-1"));
	 
	 */
	
	
	myGame.applyMove(myGame.makeMove("Fa5",true));
	myGame.applyMove(myGame.makeMove("Fe5",true));
	myGame.applyMove(myGame.makeMove("Fe4"));
	myGame.applyMove(myGame.makeMove("Fb4"));
	myGame.applyMove(myGame.makeMove("Fd4"));
	myGame.applyMove(myGame.makeMove("Cd3"));
	myGame.applyMove(myGame.makeMove("Cc4"));
	
	myGame.applyMove(myGame.makeMove("1d3+1"));
	myGame.applyMove(myGame.makeMove("Fb3"));
	myGame.applyMove(myGame.makeMove("1b4-1"));
	myGame.applyMove(myGame.makeMove("Fc5"));
	myGame.applyMove(myGame.makeMove("2d4>2"));
	myGame.applyMove(myGame.makeMove("Fc3"));
	myGame.applyMove(myGame.makeMove("Fb2"));
	myGame.applyMove(myGame.makeMove("Fc2"));
	myGame.applyMove(myGame.makeMove("1b2>1"));
	myGame.applyMove(myGame.makeMove("Fb5"));
	myGame.applyMove(myGame.makeMove("Fb2"));
	myGame.applyMove(myGame.makeMove("Sa3"));
	myGame.applyMove(myGame.makeMove("Fd3"));
	myGame.applyMove(myGame.makeMove("1a3>1"));
	myGame.applyMove(myGame.makeMove("Fa3"));
	myGame.applyMove(myGame.makeMove("Fb4"));
	myGame.applyMove(myGame.makeMove("Fa4"));
	myGame.applyMove(myGame.makeMove("Fd5"));
	myGame.applyMove(myGame.makeMove("Fa2"));
	myGame.applyMove(myGame.makeMove("2b3<2"));
	myGame.applyMove(myGame.makeMove("Fd2"));
	myGame.applyMove(myGame.makeMove("3a3-3"));
	myGame.applyMove(myGame.makeMove("3e4+3"));
	myGame.applyMove(myGame.makeMove("Fa3"));
	myGame.applyMove(myGame.makeMove("1a5>1"));
	myGame.applyMove(myGame.makeMove("Fd4"));
	myGame.applyMove(myGame.makeMove("1a4-1"));
	myGame.applyMove(myGame.makeMove("4a2>4"));
	myGame.applyMove(myGame.makeMove("4e5<112"));
	myGame.applyMove(myGame.makeMove("Fe4"));
	myGame.applyMove(myGame.makeMove("4b5-22"));
	myGame.applyMove(myGame.makeMove("2c5>2"));
	myGame.applyMove(myGame.makeMove("Sa5"));
	myGame.applyMove(myGame.makeMove("Fc5"));
	myGame.applyMove(myGame.makeMove("3b3>12"));
	myGame.applyMove(myGame.makeMove("1c3-1"));
	myGame.applyMove(myGame.makeMove("1d2<1"));
	myGame.applyMove(myGame.makeMove("1b2>1"));
	myGame.applyMove(myGame.makeMove("3b2+111"));
	myGame.applyMove(myGame.makeMove("2c2<2"));
	myGame.applyMove(myGame.makeMove("1b3>1"));
	myGame.applyMove(myGame.makeMove("3c2+3"));
	
	printGameState(myGame);
	
	int Depth = 5;
	MiniMaxAgent player(1,5,150,WhiteScores,Depth);
	player.myGame = &myGame;
	player.myGame->currentPlayer = 1;
	cout << "My Move : " << player.getMiniMaxMove() << endl;
	exit(0);
}

int main(int argc, char** argv){
	
	//	cerr << "SRAND KEY : " << time(NULL) << endl;
	//	srand((1475664597));
	//	cerr << "SRAND KEY : " << time(NULL) << endl;

	srand(time(NULL));
	Slides = GenerateAllSlides(5);
	//	debugGame();
	//
	//	double WhiteScores[] = {30,-0.227125,30,15.6073,30,30,30,30,30,-1.3207,2.87015,14.3374,21.2475};
	//
	//	FlatScore = Scores[0];
	//	StandingStoneScore = Scores[1];
	//	CapStoneScore = Scores[2];
	//	CenterScore = Scores[3];
	//	InfluenceScore = Scores[4];
	//	LibertyScore = Scores[5];
	//	GroupLibertyScore = Scores[6];
	//	HardCaptiveFlat = Scores[7];
	//	SoftCaptiveFlat = Scores[8];
	//	HardCaptiveStand = Scores[9];
	//	SoftCaptiveStand = Scores[10];
	//	HardCaptiveCap = Scores[11];
	//	SoftCaptiveCap = Scores[12];
	
	double WhiteScores[] = {60, 20, 30, 4, 2, 4, 2, 10, 10, 20, 10, 15, 10};
	double BlackScores[] = {60, 20, 30, 4, 2, 4, 2, 10, 10, 20, 10, 15, 10};
	//	double BlackScores[] = {6,1,5,7,6,7,4,6,6,4,1,6,5};
	//	double WhiteScores[] = {30,2.2591,21.2596,15.1804,30,21.7065,30,9.29319,30,1.66427,15.5,4.63779,8.09963};
	//	double BlackScores[] = {30,2.2591,21.2596,15.1804,30,21.7065,30,9.29319,30,1.66427,15.5,4.63779,8.09963};
	//	double BlackScores[] = {30,-0.227125,30,15.6073,30,30,30,30,30,-1.3207,2.87015,14.3374,21.2475};
	
	int MaxDepth = 4;
	int p, n;
	double t;
	cin >> p >> n >> t;
	timeval currenttime;
	gettimeofday(&currenttime, NULL);
	if ( p == 1){
		MiniMaxAgent player3(p, n, t,WhiteScores, MaxDepth, &currenttime);
		//	cerr << "MiniMax Player With Bits" << endl;
		player3.playFirstMove();
		player3.play();
	}
	else{
		MiniMaxAgent player3(p, n, t,BlackScores, MaxDepth, &currenttime);
		//	cerr << "MiniMax Player With Bits" << endl;
		player3.playFirstMove();
		player3.play();
	}
	return 0;
}
