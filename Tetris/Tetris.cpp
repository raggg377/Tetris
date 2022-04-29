// Visual studio is good for console development in c++
// Biggest Concept=> i=y*w+x//
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <time.h> // for sleep functions so that blocks drop periodically
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h> //CreateConsoleScreenBuffer and GetAsyncKeyState

using namespace std;
// Console Size
// Full Screen data (213,50)
int sWidth = 80;
int sHeight = 30;
// Field Size
int fWidth = 15;
int fHeight = 18;

unsigned char* playField = nullptr; // not a part of block class

class Blocks
{
public:
	vector<string> pieces;
	int score;

	int getScore();
	void addScore(int n);
	bool canBlockMove(int a, int b, int c, int d);	   // O(1)
	int Rotate(int positionX, int positionY, int rot); // O(1)
};

// Function to rotate blocks
int Blocks::Rotate(int positionX, int positionY, int rot)
{
	int positionIndex = 0;
	rot = rot % 4;
	if (rot == 0)
		positionIndex = (positionY + 0) * 4 + (positionX + 0);
	if (rot == 1)
		positionIndex = 12 + positionY - (positionX * 4);
	if (rot == 2)
		positionIndex = 15 - (positionY * 4) - positionX;
	if (rot == 3)
		positionIndex = 3 - positionY + (positionX * 4);

	return positionIndex;
}

// 0					90					180					270
//  0  1  2  3		//12  8  4  0		//15 14 13 12		// 3  7 11 15
//  4  5  6  7		//13  9  5  1		//11 10  9  8		// 2  6 10 14
//  8  9 10 11		//14 10  6  2 		// 7  6  5  4		// 1  5  9 13
// 12 13 14 15		//15 11  7  3		// 3  2  1  0		// 0  4  8 12

// Function to add score
void Blocks::addScore(int n)
{
	score += n;
}

// Function to return Score
int Blocks::getScore()
{
	return score;
}

// Function whether block can fit
bool Blocks::canBlockMove(int nPieces, int n_Rotation, int nPosX, int nPosY)
{
	// All FieldIndex >0 means there is already a piece
	for (int positionX = 0; positionX < 4; positionX++)
		for (int positionY = 0; positionY < 4; positionY++)
		{
			// Get index of piece
			int positionIndex = Rotate(positionX, positionY, n_Rotation);

			// Get index into field
			// Translated our 4*4 block into the playing field
			// shifting of origin to position of current block
			int fieldIndex = (nPosY + positionY) * fWidth + (nPosX + positionX);

			// Checking for bounds
			if (nPosX + positionX >= 0 && nPosX + positionX < fWidth)
			{
				if (nPosY + positionY >= 
					.
					0 && nPosY + positionY < fHeight)
				{
					// In Bounds so do collision check
					// If piece has * and playField is occupied then no movement possible
					if ((pieces.at(nPieces)[positionIndex] != '_') && (playField[fieldIndex] != 0))
						return false; // fail
				}
			}
		}

	return true;
}

template <typename DT>
class SinglyLinkedNode
{
public:
	// data items in the node
	DT dataItem;
	SinglyLinkedNode<DT>* next;

	// constructor
	SinglyLinkedNode(DT nodeValue, SinglyLinkedNode<DT>* nextPointer);
};

template <typename DT>
SinglyLinkedNode<DT>::SinglyLinkedNode(DT nodeValue, SinglyLinkedNode* nextPointer)
{
	dataItem = nodeValue;
	next = nextPointer;
}

template <typename DT>
class SinglyLinkedList
{
	// We declare these members protected because we will inherit this class later
protected:
	// Data Members
	SinglyLinkedNode<DT>* head;
	SinglyLinkedNode<DT>* cursor;
	int numberOfItems;

public:
	// Constructor
	SinglyLinkedList();
	void insert(const DT& newDataItem);
	void clear();
	bool isEmpty();
	DT getCursor();
	DT size();
};

template <typename DT>
SinglyLinkedList<DT>::SinglyLinkedList()
{
	head = NULL;
	cursor = NULL;
	numberOfItems = 0;
}

template <typename DT>
void SinglyLinkedList<DT>::insert(const DT& newDataItem)
{
	if (numberOfItems == 0)
	{
		head = new SinglyLinkedNode<DT>(newDataItem, NULL);
		cursor = head;
	}
	else
	{
		SinglyLinkedNode<DT>* new_node = new SinglyLinkedNode<DT>(newDataItem, cursor->next);
		cursor->next = new_node;
		cursor = cursor->next;
	}
	numberOfItems++;
}

template <typename DT>
void SinglyLinkedList<DT>::clear()
{
	cursor = head;
	while (cursor->next != NULL)
	{
		SinglyLinkedNode<DT>* nextNode = cursor->next;
		delete cursor;
		cursor = nextNode;
	}
	numberOfItems = 0;
	head = NULL;
	cursor = NULL;
}

template <typename DT>
bool SinglyLinkedList<DT>::isEmpty()
{
	return numberOfItems == 0;
}

template <typename DT>
DT SinglyLinkedList<DT>::getCursor()
{
	return cursor->dataItem;
}

