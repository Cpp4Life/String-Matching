#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

static int H, W;

struct Word
{
	int _X, _Y; //Coordinates
	string _Name, _Dir; //Name and Direction
};

char** createCrosswordTable(const string& file, vector<string>& wordsList)
{
	ifstream infile(file);
	if (!infile.is_open())
	{
		cout << "Failed to open file\n";
		return NULL;
	}
	else
	{
		string text;
		getline(infile, text);
		int fst = text.find_first_of(' ');
		W = stoi(text.substr(0, fst)); //get Width
		H = stoi(text.substr(fst + 1)); //get Height

		char** wordsTable = new char* [H + 1];
		for (int i = 0; i < H; i++)
			wordsTable[i] = new char[W + 1];

		int line = 0;
		while (getline(infile, text))
		{
			if (line < H) //get the next H lines
			{
				//remove white space in text
				text.erase(remove(text.begin(), text.end(), ' '), text.end());
				for (int i = 0; i < text.size(); i++)
					wordsTable[line][i] = text[i];
			}
			if (text == "#") //stop reading file when reach '#'
				break;
			if (line >= H) //get meaningful words from (H + 2)th lines
				wordsList.push_back(text);
			line++;
		}

		infile.close();
		return wordsTable;
	}
}

void displayTable(char** table)
{
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
			cout << table[i][j] << ' ';
		cout << endl;
	}
	cout << endl;
}

//2 directions: horizontally left, vertically down
static const int x[2] = { 0, 1 };
static const int y[2] = { 1, 0 };

bool Search2D(char** table, const int& row, const int& col, const string& word, int& direction)
{
	if (table[row][col] != word[0])
		return false;

	//loop through 2 directions
	for (int dir = 0; dir < 2; dir++)
	{
		//create starting point (x, y) for current direction
		int rowDir = row + x[dir], colDir = col + y[dir];
		int k;

		//first character is already matched
		//loop through the rest of word
		for (k = 1; k < word.size(); k++)
		{
			//out of bound -> break
			if (rowDir < 0 || rowDir >= H || colDir < 0 || colDir >= W)
				break;

			//not matched -> break
			if (table[rowDir][colDir] != word[k])
				break;

			//keep moving in a particular direction
			rowDir += x[dir];
			colDir += y[dir];
		}

		//condition when all characters matched 
		if (k == word.size())
		{
			direction = dir;
			return true;
		}
	}
	return false;
}

void patternSearch(char** table, const vector<string>& words)
{
	vector<Word> wordList;
	int count = 0; //the amount of meaningful words in the table
	for (int i = 0; i < words.size(); i++) {
		Word single;
		int exist = false;
		int dir; //direction, 0 is horizontal, 1 is vertical
		for (int row = 0; row < H; row++) {
			for (int col = 0; col < W; col++) {
				if (Search2D(table, row, col, words[i], dir)) {
					single._Name = words[i];
					single._X = row + 1;
					single._Y = col + 1;
					if (dir == 0)
						single._Dir = "LR";
					if (dir == 1)
						single._Dir = "TD";
					wordList.push_back(single);
					exist = true;
				}
			}
		}
		if (!exist)
			wordList.push_back({ 0, 0, words[i], "NF" });
	}

	int NFWord = count_if(wordList.begin(), wordList.end(), [](const Word& other) {return other._Dir == "NF"; });
	cout << wordList.size() - NFWord << endl;
	for (const auto& item : wordList)
		cout << item._Name << " (" << item._X << "," << item._Y << ") " << item._Dir << endl;
}

int main()
{
	//the name of input file
	string fileName;
	cout << "Input a file: ";
	cin >> fileName;

	//contains meaningful words
	vector<string> wordsFound;

	//create the table of crossword game
	char** crosswordTable = createCrosswordTable(fileName, wordsFound);

	//Display table of characters
	displayTable(crosswordTable);

	//Find the meaningful words in table
	patternSearch(crosswordTable, wordsFound);
}