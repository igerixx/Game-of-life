#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <iostream>
#include <fstream>

const int width = 1920;
const int height = 955;

const int cellSize = 5;
const int cellNumX = width / cellSize;
const int cellNumY = height * 0.9f / cellSize;

bool cells[cellNumX][cellNumY];

namespace cell {
    void clear(bool cells[cellNumX][cellNumY]) {
        for (int i = 0; i < cellNumX; i++) {
            for (int j = 0; j < cellNumY; j++) {
                cells[i][j] = false;
            }
        }
    }

    void randomCells(bool cells[cellNumX][cellNumY]) {
        for (int i = 0; i < cellNumX; i++) {
            for (int j = 0; j < cellNumY; j++) {
                cells[i][j] = rand() % 2;
            }
        }
    }

    void save(bool cells[cellNumX][cellNumY], std::string path) {
        std::ofstream file(path, std::ios::out);
        for (int i = 0; i < cellNumX; i++) {
            for (int j = 0; j < cellNumY; j++) {
                file << cells[i][j];
            }
            file << '\n';
        }
        file.close();
    }

    void load(bool cells[cellNumX][cellNumY], std::string path) {
        int strCount = 0;
        std::string str;
        std::ifstream file(path);
        while (std::getline(file, str)) {
            for (int i = 0; i < str.length(); i++) {
                if (str[i] != '\n') {
                    cells[strCount][i] = str[i] - '0';
                }
            }
            strCount++;
        }
        file.close();
    }

    bool update(bool cells[cellNumX][cellNumY], int x, int y) {
        int neighbors = 0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (cells[(i + x + cellNumX) % cellNumX][(j + y + cellNumY) % cellNumY] && (i != 0 || j != 0))
                    ++neighbors;
            }
        }

        // S3/B23
        if (cells[x][y]) 
            return neighbors == 2 || neighbors == 3;

        return neighbors == 3;
    }

    void fullUpdate(bool cells[cellNumX][cellNumY]) {
        bool newCells[cellNumX][cellNumY];
        for (int i = 0; i < cellNumX; i++) {
            for (int j = 0; j < cellNumY; j++) {
                newCells[i][j] = cell::update(cells, i, j);
            }
        }

        for (int i = 0; i < cellNumX; i++) {
            for (int j = 0; j < cellNumY; j++) {
                cells[i][j] = newCells[i][j];
            }
        }
    }
}

