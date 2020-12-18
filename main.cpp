#include "game.h"

#include <string>
#include <iostream>

#include <fstream>
#include <chrono>
#include <thread>

using namespace std;

// player 1 : 1 / X / black
// Player 2 : -1 / O / white

// short sequence 1 to help debugging : f5, d6, c5, f4, e3, f6, g5, e6, e7
// short sequence 2 to help debugging : d3, c5, d6, e3, b4, c3, d2, c4, f4

int main()
{
	// la fonction f(x) = 3 - 2*x
	// renvoie 1 si x = 1
	// renvoie -1 si x = -2

	// source of weight matrix :
	// https://github.com/hylbyj/Alpha-Beta-Pruning-for-Othello-Game/blob/master/readme_alpha_beta.txt
	int weight[8][8] = 	{{4, -3, 2, 2, 2, 2, -3, 4},
						{-3, -4, -1, -1, -1, -1, -4, -3},
						{2, -1, 1, 0, 0, 1, -1, 2},
						{2, -1, 0, 1, 1, 0, -1, 2},
						{2, -1, 0, 1, 1, 0, -1, 2},
						{2, -1, 1, 0, 0, 1, -1, 2},
						{-3, -4, -1, -1, -1, -1, -4, -3},
						{4, -3, 2, 2, 2, 2, -3, 4}};

	int matrix[8][8] = {0};
	matrix[3][3] = -1;
	matrix[3][4] = 1;
	matrix[4][3] = 1;
	matrix[4][4] = -1;

	string player_types[2];


	do {
		cout << "Choisir le type de Joueur 1 (H/A/F) : " << endl;
		cin >> player_types[0];
	}
	while (player_types[0] != "H" && player_types[0] != "A" && player_types[0] != "F");

	do {
		cout << "Choisir le type de Joueur 2 (H/A/F) : " << endl;
		cin >> player_types[1];
	}
	while (player_types[1] != "H" && player_types[1] != "A" && player_types[1] != "F");

	string nom_fichier_noir;
	string nom_fichier_blanc;
	if (player_types[0] == "F" || player_types[1] == "F"){
		cout << "enter path to directory containing blanc.txt and noir.txt : " << endl;
		cin >> nom_fichier_noir;
		nom_fichier_blanc = nom_fichier_noir;
		nom_fichier_noir += "noir.txt";
		nom_fichier_blanc += "blanc.txt";
	} else {
		nom_fichier_noir = "./noir.txt";
		nom_fichier_blanc = "./blanc.txt";
	}

	cout << "Début de la partie" << endl;
	printBoard(matrix);

	ofstream * po;
	ofstream fichier_out_noir(nom_fichier_noir, ios::app);
	ofstream fichier_out_blanc(nom_fichier_blanc, ios::app);
	ifstream * pi;
	ifstream fichier_in_noir(nom_fichier_noir, ios::app);
	ifstream fichier_in_blanc(nom_fichier_blanc, ios::app);

	while(!TERMINAL_TEST(matrix)) {
		for(int i = 1; i<=2; i++){
			if (i == 1) {
				// fichier blanc
				po = &fichier_out_noir;
				pi = &fichier_in_noir;
			}
			else if (i == 2) {
				// fichier blanc
				po = &fichier_out_blanc;
				pi = &fichier_in_blanc;
			}
			if (!TERMINAL_TEST(matrix)) {
				cout << "Turn of player " << i << " : " << endl;
				if (player_types[i-1] == "H") {
					bool move_OK = false;
					string move;
					while(!move_OK){
						cout << "Enter valid move : " << endl;
						cin >> move;
						if (move.length() == 2 && move[0] >= 'a' && move[0] <= 'h' && move[1] >= '1' && move[1] <= '8') {
							int col = (int)move[0] - (int)('a');
							int row = (int)move[1] - (int)('1');
							if (move == "00" && !MOVE_EXISTS(matrix, 3-(2*i) )) {
								move_OK = true;
								cout << "player " << i << " skipped his turn " << endl;
								*po << move << endl;
							} else if (MOVE_TEST( matrix, 3-(2*i), row, col )) {
								move_OK = true;
								EXECUTE_MOVE(matrix, 3-(2*i), row, col);
								cout << "player " << i << " put his piece at " << move << endl;
								*po << move << endl;
							}
						}
					}
				}

				else if (player_types[i-1] == "A") {
					if (MOVE_EXISTS(matrix, 3-(2*i))) {
						string move;
						move = MINIMAX_DECISION(weight, matrix, 3-(2*i));
						int row = (int)move[0] - (int)('0');
						int col = (int)move[1] - (int)('0');
						EXECUTE_MOVE(matrix, 3-(2*i), row, col);
						char char_col = col + 'a';
						cout << "player " << i << " put his piece at " << char_col << row + 1 << endl;
						*po << char_col << row + 1 << endl;
					} else {
						cout << "player " << i << " skipped his turn " << endl;
						*po << "00" << endl;
					}
				}

				else if (player_types[i-1] == "F") {
					while (!pi->is_open())
					{
						cout << "Attente du joueur " << i << " (fichier blanc indisponible)" << endl;
						std::this_thread::sleep_for(std::chrono::milliseconds(2000));
						pi->open(nom_fichier_noir);
					}
					string move;
					bool move_OK = false;
					while (!move_OK) {
						while (!(getline(*pi, move)))
						{
							cout << "Attente du joueur " << i << " (n'a pas encore joué)" << endl;
							pi->clear();
							pi->sync();
							std::this_thread::sleep_for(std::chrono::milliseconds(2000));
						}
						cout << i <<" move : " << move << endl;
						int col = (int)move[0] - (int)('a');
						int row = (int)move[1] - (int)('1');
						if (move == "00" && !MOVE_EXISTS(matrix, 3-(2*i))) {
							move_OK = true;
							cout << "player " << i << " skipped his turn " << endl;
						} else if (MOVE_TEST( matrix, 3-(2*i), row, col )) {
							move_OK = true;
							EXECUTE_MOVE(matrix, 3-(2*i), row, col);
							cout << "player " << i << " put his piece at " << move << endl;
						}
					}
				}

				printBoard(matrix);
			}
		}
	}
	printScore(matrix);
	return 0;
}
