#include <iostream>
#include <algorithm>
#include "game.h"

void printBoard(int matrix[8][8]){
	std::cout << "\n  a b c d e f g h  \n";
	for (int i = 0; i < 8; i++) {
		std::cout << i+1 << " ";
		for (int j = 0; j < 8; j++) {
			if (matrix[i][j] == 1) {
				std::cout << "X ";
			} else if (matrix[i][j] == -1) {
				std::cout << "O ";
			} else {
				std::cout << ". ";
			}
		}
		std::cout << i+1 << "\n";
	}
	std::cout << "  a b c d e f g h  \n\n";
}

void printScore(int matrix[8][8]){
	int scoreP1 = 0;
	int scoreP2 = 0;
	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			if (matrix[i][j] == 1){
				scoreP1++;
			} else if (matrix[i][j] == -1) {
				scoreP2++;
			}
		}
	}
	std::cout << "GAME OVER\n" << "score is :\n" << "P1 : " << scoreP1 << "\nP2 : " << scoreP2 << std::endl;
}

bool MOVE_TEST(int matrix[8][8], int player, int row, int col){
    if (matrix[row][col] == 0) {
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				int k = 1;
				while ( col+k*i >= 0 && col+k*i <= 7 && row+k*j >= 0 && row+k*j <= 7 && !(i==0 && j==0)) {
					// tant que les indices restent bien dans les limites du tableau, et que i et j ne sont pas nuls en même temps
					if (matrix[row+k*j][col+k*i] == -player) {
						k++;
					} else if (matrix[row+k*j][col+k*i] == player && k>1) {
						return true;
					} else {
						break;
					}
				}
			}
		}
	}
	return false;
}

bool MOVE_EXISTS(int matrix[8][8], int player){
    for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			if (MOVE_TEST(matrix, player, i, j)) {
				return true;
			}
		}
	}
	return false;
}

bool TERMINAL_TEST(int matrix[8][8]){
    if (MOVE_EXISTS(matrix, 1) || MOVE_EXISTS(matrix, -1)) {
		return false;
	}
	return true;
}

std::string ACTIONS(int matrix[8][8], int player){
	std::string actions = "";
	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			if (MOVE_TEST(matrix, player, i, j)) {
				actions += std::to_string(i);
				actions += std::to_string(j);
				// format : row, col
			}
		}
	}
	return actions;
}

int EVAL(int weight[8][8], int matrix[8][8], int MAX){
	int sum = 0;
	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			if (matrix[i][j] == MAX) {
				sum+=weight[i][j];
				// sum++;
			}
		}
	}
	return sum;
}


bool CUTOFF_TEST(int matrix[8][8], int depth){
	int max_depth = 11;
	if ( (depth > max_depth) || TERMINAL_TEST(matrix) ){
		return true;
	}
	return false;
}

int MAX_VALUE(int depth, int weight[8][8], int matrix[8][8], int player, int MAX, int alpha, int beta){
	if ( CUTOFF_TEST(matrix, depth) ){
		return EVAL(weight, matrix, MAX);
	}
	int v = -10000;
	player = -player;
	std::string actions = ACTIONS(matrix, player);
	for (int i = 0; i < actions.length(); i += 2) {
		int row = (int)actions[i]  - (int)('0');
		int col = (int)actions[i + 1] - (int)('0');

		int matrix_copy[8][8] = {0};
		for (int x = 0 ; x < 8; x++){
  			for (int y = 0 ; y < 8; y++){
    			matrix_copy[x][y] = matrix[x][y];
  			}
		}
		EXECUTE_MOVE(matrix_copy, player, row, col);
		v = std::max(v, MIN_VALUE(depth + 1, weight, matrix_copy, player, MAX, alpha, beta));
		if (v >=beta ){
			return v;
		}
		alpha = std::max(alpha, v);
	}
	return v;
}

int MIN_VALUE(int depth, int weight[8][8], int matrix[8][8], int player, int MAX,int alpha, int beta){
	if ( CUTOFF_TEST(matrix, depth) ){
		return EVAL(weight, matrix, MAX);
	}
	int v = 10000;
	player = -player;
	std::string actions = ACTIONS(matrix, player);
	for (int i = 0; i < actions.length(); i += 2) {
		int row = (int)actions[i]  - (int)('0');
		int col = (int)actions[i + 1] - (int)('0');

		int matrix_copy[8][8] = {0};
		for (int x = 0 ; x < 8; x++){
  			for (int y = 0 ; y < 8; y++){
    			matrix_copy[x][y] = matrix[x][y];
  			}
		}
		EXECUTE_MOVE(matrix_copy, player, row, col);
		v = std::min(v, MAX_VALUE(depth + 1, weight, matrix_copy, player, MAX, alpha, beta));
		if (v <= alpha){
			return v;
		}
		beta = std::min(beta, v);
	}
	return v;
}


std::string MINIMAX_DECISION(int weight[8][8], int matrix[8][8], int player){
	std::string action = "ER";
	std::string actions = ACTIONS(matrix, player);
	int v = -10001;
	int depth = 1;
	int MAX = player;
	int alpha = -10000;
	int beta = -10000;
	for (int i = 0; i < actions.length(); i += 2) {
		int row = (int)actions[i]  - (int)('0');
		int col = (int)actions[i + 1] - (int)('0');
		int matrix_copy[8][8] = {0};
		for (int x = 0 ; x < 8; x++){
  			for (int y = 0 ; y < 8; y++){
    			matrix_copy[x][y] = matrix[x][y];
  			}
		}
		EXECUTE_MOVE(matrix_copy, player, row, col);
		int temp = MIN_VALUE(depth + 1, weight, matrix_copy, player, MAX, alpha,beta);
		if (temp > v) {
			v = temp;
			action = "";
			action += std::to_string(row);
			action += std::to_string(col);
		}
	}
	return action;
}


void EXECUTE_MOVE(int matrix[8][8], int player, int row, int col){
	bool MoveIsValid = false;
	if (matrix[row][col] == 0) {
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				int k = 1;
				while ( col+k*i >= 0 && col+k*i <= 7 && row+k*j >= 0 && row+k*j <= 7 && !(i==0 && j==0)) {
					// tant que les indices restent bien dans les limites du tableau, et que i et j ne sont pas nuls en même temps
					if (matrix[row+k*j][col+k*i] == -player) {
						k++;
					} else if (matrix[row+k*j][col+k*i] == player && k>1) {
						for (int l = k-1; l >= 0; l--) {
								matrix[row+l*j][col+l*i] = player;
						}
						MoveIsValid = true;
						break;
					} else {
						break;
					}
				}
			}
		}
	}
}
