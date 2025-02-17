#ifndef THUVIEN_H
#define THUVIEN_H
#include<iostream>
#include<ctime>
#include<cstdlib>
#include<iomanip>
#include<fstream>
#include<string.h>
using namespace std;
struct GameState {
    int matrix[4][4];
    int score;
    GameState* next;
};
struct HighScore {
	char name[50];
	int score;
};
void move(int drs);
bool canDoMove(int Line, int Col, int nextLine, int nextCol);
void swap(HighScore& a, HighScore& b);
void print_Game();
void new_Game();
void addNumber();
pair<int, int> findRandomUnOccupiedIndex();
void loadHighScore();
bool checkWin();
bool checkLose();
void saveHighScore();
void sortHighScore();
void printHighScore();
void loadHighScore();
void initHighScore();
GameState* getPrevious();
void saveGameStatement();
bool undo();
bool redo();
void loadGameStatement();
void clearState();
void addState();
void play();
#endif