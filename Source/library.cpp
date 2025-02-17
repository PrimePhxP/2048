#include "library.h"
int matrix[4][4];
int dirLine[] = { 1,0,-1,0 };
int dirCol[] = { 0,1,0,-1 };
int score;
GameState* head = NULL;
GameState* current = NULL;
const int max_state = 10;
int state_count = 0;
const int maxn = 3;
HighScore highScore[maxn];
bool checkWin() {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (matrix[i][j] == 2048) {
				return true;
			}
		}
	}
	return false;
}
bool checkLose() {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (matrix[i][j] == 0) {
				return false;
			}
			if (j != 3 && matrix[i][j] == matrix[i][j + 1]) {
				return false;
			}
			if (i != 3 && matrix[i][j] == matrix[i + 1][j]) {
				return false;
			}
		}
	}
	return true;
}
void swap(HighScore& a, HighScore& b) {
	HighScore temp = a;
	a = b;
	b = temp;
}
void sortHighScore() {
	for (int i = 0; i < maxn; ++i) {
		for (int j = i + 1; j < maxn; ++j) {
			if (highScore[i].score < highScore[j].score) {
				swap(highScore[i], highScore[j]);
			}
		}
	}
}
void printHighScore() {
	sortHighScore();
	cout << "Bang Xep Hang:\n";
	for (int i = 0; i < maxn; ++i) {
		cout << highScore[i].name << " " << highScore[i].score << "\n";
	}
}
void initHighScore() {
	for (int i = 0; i < maxn; ++i) {
		highScore[i].score = 0;
		strcpy(highScore[i].name, "null");
	}
}
void loadHighScore() {
	ifstream file;
	file.open("highscore.txt",ios::binary|ios::in);
	if (file.is_open()) {
		for (int i = 0; i < maxn; ++i) {
			file.read(reinterpret_cast<char*>(&highScore[i]), sizeof(highScore[i]));
		}
	}
	file.close();
}
void saveHighScore() {
	sortHighScore();
	ofstream file;
	file.open("highscore.txt", ios::binary | ios::out);
	if (!file.is_open()) {
		cout << "Khong the mo file!\n";
	}
	else {
		for (int i = 0; i < maxn; ++i) {
			file.write(reinterpret_cast<char*>(&highScore[i]), sizeof(highScore[i]));
		}
	}
	file.close();
}
pair<int, int> findRandomUnOccupiedIndex() {
	int col, line;
	while (true) {
		col = rand() % 4;
		line = rand() % 4;
		if (matrix[line][col] == 0) {
			break;
		}
	}
	return { line,col };
}
void addNumber() {
	pair<int, int> start = findRandomUnOccupiedIndex();
	matrix[start.first][start.second] = (1 + (rand()%2))*2;
}
void new_Game() {
	clearState();
	head = current = NULL;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			matrix[i][j] = 0;
		}
	}
	addNumber();
	score = 0;
	addState();
}
void print_Game() {
	system("cls");
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (matrix[i][j] == 0) {
				cout << setw(4) << ".";
			}
			else {
				cout << setw(4) << matrix[i][j];
			}
		}
		cout << "\n";
	}
	cout << "Score: " << score << "\n";
	cout << "n: game moi, q: thoat game, a: trai, s: xuong, d: phai, w: len\n";
	cout << "o: Bang Xep Hang!, p: Luu game va tat, z: mo lai game gan nhat\n";
	cout << "u: undo, r: redo\n";
}
bool canDoMove(int Line, int Col, int nextLine, int nextCol) {
	if (nextLine < 0 || nextLine >= 4 || nextCol < 0 || nextCol >= 4 || (matrix[Line][Col] != matrix[nextLine][nextCol] && matrix[nextLine][nextCol] != 0)) {
		return false;
	}
	return true;
}
void move(int drs) {
	int startL = 0, startC = 0, Lstep = 1, Cstep = 1;
	if (drs == 0) {
		startL = 3;
		Lstep = -1;
	}
	if (drs == 1) {
		startC = 3;
		Cstep = -1;
	}
	bool movePossible, canAddNumber = 0;
	int nextI, nextJ;
	do {
		movePossible = 0;
		for (int i = startL; i >= 0 && i < 4; i += Lstep) {
			for (int j = startC; j >= 0 && j < 4; j += Cstep) {
				nextI = i + dirLine[drs]; nextJ = j + dirCol[drs];
				if (matrix[i][j] != 0 && canDoMove(i, j, nextI, nextJ)) {
					if (matrix[nextI][nextJ] != 0) {
						score += matrix[nextI][nextJ] * 2;
					}
					matrix[nextI][nextJ] += matrix[i][j];
					matrix[i][j] = 0;
					movePossible = canAddNumber = 1;
				}
			}
		}
		print_Game();
	} while (movePossible);
	if (canAddNumber) {
		addNumber();
		addState();
	}
	if (checkWin()) {
		print_Game();
		cout << "YOU WIN!\n";
		cout << "Nhan phim bat ky de tiep tuc\n";
		system("pause");
		if (score > highScore[2].score) {
			cout << "Ban da dat duoc diem cao, hay nhap ten cua ban: ";
			cout << "Nhap ten cua ban: ";
			cin >> highScore[2].name;
			highScore[2].score = score;
			saveHighScore();
		}
	}
	else if (checkLose()) {
		print_Game();
		cout << "YOU LOSE!\n";
		cout << "Nhan phim bat ky de tiep tuc\n";
		system("pause");
		if (score > highScore[2].score) {
			cout << "Ban da dat duoc diem cao, hay nhap ten cua ban: ";
			cout << "Nhap ten cua ban: ";
			cin >> highScore[2].name;
			highScore[2].score = score;
			saveHighScore();
		}
	}
}
void saveGameStatement() {
	ofstream file;
	file.open("game.txt", ios::binary | ios::out);
	if (!file.is_open()) {
		cout << "Khong the mo file!\n";
	}
	else {
		file.write(reinterpret_cast<char*>(&score), sizeof(score));
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				file.write(reinterpret_cast<char*>(&matrix[i][j]), sizeof(matrix[i][j]));
			}
		}
	}
	file.close();
}
void loadGameStatement() {
	ifstream file;
	file.open("game.txt", ios::binary | ios::in);
	if (file.is_open()) {
		file.read(reinterpret_cast<char*>(&score), sizeof(score));
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				file.read(reinterpret_cast<char*>(&matrix[i][j]), sizeof(matrix[i][j]));
			}
		}
	}
	file.close();
}
GameState* getPrevious() {
	if (current == NULL || current == head) {
		return NULL;
	}
	GameState* temp = head;
	while (temp->next != current) {
		temp = temp->next;
	}
	return temp;
}
bool undo() {
	if (current == NULL || current == head) {
		return false;
	}
	GameState* previous = getPrevious();
	if (previous != NULL) {
		current = previous;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				matrix[i][j] = current->matrix[i][j];
			}
		}
		score = current->score;
		return true;
	}
	return false;
}
bool redo() {
	if (current == NULL || current->next == NULL) {
		return false;
	}
	current = current->next;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			matrix[i][j] = current->matrix[i][j];
		}
	}
	score = current->score;
	return true;
}
void addState() {
	if (current != NULL) {
		GameState* temp = current->next;
		while (temp != NULL) {
			GameState* next = temp->next;
			delete temp;
			temp = next;
		}
		current->next = NULL;
	}
	GameState* newState = new GameState;
	newState->score = score;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			newState->matrix[i][j] = matrix[i][j];
		}
	}
	newState->next = NULL;
	if (head == NULL) {
		head = current = newState;
	}
	else {
		current->next = newState;
		current = newState;
	}
	if (state_count < max_state) {
		++state_count;
	}
	else {
		GameState* temp = head;
		head = head->next;
		delete temp;
	}

}
void clearState() {
	GameState* temp = head;
	while (temp != NULL) {
		GameState* next = temp->next;
		delete temp;
		temp = next;
	}
	head = current = NULL;
	state_count = 0;
}
void play() {
	initHighScore();
	loadHighScore();
	score = 0;
	srand(static_cast<unsigned int>(time(0)));
	new_Game();
	while (true) {
		print_Game();
		char sls;
		cin >> sls;
		if (sls == 'n') {
			new_Game();
		}
		else if (sls == 'q') {
			if (score > highScore[2].score) {
				cout << "Ban da dat duoc diem cao, hay nhap ten cua ban: ";
				cout << "Nhap ten cua ban: ";
				cin >> highScore[2].name;
				highScore[2].score = score;
				saveHighScore();
			}
			break;
		}
		else {
			if (sls == 'a') {
				move(3);
			}
			else if (sls == 's') {
				move(0);
			}
			else if (sls == 'd') {
				move(1);
			}
			else if (sls == 'w') {
				move(2);
			}
			else if (sls == 'o') {
				printHighScore();
				system("pause");
				cout << "Nhan phim bat ky de tiep tuc\n";
			}
			else if (sls == 'p') {
				saveGameStatement();
				break;
			}
			else if (sls == 'z') {
				loadGameStatement();
			}
			else if (sls == 'u') {
				undo();
			}
			else if (sls == 'r') {
				redo();
			}
			else {

			}
		}
	}
}