int main() {
    unsigned long long generations = 0;

    int count = 0, count1 = 0, count2 = 0, count3 = 0, evens = 0;

    float time = 50.0f;

    bool play = true;
    bool grid = false;

    cell::randomCells(cells);

    std::cout << "n = random cells\nc = clear cells\np = stop/continue cells\nleft/right mouse button = draw/erase cells\ng = show/close grid (with grid it's workin slower)\nCtrl + s = save position of cells\nf = load cells from save.txt\nu = speed up time\nl = slow down time\n0x - speedest, 20x - slowest" << std::endl;

    sf::Font font;
    font.loadFromFile("assets/Koulen-Regular.ttf");

    sf::Texture texturePause;
    texturePause.loadFromFile("assets/pause.png");

    sf::Text text, about;
    text.setFont(font);
    about.setFont(font);

    about.setString("Game of life");
    about.setPosition(width * 0.45f, height * 0.916f);
    about.setFillColor(sf::Color::Black);
    about.setCharacterSize(height * 0.1f / 2);

    sf::RenderWindow window(sf::VideoMode(width, height), "Game of life");

    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
    cell.setFillColor(sf::Color::White);

    sf::RectangleShape ground(sf::Vector2f(width, height * 0.1f));
    ground.setFillColor(sf::Color::White);
    ground.setPosition(0, height * 0.9f);

    sf::RectangleShape pause(sf::Vector2f(height * 0.1f / 2, height * 0.1f / 2));
    pause.setPosition(width - height * 0.1f, height - height * 0.1f / 1.3);
    pause.setTexture(&texturePause);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !count3) {
            cell::save(cells, "saves/save.txt");
            count3++;
        }
        else count3 = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
            cell::load(cells, "saves/save.txt");
            play = false;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
            cell::load(cells, "saves/gun.txt");
            play = false;
            generations = 0;
        }

        float deltaTime = clock.restart().asSeconds();
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::N) && !count2) {
            cell::randomCells(cells);
            play = false;
            generations = 0;
        }
        else count2 = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
            cell::clear(cells);
            play = false;
            generations = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
            grid = !count ? !grid : grid;
            count++;
        }
        else count = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            play = !count1 ? !play : play;
            count1++;
        }
        else count1 = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::U) && time > 1) time -= 50 * deltaTime;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && time < 200) time += 50 * deltaTime;

        if (time > 201)
            while (time > 201) 
                time--;

        if (time < 1)
            while (time < 1) 
                time++;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            play = false;
            if (sf::Mouse::getPosition(window).x > 0 && sf::Mouse::getPosition(window).y > 0 && sf::Mouse::getPosition(window).x < width && sf::Mouse::getPosition(window).y < height) {
                
                // 1 x 1 size
                
                cells[(int)(sf::Mouse::getPosition(window).x / cellSize)][(int)(sf::Mouse::getPosition(window).y / cellSize)] = true;

                // Custom size

                // int size = 10;
                // for (int i = 0; i < size; i++)
                //     for (int j = 0; j < size; j++)
                //         cells[(int)(sf::Mouse::getPosition(window).x / cellSize + i)][(int)(sf::Mouse::getPosition(window).y / cellSize + j)] = true;
            }
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            play = false;
            if (sf::Mouse::getPosition(window).x > 0 && sf::Mouse::getPosition(window).y > 0 && sf::Mouse::getPosition(window).x < width && sf::Mouse::getPosition(window).y < height) {
                
                // 1 x 1 size
                
                cells[(int)(sf::Mouse::getPosition(window).x / cellSize)][(int)(sf::Mouse::getPosition(window).y / cellSize)] = false;

                // Custom size

                // int size = 50;
                // for (int i = 0; i < size; i++)
                //     for (int j = 0; j < size; j++)
                //         cells[(int)(sf::Mouse::getPosition(window).x / cellSize + i)][(int)(sf::Mouse::getPosition(window).y / cellSize + j)] = false;
            }
        }

        if (play) {
            std::this_thread::sleep_for(std::chrono::milliseconds((int)time));

            cell::fullUpdate(cells);
            generations++;

            pause.setFillColor(sf::Color(0, 0, 0, 0));
        }
        else {
            pause.setFillColor(sf::Color(255, 0, 0, 255));
        }

        
        text.setString("Generations passed " + std::to_string(generations) + "\nSpeed is " + std::to_string((int)(time * 0.1f)) + "x");

        window.clear(sf::Color::Black);

        int x = 1;
        for (int i = 0; i < cellNumX; i++) {
            for (int j = 0; j < cellNumY; j++) {
                ++x;
                if (cellSize >= 5) {
                    if (grid) {
                        if (cells[i][j]) cell.setFillColor(sf::Color::White);
                        else if (x % 2 && cellSize < 20) cell.setFillColor(sf::Color::Black); 
                        else if (cellSize < 20) cell.setFillColor(sf::Color(20,20,20));
                        else if ((i + j) % 2) cell.setFillColor(sf::Color::Black);
                        else cell.setFillColor(sf::Color(20,20,20));
                        cell.setPosition(i * cellSize, j * cellSize);
                        window.draw(cell);
                    }
                    else {
                        if (cells[i][j]) {
                            cell.setFillColor(sf::Color::White);
                            cell.setPosition(i * cellSize, j * cellSize);
                            window.draw(cell);
                        }
                    }
                }
                else if (cells[i][j]) {
                    cell.setFillColor(sf::Color::White);
                    cell.setPosition(i * cellSize, j * cellSize);
                    window.draw(cell);
                }
            }
        }

        window.draw(ground);

        text.setCharacterSize(height * 0.1f / 3);
        text.setFillColor(sf::Color::Black);
        text.setPosition(height * 0.005f, height * 0.9f);
        window.draw(text);

        window.draw(about);
        window.draw(pause);

        window.display();
    }
}
