#include "gameplay.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "global.h"

void CreateCharacters() {
    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board[i].size(); ++j) {
            if (board[i][j] == 5) {
                pacman = new Character(i, j, "pacman_texture.png");
                board[i][j] = 3;
            }
        }
    }
}

// add or 7
void EventProcessing(sf::Event &event) {
    if (event.type == sf::Event::KeyPressed) {
        auto position = pacman->getPosition();
        if (event.key.code == sf::Keyboard::Right &&
            pacman->getDirection() != Direction::RIGHT) {
            if (position.second + 1 < board[0].size() &&
                (board[position.first][position.second + 1] == 3 || board[position.first][position.second + 1] == 1 ||
                 board[position.first][position.second + 1] == 7)) {
                if (pacman->getDirection() == Direction::UP ||
                    pacman->getDirection() == Direction::DOWN) {
                    pacman->setDelta(0);
                } else if (pacman->getDirection() == Direction::LEFT) {
                    pacman->setDelta((kCell - pacman->getDelta()) % kCell);
                    pacman->setPosition(position.first, position.second - 1);
                }
                pacman->setDirection(Direction::RIGHT);
            } else {
                pacman->setNextDirection(Direction::RIGHT);
            }
        } else if (event.key.code == sf::Keyboard::Left &&
                   pacman->getDirection() != Direction::LEFT) {
            if (position.second > 0 &&
                (board[position.first][position.second - 1] == 3 ||
                 board[position.first][position.second - 1] == 1 ||
                 board[position.first][position.second - 1] == 7)) {
                if (pacman->getDirection() == Direction::UP ||
                    pacman->getDirection() == Direction::DOWN) {
                    pacman->setDelta(0);
                } else if (pacman->getDirection() == Direction::RIGHT) {
                    pacman->setDelta((kCell - pacman->getDelta()) % kCell);
                    pacman->setPosition(position.first, position.second + 1);
                }
                pacman->setDirection(Direction::LEFT);
            } else {
                pacman->setNextDirection(Direction::LEFT);
            }
        } else if (event.key.code == sf::Keyboard::Up &&
                   pacman->getDirection() != Direction::UP) {
            if (position.first > 0 &&
                (board[position.first - 1][position.second] == 3 ||
                 board[position.first - 1][position.second] == 1 ||
                 board[position.first - 1][position.second] == 7)) {
                if (pacman->getDirection() == Direction::LEFT || pacman->getDirection() == Direction::RIGHT) {
                    pacman->setDelta(0);
                } else if (pacman->getDirection() == Direction::DOWN) {
                    pacman->setDelta((kCell - pacman->getDelta()) % kCell);
                    pacman->setPosition(position.first + 1, position.second);
                }
                pacman->setDirection(Direction::UP);
            } else {
                pacman->setNextDirection(Direction::UP);
            }
        } else if (event.key.code == sf::Keyboard::Down &&
                   pacman->getDirection() != Direction::DOWN) {
            if (position.first + 1 < board.size() &&
                (board[position.first + 1][position.second] == 3 ||
                 board[position.first + 1][position.second] == 1 ||
                 board[position.first + 1][position.second] == 7)) {
                if (pacman->getDirection() == Direction::LEFT || pacman->getDirection() == Direction::RIGHT) {
                    pacman->setDelta(0);
                } else if (pacman->getDirection() == Direction::UP) {
                    pacman->setDelta((kCell - pacman->getDelta()) % kCell);
                    pacman->setPosition(position.first - 1, position.second);
                }
                pacman->setDirection(Direction::DOWN);
            } else {
                pacman->setNextDirection(Direction::DOWN);
            }
        }
    }
}

void DrawCharacters(sf::RenderWindow &window) { pacman->draw(window); }

void UpdateCharactersPosition() {
    pacman->move();
    // switch (pacman->getDirection()) {
    //     case Direction::DOWN:
    //         std::cout << 3;
    //         break;
    //     case Direction::UP:
    //         std::cout << 1;
    //         break;
    //     case Direction::LEFT:
    //         std::cout << 4;
    //         break;
    //     case Direction::RIGHT:
    //         std::cout << 2;
    //         break;
    //     case Direction::NULL_DIR:
    //         std::cout << 0;
    //         break;
    // }
    // std::cout << " ";
    // std::cout.flush();
}

void DidPacmanEat() {
    int delta = pacman->getDelta();
    if (delta == 0) {
        auto position = pacman->getPosition();
        if (dots[position]) {
            dots.erase(position);
            board[position.first][position.second] = 3;
        }
    }
}
