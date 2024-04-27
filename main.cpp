#include <SFML/Graphics.hpp>
#include <chrono>
#include <fstream>
#include <random>
#include <sstream>
#include <vector>

#include "board.h"
#include "gameplay.h"
#include "global.h"
#include "gui.h"

// резкие скачки

std::vector<std::vector<int>> ReadBoard(std::string file_name) {
    std::mt19937 mt(
        std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::ifstream in(file_name);
    std::vector<std::string> str;
    std::string s;
    while (std::getline(in, s)) {
        str.push_back(s);
    }
    std::vector<std::vector<int>> vec(str.size());
    for (int i = 0; i < str.size(); ++i) {
        for (int j = 0; j < str[i].length(); ++j) {
            vec[i].push_back(str[i][j] - '0');
        }
    }

    int energy = 4;
    while (energy) {
        int x = mt() % vec.size();
        int y = mt() % vec[x].size();
        if (vec[x][y] == 1) {
            vec[x][y] = 7;
            --energy;
        }
    }
    return vec;
}

int main() {
    board = ReadBoard("board.txt");
    CreateCharacters();

    int width = board[0].size() * 30;
    int heigth = board.size() * 30;
    sf::RenderWindow window(sf::VideoMode(width, heigth), "Pacman");
    sf::Time elapsed_time;
    sf::Clock clock;

    while (window.isOpen()) {
        window.clear(sf::Color::Black);
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            EventProcessing(event);
        }
        sf::Time delta_time = clock.restart();
        elapsed_time += delta_time;
        if (elapsed_time > sf::milliseconds(7)){
            UpdateCharactersPosition();
            elapsed_time = sf::milliseconds(0);
        }
        DidPacmanEat();
        DidBotEat();
        DrawBoard(window);
        DrawCharacters(window);
        CheckEnd(window);
        window.display();
    }
    return 0;
}