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

std::vector<std::vector<int>> ReadBoard(std::string file_name) {
    std::mt19937 mt(std::chrono::high_resolution_clock::now().time_since_epoch().count());
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

    for (int i = 0; i < vec.size(); ++i) {
        for (int j = 0; j < vec[0].size(); ++j) {
            if (vec[i][j] == 1) {
                dots.insert({i, j});
            }
        }
    }
    // std::cout << dots.size() << std::endl;
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

    sf::Time elapsed_time_ghost;
    sf::Clock clock_ghost;
    int time_ghost = 3000;

    while (window.isOpen()) {
        if (new_game_button.isActive()) {
            board.clear();
            dots.clear();
            board = ReadBoard("board.txt");
            delete pacman;
            delete bot;
            delete pink;
            CreateCharacters();
            elapsed_time = sf::milliseconds(0);
            elapsed_time_ghost = sf::milliseconds(0);
            sf::sleep(sf::milliseconds(1000));
            new_game_button.notClicked();
            play_button.notClicked();
            random_button.notClicked();
            smart_button.notClicked();
            time_ghost = 3000;
        }
        window.clear(sf::Color::Black);
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (play_button.isActive()) EventProcessing(event);
            EventButtons(event);
        }
        DrawBoard(window);
        sf::Time delta_time = clock.restart();
        sf::Time delta_time_ghost = clock_ghost.restart();
        if (!CheckEnd(window) && !CheckCrash(window) && play_button.isActive()) {
            elapsed_time += delta_time;
            elapsed_time_ghost += delta_time_ghost;
            int tmp = pacman->get_speed();
            if (elapsed_time > sf::milliseconds(7.0 / pacman->get_speed())) {
                UpdateCharactersPosition();
                elapsed_time = sf::milliseconds(0);
            }
            if (elapsed_time_ghost > sf::milliseconds(time_ghost / pacman->get_speed())) {
                pink->move();
                elapsed_time_ghost = sf::milliseconds(0);
                time_ghost = 5;
            }
            DidPacmanEat();
            DidBotEat();
        }

        DrawCharacters(window);

        window.display();
    }
    return 0;
}