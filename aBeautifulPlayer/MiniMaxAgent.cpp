//
//  MiniMaxAgent.cpp
//  aBeautifulPlayer
//
//  Created by Shantanu Kumar on 01/10/16.
//  Copyright © 2016 Shantanu Kumar. All rights reserved.
//

#include <stdio.h>
#include <sys/time.h>
#include "Game.cpp"

#define INF 50000


#define LOSE_DEPTH 2
#define MID_DEPTH 3
#define MAX_DEPTH 4




struct MiniMaxAgent;

struct Helper{
	struct MiniMaxAgent * x;
	int z;
};

struct Container{
	int val;
	Move movex;
};
struct MiniMaxAgent{
	int StatesExplored;
	int SIZE_ALL_MOVES;
	bool QUIT = false;
	int ALPHA;
	int BETA;
	int MAXSTATEVALUE;
	Move ALLMOVES[1000];
	Move* BESTMOVE;
	pthread_t threads[4];
	pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
	
	Game* myGame;
	timeval lasttimenoted;
	timeval currenttime;
	int myPlayerNumber;
	string myPlayer;
	int boardSize;
	int CurrentMaxDepth;
	int maxDepthAllowed = 4;
	bool ROAD_WIN = false;
	bool FLAT_WIN = false;
	bool LOST_FLAG = false;
	bool WIN_FLAG = false;
	bool ONE_STEP_FLAG = false;
	bool GO_TO_LOWEST_DEPTH = false;
	short Win_Iterations = 0;
	short Lost_Iterations = 0;
	double timeLeft;
	short int moves;
	
	MiniMaxAgent (int playerNum, int n, double t,double Scores [],double Scores2 [], int MaxDepthIn,timeval* starttime =NULL ){
		myPlayerNumber = playerNum % 2;
		if (starttime)
			lasttimenoted = *starttime;
		else
			gettimeofday(&lasttimenoted,NULL);
		myPlayer = "Black";
		if (playerNum == 1)
			myPlayer = "White";
		boardSize = n;
		timeLeft = t;
		maxDepthAllowed = MaxDepthIn;
		CurrentMaxDepth = MaxDepthIn;
		moves = 0;
		myGame = new Game(boardSize, myPlayerNumber,Scores,Scores2);
	}
	
	void playFirstMove(){
		string moveString;
		Move move;
		moves++;
		if (myPlayerNumber == 0){
			cin >> moveString;
			cerr << "Received Move String" << endl;
			gettimeofday(&lasttimenoted,NULL);
			myGame->applyMove(myGame->makeMove(moveString, true));
			
			vector<Move> allMoves = myGame->generateFirstMove();
			move = allMoves[rand()%allMoves.size()];
			moveString = myGame->getMoveString(move);
			
			myGame->applyMove(move);
			cout << moveString << endl;
			cerr << "Sent Move String" <<  endl;
			TimeandDepth();
		}
		
		if (myPlayerNumber == 1){
			
			vector<Move> allMoves = myGame->generateFirstMove();
			move = allMoves[rand()%allMoves.size()];
			moveString = myGame->getMoveString(move);
			
			myGame->applyMove(move);
				// // cerr << "My Move : " << moveString << endl;
			cout << moveString << endl;
			cerr << "Sent Move String" <<  endl;
			TimeandDepth();
				// // cerr << "Time left : " << timeLeft << endl;
			cin >> moveString;
			cerr << "Received Move String" << endl;
			gettimeofday(&lasttimenoted,NULL);
			myGame->applyMove(myGame->makeMove(moveString, true));
		}
	}
	
	void play(){
		string move;
		
		if (myPlayerNumber == 0){
			cin >> move;
			cerr << "Received Move String" << endl;
			gettimeofday(&lasttimenoted, NULL);
			myGame->applyMove(myGame->makeMove(move));
			moves++;
		}
		
		
		while(true){
				//chooseDefOrAttack();
			move = getMiniMaxMove();
				// // cerr << "My Move as " << myPlayer << "Player :" << move << endl;
			cout << move << endl;
			cerr << "Sent Move String" <<  endl;
			moves++;
			TimeandDepth();
			myGame->applyMove(myGame->makeMove(move));
			cin >> move;
			cerr << "Received Move String" << endl;
				// // cerr << timeLeft << endl;
			gettimeofday(&lasttimenoted, NULL);
			myGame->applyMove(myGame->makeMove(move));
			moves++;
		}
			//cout << move << endl;
	}
	
	void chooseDefOrAttack(){
		double Z =120 +  myGame->getStateValue();
		if (Z  < 0)
			myGame->ToAttack ^= 1;
		if (myGame->ToAttack)
			cerr << Z <<"\t Attack" << endl;
		else
			cerr << Z << "\t Defense" << endl;
	}
	
