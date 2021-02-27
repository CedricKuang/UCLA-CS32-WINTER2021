#include <queue>
#include <iostream>
#include <string>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_row(rr), m_col(cc) {}
    int r() const { return m_row; }
    int c() const { return m_col; }
private:
    int m_row;
    int m_col;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    queue<Coord> coordQueue;
    coordQueue.push(Coord(sr, sc));
    maze[sr][sc] = 'X';
    while (!(coordQueue.empty()))
    {
        Coord current = coordQueue.front();
        coordQueue.pop();
        if (current.r() == er && current.c() == ec)
            return true;
        if (maze[current.r() - 1][current.c()] != 'X')
        {
            coordQueue.push(Coord(current.r() - 1, current.c()));
            maze[current.r() - 1][current.c()] = 'X';
        }
        if (maze[current.r()][current.c() + 1] != 'X')
        {
            coordQueue.push(Coord(current.r(), current.c() + 1));
            maze[current.r()][current.c() + 1] = 'X';
        }
        if (maze[current.r() + 1][current.c()] != 'X')
        {
            coordQueue.push(Coord(current.r() + 1, current.c()));
            maze[current.r() + 1][current.c()] = 'X';
        }
        if (maze[current.r()][current.c() - 1] != 'X')
        {
            coordQueue.push(Coord(current.r(), current.c() - 1));
            maze[current.r()][current.c() - 1] = 'X';
        }
    }
    return false;
}

int main()
{
    string maze[10] = {
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
}