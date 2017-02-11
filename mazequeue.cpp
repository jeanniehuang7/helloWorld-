#include <string>
#include <queue>
#include <iostream>
using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool mark_if_undiscovered(string maze[], queue <Coord> &coordQueue, int nRows, int nCols, Coord b)
{
	int row = b.r();
	int col = b.c(); //don't push if the coordinate is out of bounds, a wall, already discovered, or the starting point. 
					 //spec said you don't have to check if out of bounds, but I did anyway. 
	if (row < 0 || col < 0 || row > nRows - 1 || col > nCols - 1 || maze[row].at(col) == 'X' || maze[row].at(col) == '#' || maze[row].at(col) == 'S')
		return false;
	else
	{
		coordQueue.push(b);
		maze[row].replace(col, 1, "#");
		//cerr << "I pushed coordinate " << row << "," << col << " onto the stack." << endl;
	}
	return true;
}

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	queue<Coord> coordQueue;
	Coord start(sr, sc);	//save, and update start/ending positions
	Coord end(er, ec);
	maze[sr].replace(sc, 1, "S");
	maze[er].replace(ec, 1, "E");

	coordQueue.push(start); //push starting coordinate onto the queue

	//int numPopped = 0; 
	while (!coordQueue.empty())
	{
		Coord a = coordQueue.front();
		coordQueue.pop();

		int aRow = a.r(); //current row
		int aCol = a.c(); //current column
		//numPopped++;
		//cerr << "I popped coordinate " << aRow << "," << aCol << " off of the stack." << endl << "I popped " << numPopped << " coordinates.\n";

		if (aRow == end.r() && aCol == end.c()) //if coord a == coord end
			return true;
		else //check in all 4 directions
		{
			Coord north(aRow - 1, aCol);
			mark_if_undiscovered(maze, coordQueue, nRows, nCols, north);

			Coord east(aRow, aCol + 1);
			mark_if_undiscovered(maze, coordQueue, nRows, nCols, east);

			Coord south(aRow + 1, aCol);
			mark_if_undiscovered(maze, coordQueue, nRows, nCols, south);

			Coord west(aRow, aCol - 1);
			mark_if_undiscovered(maze, coordQueue, nRows, nCols, west);
		}
	}
	return false;
}


int main()
{
	const int size = 10;

	string maze[size] = {
		"XXXXXXXXXX",
		"X........X",
		"XX.X.XXXXX",
		"X..X.X...X",
		"X..X...X.X",
		"XXXX.XXX.X",
		"X.X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;

	string x;
	for (int i = 0; i < size; i++)
	{
		cout << maze[i] << endl;
	}
	cin >> x;
}