	inline void TimeandDepth(){
		
			//myGame->gameConfig->FlatScoreOffset = myGame->gameConfig->Pieces - min(myGame->flats[0], myGame->flats[1]);
		gettimeofday(&currenttime, NULL);
		timeLeft -= currenttime.tv_sec - lasttimenoted.tv_sec;
		timeLeft -= (currenttime.tv_usec - lasttimenoted.tv_usec)/1000000.0;
		if (GO_TO_LOWEST_DEPTH){
			CurrentMaxDepth = 1;
			GO_TO_LOWEST_DEPTH = false;
		}
		else if ( LOST_FLAG || timeLeft < 30){
			CurrentMaxDepth = LOSE_DEPTH;
			LOST_FLAG = false;
		}
		else if (WIN_FLAG || timeLeft < 75)
			CurrentMaxDepth = MID_DEPTH;
		else
			CurrentMaxDepth = MAX_DEPTH;
	}
	
	
	static void * InternalRunHelper(void * This) {
		struct Helper curr = *(Helper *) This;
		curr.x->ParallelSearch(curr.z);
		return NULL;
	}
	
	void * ParallelSearch(int I){
		int left = (SIZE_ALL_MOVES/4)*I;
		int Right = min((SIZE_ALL_MOVES/4)*(I+1), SIZE_ALL_MOVES );
		for (int i=left; i<Right; i++){
			Game nextState = *myGame;
			nextState.applyMove(ALLMOVES[i]);
			
			int value = MiniMaxSearch(nextState, false, 1, ALPHA, BETA);
			if (!QUIT && value > MAXSTATEVALUE){
				pthread_mutex_lock(&mutex1);
				if (!QUIT && value > MAXSTATEVALUE){
					MAXSTATEVALUE = value;
					BESTMOVE = &ALLMOVES[i];
				}
				pthread_mutex_unlock(&mutex1);
			}
			pthread_mutex_lock(&mutex2);
			ALPHA = max(ALPHA, MAXSTATEVALUE);
			pthread_mutex_unlock(&mutex2);
			if (!QUIT && MAXSTATEVALUE == ROAD_REWARD ){
				ROAD_WIN = true;
				QUIT= true;
				break;
			}
			if (!QUIT && MAXSTATEVALUE == ROAD_REWARD/2 ){
				QUIT= true;
				FLAT_WIN = true;
			}
			if (BETA < ALPHA)
				break;
		}
		pthread_exit(NULL);
	}
	
	string getMiniMaxMoveParallel(){
		QUIT = false;
		SIZE_ALL_MOVES = myGame->generateAllMoves(ALLMOVES);
		
		MAXSTATEVALUE = -INF;
		ALPHA = -INF;
		BETA = INF;
		BESTMOVE = &ALLMOVES[0];
		
		for (int i = 0; i < 4; i ++){
			struct Helper help;
			help.x = this;
			help.z = i;
			pthread_create( &threads[i], NULL, InternalRunHelper, (void *)&help );
		}
		
		for (int i = 0; i < 4; i++)
			pthread_join(threads[i],NULL);
		
		QUIT  = false;
		if (MAXSTATEVALUE < -ROAD_REWARD/2 + 1){
			LOST_FLAG = 1;
			if (Lost_Iterations++ >= 5){
				GO_TO_LOWEST_DEPTH = true;
				LOST_FLAG = 0;
				Lost_Iterations = 0;
			}
		}
		else{
			Lost_Iterations = 0;
		}
		
		if (ROAD_WIN || FLAT_WIN){
			WIN_FLAG = true;
			for (int i=0; i<SIZE_ALL_MOVES; i++){
				Game nextState = *myGame;
				nextState.applyMove(ALLMOVES[i]);
				int r = nextState.isFinishState();
				if (r == (myPlayerNumber)){
						// cerr << "1 Move Win/Loss" << endl;
					BESTMOVE = &ALLMOVES[i];
					break;
				}
				if (!ROAD_WIN && FLAT_WIN && r == myPlayerNumber + 4 ){
						// cerr << "direct flat win" << endl;
					BESTMOVE = &ALLMOVES[i];
					break;
				}
			}
		}
		
		if (WIN_FLAG && Win_Iterations++ >= 5){
				// cerr << "GOING TO LOWEST DEPTH" << endl;
			GO_TO_LOWEST_DEPTH = true;
			WIN_FLAG = false;
			Win_Iterations = 0;
		}
		else{
			Win_Iterations = 0;
		}
		
		ROAD_WIN =false;
		FLAT_WIN = false;
			//// // cerr << "ONE_STEP_FLAG : " << ONE_STEP_FLAG << endl << endl;
		cerr << "Max State Value was  : " << MAXSTATEVALUE << endl;
		return myGame->getMoveString(*BESTMOVE);
	}
	
