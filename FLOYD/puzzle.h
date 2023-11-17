#pragma once
#include"SFML/Graphics.hpp"
#include<ctime>
#include<thread>
using namespace sf;
using namespace std;
void InitWindow()
{
    vector<vector<int>>Maze;
    srand(time(NULL));
    int size = 23;
    textcolor(7);
    system("cls");
    cout << "Dang chuan bi me cung!!!\n";
    Maze.resize(size, vector<int>(size, 0));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            float randomValue = static_cast<float>(rand()) / RAND_MAX;  // Random value between 0 and 1

            if (randomValue > 0.4) {
                Maze[i][j] = 1;  // Set as path
            }
        }
    }

    cout << "10%\n";
    vector<vector<int>>Matrix(size*size, vector < int>(size*size, 0));
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (Maze[i][j] == 0)
                continue;
            int Current = size * i + j;
            if (i >= 1 && Maze[i - 1][j])
            {
                Matrix[Current][size * (i - 1) + j] = Matrix[size * (i - 1) + j][Current] = 1;
            }
            if (i <= size - 2 && Maze[i + 1][j])
            {
                Matrix[Current][size * (i + 1) + j] = Matrix[size * (i + 1) + j][Current] = 1;
            }
            if (j >= 1 && Maze[i][j - 1])
            {
                Matrix[Current][size * (i) + j-1] = Matrix[size * (i) + j-1][Current] = 1;
            }
            if (j <= size - 2 && Maze[i][j + 1])
            {
                Matrix[Current][size * i + j + 1] = Matrix[size * i + j + 1][Current] = 1;
            }
        }
    }
    cout << "20%\n";
    Graph G=Graph(size*size);
    
    G.Floyd(Matrix);
    cout << "80%\n";
    queue<int> Path;
    queue<int>Way;
    queue<int>Way_tmp;
    cout << "90%\n";
    int start[2],end[2];
    
loop:
    do
    {
        start[0] = rand() % size ;
        start[1] = rand() % size ;
    } while (Maze[start[0]][start[1]] == 0);
    do
    {
        end[0] = rand() % size ;
        end[1] = rand() % size ;
    } while (Maze[end[0]][end[1]] == 0&& end[0]==start[0]&& end[1]==start[1]);
    Path = G.TruyVet(start[0] * size + start[1] + 1, end[0] * size + end[1] + 1);
    if (Path.empty())
        goto loop;
    cout << "100%\n";
    RenderWindow window(VideoMode(40 * size, 40 * size), "MAZE");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        window.clear();
        
        float cellSize = 40.f;
        sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
        cell.setOutlineThickness(1);
        cell.setOutlineColor(Color::Black);
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                cell.setPosition(i * cellSize, j * cellSize);
                cell.setFillColor(Maze[i][j] == 0 ? sf::Color::Black : sf::Color::White);
                window.draw(cell);
            }
        }
       if(!Path.empty())
        {
           Way.push(Path.front());
           Path.pop();
        }
       Way_tmp = Way;
       while (!Way_tmp.empty())
       {
           int currentCell = Way_tmp.front();
           int col = (currentCell - 1) / size;  // Adjust the index
           int row = (currentCell - 1) % size;  // Adjust the index

           cell.setPosition(col* cellSize, row* cellSize);  // Swap row and col
           cell.setFillColor(sf::Color::Blue);
           window.draw(cell);
           Way_tmp.pop();
       }
       cell.setPosition(start[0] * cellSize, start[1] * cellSize);
       cell.setFillColor(sf::Color::Green);
       window.draw(cell);
       cell.setPosition(end[0] * cellSize, end[1] * cellSize);
       cell.setFillColor(sf::Color::Red);
       window.draw(cell);
       
        window.display();
    }
}
