#pragma once
#include"SFML/Graphics.hpp"
#include<ctime>
#include<thread>
using namespace sf;
using namespace std;
void InitWindow()
{
    SetConsoleTitle(L"Thuat Toan FLOYD");
    vector<vector<int>>Maze;
    srand(time(NULL));
    int size = 25;
    textcolor(7);
    system("cls");
    cout << "Dang chuan bi me cung!!!\n";
    Maze.resize(size, vector<int>(size, 0));
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            float randomValue = (float)(rand()) / RAND_MAX;  // Ngẫu nhiên từ 0 tới 1
            if (randomValue > 0.3) 
                Maze[i][j] = 1;  // Đặt là đường
        }
    }

    cout << "10%\n";
    vector<vector<int>>Matrix(size * size, vector < int>(size * size, 0));
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
                Matrix[Current][size * (i)+j - 1] = Matrix[size * (i)+j - 1][Current] = 1;
            }
            if (j <= size - 2 && Maze[i][j + 1])
            {
                Matrix[Current][size * i + j + 1] = Matrix[size * i + j + 1][Current] = 1;
            }
        }
    }
    cout << "20%\n";
    Graph G = Graph(size * size);
    int number = 12;
    G.Floyd(Matrix);
    cout << "80%\n";
    vector<queue<int>> Path(number);
    queue<int>Way;
    queue<int>Way_tmp;
    float cellSize = 25.f;
    int min = -1;
    int tmp;
    cout << "90%\n";
    int start[2];
    bool flag = true;
    vector<vector<int>>end(number, vector<int>(2, 0));

loop:
    do
    {
        start[0] = rand() % size;
        start[1] = rand() % size;
    } while (Maze[start[0]][start[1]] == 0);
    for (int i = 0; i < number; i++)
    {
        do
        {
            end[i][0] = rand() % size;
            end[i][1] = rand() % size;
        } while (Maze[end[i][0]][end[i][1]] == 0 || (end[i][0] == start[0] && end[i][1] == start[1]));
    }
    for (int i = 0; i < number; i++)
    {
        
        if (G.Length[start[0] * size + start[1] ][ end[i][0] * size + end[i][1] ]==vc)
            goto loop;
    }
    cout << "100%\n";
    RenderWindow window(VideoMode(cellSize * size, cellSize * size), "MAZE");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        window.clear();


        sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
        cell.setOutlineThickness(1);
        cell.setOutlineColor(Color::Black);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                cell.setPosition(i * cellSize, j * cellSize);
                cell.setFillColor(Maze[i][j] == 0 ? sf::Color::Black : sf::Color::White);
                window.draw(cell);
            }
        }
        if (flag)
        {
            tmp = 10000000;
            min = -1;
            for (int i = 0; i < number; i++)
            {
                if (tmp > G.Length[start[0] * size + start[1]][end[i][0] * size + end[i][1]])
                {
                    tmp = G.Length[start[0] * size + start[1]][end[i][0] * size + end[i][1]];
                    min = i;
                }
            }
                Path[min] = G.TruyVet(start[0] * size + start[1] + 1, end[min][0] * size + end[min][1] + 1);
            flag = false;
        }
        if (!Path[min].empty())
        {
            Way.push(Path[min].front());
            Path[min].pop();
        }
        if (Path[min].empty())
        {

            start[0] = end[min][0];
            start[1] = end[min][1];
            Path.erase(Path.begin() + min);
            end.erase(end.begin() + min);
            number--;
            flag = true;
            while (!Way.empty())
                Way.pop();
        }
        Way_tmp = Way;
        while (!Way_tmp.empty())
        {
            int currentCell = Way_tmp.front();
            int col = (currentCell - 1) / size;  // Adjust the index
            int row = (currentCell - 1) % size;  // Adjust the index

            cell.setPosition(col * cellSize, row * cellSize);  // Swap row and col
            cell.setFillColor(sf::Color::Blue);
            window.draw(cell);
            Way_tmp.pop();
        }
        cell.setPosition(start[0] * cellSize, start[1] * cellSize);
        cell.setFillColor(sf::Color::Green);
        window.draw(cell);
        for (int i = 0; i < number; i++)
        {
            cell.setPosition(end[i][0] * cellSize, end[i][1] * cellSize);
            cell.setFillColor(sf::Color::Red);
            window.draw(cell);
        }

        window.display();
        if (number == 0)
        {
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    cell.setPosition(i * cellSize, j * cellSize);
                    cell.setFillColor(Maze[i][j] == 0 ? sf::Color::Black : sf::Color::White);
                    window.draw(cell);
                }
            }
            cell.setPosition(start[0] * cellSize, start[1] * cellSize);
            cell.setFillColor(sf::Color::Green);
            window.draw(cell);
            window.display();
            cout << "Window closed. Exiting...\n";
            std::this_thread::sleep_for(std::chrono::seconds(3));
            window.close();
        }
    }
}