	vector<Container> reorder_nodes(Move * allMoves, int K, bool Max){
		vector<Container> to_ret;
		for (int i = 0 ; i < K; i++){
			Game nextState = *myGame;
			nextState.applyMove(allMoves[i]);
			nextState.short_val();
			to_ret.push_back({nextState.Short_Val,allMoves[i]});
		}
		if (!Max){
		sort(to_ret.begin(), to_ret.end(),
			 [](const Container& a, const Container& b)
			 {
			 return a.val > b.val;
			 });
		}
		else{
			sort(to_ret.begin(), to_ret.end(),
				 [](const Container& a, const Container& b)
				 {
				 return a.val < b.val;
				 });
		}
		return to_ret;
		
	}
	
	string getMiniMaxMove(){
		StatesExplored = 0;
		Move allMoves[1000];
		int size_all_moves = myGame->generateAllMoves(allMoves);
		
		int maxStateValue = -INF;
		int alpha = -INF, beta = INF;
		Move* bestMove = &allMoves[0];
			// cerr << "Minimax with Depth " << CurrentMaxDepth << " At Move number" << moves << endl;
		
		vector<Container> AllGames = reorder_nodes(allMoves, size_all_moves,1);
		for (int i=0; i<size_all_moves; i++){
			Game nextState = *myGame;
			nextState.applyMove(AllGames[i].movex);
			StatesExplored++;
			
			int value = MiniMaxSearch(nextState, false, 1, alpha, beta);
			if (value > maxStateValue){
				maxStateValue = value;
				bestMove = &AllGames[i].movex;
			}
			alpha = max(alpha, maxStateValue);
			if (maxStateValue == ROAD_REWARD ){
				ROAD_WIN = true;
				break;
			}
			if (maxStateValue == ROAD_REWARD/2 ){
				FLAT_WIN = true;
			}
			if (beta < alpha)
				break;
		}
		
		if (maxStateValue < -ROAD_REWARD/2 + 1){
			LOST_FLAG = 1;
			if (Lost_Iterations++ >= 5){
				GO_TO_LOWEST_DEPTH = true;
				LOST_FLAG = 0;
				Lost_Iterations = 0;
			}
		}
		else{
			Lost_Iterations = 0;
		}
		
		if (ROAD_WIN || FLAT_WIN){
			WIN_FLAG = true;
			for (int i=0; i<size_all_moves; i++){
				Game nextState = *myGame;
				nextState.applyMove(allMoves[i]);
				int r = nextState.isFinishState();
				if (r == (myPlayerNumber)){
						// cerr << "1 Move Win/Loss" << endl;
					bestMove = &allMoves[i];
					break;
				}
				if (!ROAD_WIN && FLAT_WIN && r == myPlayerNumber + 4 ){
						// cerr << "direct flat win" << endl;
					bestMove = &allMoves[i];
					break;
				}
			}
		}
		
		if (WIN_FLAG && Win_Iterations++ >= 5){
				// cerr << "GOING TO LOWEST DEPTH" << endl;
			GO_TO_LOWEST_DEPTH = true;
			WIN_FLAG = false;
			Win_Iterations = 0;
		}
		else{
			Win_Iterations = 0;
		}
		
		ROAD_WIN =false;
		FLAT_WIN = false;
			//// // cerr << "ONE_STEP_FLAG : " << ONE_STEP_FLAG << endl << endl;
		cerr << "Max State Value was  : " << maxStateValue << "\t At Depth " <<  CurrentMaxDepth <<"\t States Explored " << StatesExplored <<endl;
		return myGame->getMoveString(*bestMove);
	}
	
	int MiniMaxSearch (Game &gameState, bool maximize, int depth, int alpha, int beta){
		StatesExplored++;
			// if (QUIT)
			//    return -ROAD_REWARD;
		int winner = gameState.isFinishState();
		if (winner != -1){
			if (winner == myPlayerNumber)
				return ROAD_REWARD;
			else if (winner + 4 == myPlayerNumber)
				return ROAD_REWARD/2;
			else if (winner == (myPlayerNumber + 1)%2)
				return -ROAD_REWARD;
			return -ROAD_REWARD/2;
		}
		
		if (depth>CurrentMaxDepth)
			return gameState.getStateValue();
		
		Move allMoves[1000];
		int size_all_moves = gameState.generateAllMoves(allMoves);
		int bestValue = INF;
		if (maximize)
			bestValue = -INF;
		
		vector<Container> AllGames = reorder_nodes(allMoves, size_all_moves,maximize);
		for (int i=0; i<size_all_moves; i++){
				//if (QUIT)
				//    return -ROAD_REWARD;
			Game nextState(gameState);
			nextState.applyMove(AllGames[i].movex);
			int value = MiniMaxSearch(nextState, !maximize, depth+1, alpha, beta);
			if (maximize){
				bestValue = max(bestValue, value);
				alpha = max(alpha, bestValue);
				if ((beta < alpha) || (bestValue == ROAD_REWARD))
					break;
			}
			else {
				bestValue = min(bestValue, value);
				beta = min(beta, bestValue);
				if ((beta < alpha) || ((bestValue == -ROAD_REWARD)))
					break;
			}
		}
		
		return bestValue;
	}
	
};
