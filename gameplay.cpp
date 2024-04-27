#include "gameplay.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "global.h"
#include "gui.h"

void CreateCharacters() {
    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board[i].size(); ++j) {
            if (board[i][j] == 5) {
                pacman = new Pacman(i, j);
                pacman->set_textures("pacman_default", "pacman_up_default", "pacman_down_default",
                                     "pacman_right_default", "pacman_left_default");
                pacman->set_texture_open("pacman_up_open", "pacman_down_open", "pacman_left_open",
                                         "pacman_right_open");
                board[i][j] = 3;
            }
            if (board[i][j] == 9) {
                bot = new Bot(i, j);
                bot->set_textures("bot_left", "bot_up", "bot_down", "bot_right", "bot_left");
                bot->setDirection(Direction::LEFT);
                board[i][j] = 3;
            }
        }
    }
}

// add or 7
void EventProcessing(sf::Event &event) {
    if (event.type == sf::Event::KeyPressed) {
        Direction direction = Direction::NULL_DIR;
        if (event.key.code == sf::Keyboard::Right) {
            direction = Direction::RIGHT;
        } else if (event.key.code == sf::Keyboard::Up) {
            direction = Direction::UP;
        } else if (event.key.code == sf::Keyboard::Left) {
            direction = Direction::LEFT;
        } else if (event.key.code == sf::Keyboard::Down) {
            direction = Direction::DOWN;
        }
        if (direction != Direction::NULL_DIR) {
            UpdateDirection(direction, pacman);
        }
    }
}

void DrawCharacters(sf::RenderWindow &window) {
    pacman->draw(window);
    bot->draw(window);
}

void UpdateCharactersPosition() {
    pacman->move();
    bot->move();
}

void DidPacmanEat() {
    int delta = pacman->getDelta();
    if (delta == 0) {
        auto position = pacman->getPosition();
        if (dots[position]) {
            dots.erase(position);
            board[position.first][position.second] = 3;
            pacman->eat_dot();
        }
    }
}

void DidBotEat() {
    int delta = bot->getDelta();
    if (delta == 0) {
        auto position = bot->getPosition();
        if (dots[position]) {
            dots.erase(position);
            board[position.first][position.second] = 3;
            bot->eat_dot();
        }
    }
}

void UpdateDirection(Direction direction, Character *ch) {
    auto position = ch->getPosition();
    if (direction == Direction::RIGHT && ch->getDirection() != Direction::RIGHT) {
        if (position.second + 1 < board[0].size() &&
            IsAvailableCell(position.first, position.second + 1)) {
            if (ch->getDirection() == Direction::UP || ch->getDirection() == Direction::DOWN) {
                ch->setDelta(0);
            } else if (ch->getDirection() == Direction::LEFT) {
                ch->setDelta((kCell - ch->getDelta()) % kCell);
                ch->setPosition(position.first, position.second - 1);
            }
            ch->setDirection(Direction::RIGHT);
        } else {
            ch->setNextDirection(Direction::RIGHT);
        }
    } else if (direction == Direction::LEFT && ch->getDirection() != Direction::LEFT) {
        if (position.second > 0 && (board[position.first][position.second - 1] == 3 ||
                                    board[position.first][position.second - 1] == 1 ||
                                    board[position.first][position.second - 1] == 7)) {
            if (ch->getDirection() == Direction::UP || ch->getDirection() == Direction::DOWN) {
                ch->setDelta(0);
            } else if (ch->getDirection() == Direction::RIGHT) {
                ch->setDelta((kCell - ch->getDelta()) % kCell);
                ch->setPosition(position.first, position.second + 1);
            }
            ch->setDirection(Direction::LEFT);
        } else {
            ch->setNextDirection(Direction::LEFT);
        }
    } else if (direction == Direction::UP && ch->getDirection() != Direction::UP) {
        if (position.first > 0 && (board[position.first - 1][position.second] == 3 ||
                                   board[position.first - 1][position.second] == 1 ||
                                   board[position.first - 1][position.second] == 7)) {
            if (ch->getDirection() == Direction::LEFT || ch->getDirection() == Direction::RIGHT) {
                ch->setDelta(0);
            } else if (ch->getDirection() == Direction::DOWN) {
                ch->setDelta((kCell - ch->getDelta()) % kCell);
                ch->setPosition(position.first + 1, position.second);
            }
            ch->setDirection(Direction::UP);
        } else {
            ch->setNextDirection(Direction::UP);
        }
    } else if (direction == Direction::DOWN && ch->getDirection() != Direction::DOWN) {
        if (position.first + 1 < board.size() &&
            (board[position.first + 1][position.second] == 3 ||
             board[position.first + 1][position.second] == 1 ||
             board[position.first + 1][position.second] == 7)) {
            if (ch->getDirection() == Direction::LEFT || ch->getDirection() == Direction::RIGHT) {
                ch->setDelta(0);
            } else if (ch->getDirection() == Direction::UP) {
                ch->setDelta((kCell - ch->getDelta()) % kCell);
                ch->setPosition(position.first - 1, position.second);
            }
            ch->setDirection(Direction::DOWN);
        } else {
            ch->setNextDirection(Direction::DOWN);
        }
    }
}

void CheckEnd(){
    if (dots.size() == 0){
        if (pacman->get_score() > bot->get_score()){
            
        }
    }
}