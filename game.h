#include <string>

void printBoard(int matrix[8][8]);
void printScore(int matrix[8][8]);
bool MOVE_TEST(int matrix[8][8], int player, int row, int col);
bool MOVE_EXISTS(int matrix[8][8], int player);
bool TERMINAL_TEST(int matrix[8][8]);
std::string ACTIONS(int matrix[8][8], int player);
int EVAL(int weight[8][8], int matrix[8][8], int player);
bool CUTOFF_TEST(int matrix[8][8], int depth);
int MAX_VALUE(int depth, int weight[8][8], int matrix[8][8], int player, int MAX);
int MIN_VALUE(int depth, int weight[8][8], int matrix[8][8], int player, int MAX);
std::string MINIMAX_DECISION(int weight[8][8], int matrix[8][8], int player);
void EXECUTE_MOVE(int matrix[8][8], int player, int row, int col);

// rename matrix[][] as state[][]