template <typename DT>
DT SinglyLinkedList<DT>::size()
{
	return numberOfItems;
}

int main()
{
	Blocks tetris; // created object for Block class

	//A screen buffer is where the output is showm in a console window. SHOW THE WINDOW. It is an array of characters of size swidth*sheight 
	// we defined theM as 80 30 earlier as this is the default size of the window AND THEN SHOW IN PROPERTIES
	//**wchar_t or Wide char is similar to char data type,a char can take upto 256 values but wide char takes more space and can store much larger values
	wchar_t* screen = new wchar_t[sWidth * sHeight];

	// Initializing it every value is initially made blank 
	for (int i = 0; i < sWidth * sHeight; i++)
		screen[i] = L' ';

	// Now we call create console screen buffer function which creates a screen buffer 
	// And If the function succeeds, the return value is a handle to the new console screen buffer.
	//** A handle is a pointer or index with no visible type attached to it.
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	// A console can have multiple screen buffers. The active screen buffer is the one that is displayed on the screen. 
	//Makes the specified screen buffer to be the currently displayed console screen buffer.
	SetConsoleActiveScreenBuffer(hConsole);

	// And we create a byteswritten variable which is also part of the screen buffer code
	//** A dword, which is short for "double word," is a data type which when defined is an unsigned, 32-bit unit of data 

	DWORD dwBytesWritten = 0;

	//Now we create the diff shapes. There are total 7 shapes in tetris which are **DRAW THE SHAPES i shape, square, t shape, zigzag and l shape
	// But here we can also create a block of any shape we want
	// Now we store them as pieces which we can imagine as a 4*4 block stored in 1D form in row major order and _ denotes empty space and * denotes occupied
	//** Motivation for storing as 1d comes very useful in rotation
	//**  We did not use multi dimentional strings as we would have to create 28 blocks
	//  				 "0123456789101112131415"
	tetris.pieces.push_back("__*___*___*___*_"); // 0  1  2  3																
	tetris.pieces.push_back("__*__**___*_____"); // 4  5  6  7
	tetris.pieces.push_back("_____**__**_____"); // 8  9 10 11
	tetris.pieces.push_back("__*__**__*______"); // 12 13 14 15
	tetris.pieces.push_back("_*___**___*_____");
	tetris.pieces.push_back("_*___*___**_____");
	tetris.pieces.push_back("__*___*__**_____");

	// Now we create playfield which is the region where the blocks fall         SHOW PLAYFIELD
	// playfield as we declared earlier is a pointer to character array of size fieldwidth*fieldheight
	playField = new unsigned char[fWidth * fHeight];

	//Now we initialize the playfield
	for (int x = 0; x < fWidth; x++) // Board Boundary
		for (int y = 0; y < fHeight; y++)
			if (x == 0 || x == fWidth - 1 || y == fHeight - 1)
				playField[y * fWidth + x] = 9; // 9 has significance which will be seen later
			else
				playField[y * fWidth + x] = 0;

	// Game Logic
	bool controls[4];  // for keys
	int currPiece = 0; // curr shape
	int currRotation = 0;
	// currX and currY determine initial coordinates of the block
	int currX = (fWidth / 2) - 3;
	int currY = 0;
	int speed = 20;				// speed helps in managing the speed of the game basically time taken for block to fall 1 step down is given by speed * 50ms
	int countSpeed = 0;				   // basically after every 50ms its value is incremented and after it becomes equal to speed(which we defined 20 earlier) then down is made true
	bool down = false;				   // if its true the block goes one step down 
	bool hold = true;				   // prevents extra rotations if held continuously i.e 1 spacebar =1 rotation
	int blockCount = 0;				   //
	SinglyLinkedList<int> linkedLines; // accepts the completed line and clears it
	bool endGame = false;			   // if its true GameOver gg!!

	while (!endGame) // Main Loop
	// this while loop is executed continuously in the background till the game does not end
	{
		// Timing =======================
		//Now we call sleep for method of the thread class which stops the execution of the current thread for a specific duration of time 
		//We use this to control our speed of the game 
		this_thread::sleep_for(50ms); // Small Step = 1 Game Tick which takes 50 ms
		countSpeed++;
		down = (countSpeed == speed);

		// Input ========================
		for (int k = 0; k < 4; k++)
			//GetAsyncKeyState() Determines whether a key is pressed at the time the function is called,
			//And it  gets the key state asynchronously, i.e., without waiting for anything 
			//Parameters passed are The virtual-key code 				// ->   <- Down Space
			controls[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28\x20"[k]))) != 0; // asynchoronus key state

		// Game Logic ===================

		// Handle player movement
		if (controls[0]) //->  is pressed
		{
			if (tetris.canBlockMove(currPiece, currRotation, currX + 1, currY))
				currX = currX + 1;

			else
				currX = currX + 0;
		}

		if (controls[1]) //<- is pressed
		{
			if (tetris.canBlockMove(currPiece, currRotation, currX - 1, currY))
				currX = currX - 1;
			else
				currX = currX - 0;
		}
		if (controls[2]) // down is pressed
		{
			if (tetris.canBlockMove(currPiece, currRotation, currX, currY + 1))
				currY = currY + 1;
			else
				currY = currY + 0;
		}


		if (controls[3]) // spacebar is pressed
		{
			//We use hold to ensure that if spacebar is pressed continuously the block does not rotate wildy 
			// Demonstrate by removing hold
			// When we leave spacebar in those 50ms, the else will get executed and hold will become true
			if (hold && tetris.canBlockMove(currPiece, currRotation + 1, currX, currY))

				currRotation = currRotation + 1;
			else
				currRotation = currRotation + 0;

			hold = false;
		}
		else
			hold = true;

		// Force the piece down the playfield if it's time
		if (down)
		{
			countSpeed = 0;
			blockCount++;
			// decreases the time after drop of 50 times
			if (blockCount % 50 == 0)
				if (speed >= 10)
					speed--;

			// Test if piece can be moved down
			if (tetris.canBlockMove(currPiece, currRotation, currX, currY + 1))
				currY++;
			else
			{
				// It can't move more! Hence we will fix the block
				for (int positionX = 0; positionX < 4; positionX++) // traverse all indexes
					for (int positionY = 0; positionY < 4; positionY++)
					{
						// checks whether the positionIndex of block is occupied
						if (tetris.pieces.at(currPiece)[tetris.Rotate(positionX, positionY, currRotation)] != '_')
							// If it is occupied by * then at that index playField is updated by the new respective piece value
							//  we add one to use the string array " #######=*"
							playField[(currY + positionY) * fWidth + (currX + positionX)] = currPiece + 1;
					}

				// After Fixing the block we will Check for line completion
				for (int positionY = 0; positionY < 4; positionY++)
					if (currY + positionY < fHeight - 1) // the whole block should be inside the boundary
					{
						bool line = true;
						for (int positionX = 1; positionX < fWidth - 1; positionX++)
							// The checking will begin from:
							// Y of cuurent block i.e. the topmost line i.e starting from the top of 4*4 block
							// X=0 of that line.
							// And it will go on till the 4th line

							// we are using bitwise and operator just for fun
							// Alernate code
							//  if ((playField[(currY + positionY) * fWidth + positionX]) == 0)
							//	{
							//	line = false;
							//	break;
							//	}
							line = line & ((playField[(currY + positionY) * fWidth + positionX]) != 0);

						if (line)
						{
							// Remove Line, set to =
							for (int positionX = 1; positionX < fWidth - 1; positionX++)
								playField[(currY + positionY) * fWidth + positionX] = 8;
							linkedLines.insert(currY + positionY); // The Y of the line which is getting deleted is stored in list
						}
					}

				tetris.addScore(50); // Adding 50 points for each block

				if (!linkedLines.isEmpty())
					tetris.addScore((linkedLines.size()) * 100); // Adding 100 points for each line

				//  New Piece
				currX = (fWidth / 2) - 3;
				currY = 0;
				currRotation = 0;
				srand(time(0));
				currPiece = rand() % 7; // Since we have 7 blocks

				// If piece does not fit straight away, game over!
				endGame = !tetris.canBlockMove(currPiece, currRotation, currX, currY);
			}
		}

		// Display =======================================================================================

		// Draw Field
		for (int x = 0; x < fWidth; x++)
			for (int y = 0; y < fHeight; y++)
				// We can change the symbol for each piece L" ABCDEFG=*"
				screen[(y + 2) * sWidth + (x + 2)] = L" #######=*"[playField[y * fWidth + x]];

		// Draw Current Piece
		for (int positionX = 0; positionX < 4; positionX++)
			for (int positionY = 0; positionY < 4; positionY++)
				if (tetris.pieces.at(currPiece)[tetris.Rotate(positionX, positionY, currRotation)] != '_')
					screen[(currY + positionY + 2) * sWidth + (currX + positionX + 2)] = 35; // 35 for # 65 for Alphabets

		// Draw score
		swprintf_s(&screen[2 * sWidth + fWidth + 6], 16, L"score: %8d", tetris.getScore());

		// Animate Line Completion
		if (!linkedLines.isEmpty())
		{
			// Display Frame (cheekily to draw lines)
			// Reflects the output of the screen buffer
			WriteConsoleOutputCharacter(hConsole, screen, sWidth * sHeight, { 0, 0 }, &dwBytesWritten);
			this_thread::sleep_for(400ms); // Delay a bit
			int i = 0;
			while (i != linkedLines.size())
			{
				for (int positionX = 1; positionX < fWidth - 1; positionX++)
				{
					for (int positionY = linkedLines.getCursor(); positionY > 0; positionY--)
						playField[positionY * fWidth + positionX] = playField[(positionY - 1) * fWidth + positionX];
					playField[positionX] = 0;
				}
				i++;
			}
			linkedLines.clear();
		}

		// Display Frame
		WriteConsoleOutputCharacter(hConsole, screen, sWidth * sHeight, { 0, 0 }, &dwBytesWritten);
	}

	CloseHandle(hConsole);
	cout << "Well Played Try next time.." << endl;
	cout << "SCORE:" << tetris.getScore() << endl;
	system("pause"); // Pauses the console
	return 0;
}