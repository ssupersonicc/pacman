#include "gameplay.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>

#include "global.h"
#include "gui.h"

const int inf = INT32_MAX;

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
                bot->set_next(i, j);
                board[i][j] = 3;
            }
            if (board[i][j] == 0 && i > 0 && board[i - 1][j] == 4) {
                pink = new Ghost(i, j);
                pink->set_textures("pink_left", "pink_up", "pink_down", "pink_right", "pink_left");
                pink->set_next(i, j);
            }
        }
    }
}

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
    if (pacman->exist) pacman->draw(window);
    if (bot->exist) bot->draw(window);
    pink->draw(window);
}

void UpdateCharactersPosition() {
    if (pacman->exist) pacman->move();
    if (bot->exist) bot->move();
}

void DidPacmanEat() {
    int delta = pacman->getDelta();
    if (delta == 0) {
        auto position = pacman->getPosition();
        if (dots.count(position)) {
            if (board[position.first][position.second] == 7) {
                pacman->set_speed(1.5);
            }
            pacman->eat_dot(1);
            dots.erase(position);
            board[position.first][position.second] = 3;
        }
    }
}

void DidBotEat() {
    int delta = bot->getDelta();
    if (delta == 0) {
        auto position = bot->getPosition();
        if (dots.count(position)) {
            bot->eat_dot(1);
            dots.erase(position);
            board[position.first][position.second] = 3;
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
        if (position.second > 0 && IsAvailableCell(position.first, position.second - 1)) {
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

void PrintWin(sf::RenderWindow &window) {
    double width = board[0].size() * 30;
    double height = board.size() * 30;
    std::string result = "YOU WON!";
    int len = result.length();
    double r = 50.0;
    int x = (width - len * (r / golden_ratio)) / 2.0 - 100;
    int y = (height - r) / 2.0;
    DrawObject(CreateText(x, y, result, r, "emulogic.ttf", sf::Color::Yellow), window);
}

void PrintLose(sf::RenderWindow &window) {
    double width = board[0].size() * 30;
    double height = board.size() * 30;
    std::string result = "YOU LOST!";
    int len = result.length();
    double r = 50.0;
    int x = (width - len * (r / golden_ratio)) / 2.0 - 100;
    int y = (height - r) / 2.0;
    DrawObject(CreateText(x, y, result, r, "emulogic.ttf", sf::Color::Yellow), window);
}

bool CheckEnd(sf::RenderWindow &window) {
    // std::cout << dots.size() << '\n';
    if (pacman->get_score() > bot->get_score() && dots.size() == 0) {
        PrintWin(window);
        return true;
    } else if (dots.size() == 0 && pacman->get_score() <= bot->get_score()) {
        PrintLose(window);
        return true;
    } else if (!pacman->exist && !bot->exist) {
        if (pacman->get_score() > bot->get_score()) {
            PrintWin(window);
        } else {
            PrintLose(window);
        }
        return true;
    } else if (!pacman->exist) {
        PrintLose(window);
    }
    return false;
}

std::vector<std::vector<int>> dist(std::pair<int, int> f) {
    int a = f.first;
    int b = f.second;
    std::vector<std::vector<int>> d(board.size(), std::vector<int>(board[0].size(), inf));
    d[a][b] = 0;
    std::queue<std::pair<int, int>> q;
    q.push({a, b});
    while (!q.empty()) {
        auto t = q.front();
        q.pop();
        std::vector<std::pair<int, int>> temp;
        if (t.first + 1 < board.size() && IsAvailableCell(t.first + 1, t.second))
            temp.push_back({t.first + 1, t.second});
        if (t.first - 1 >= 0 && IsAvailableCell(t.first - 1, t.second))
            temp.push_back({t.first - 1, t.second});
        if (t.second + 1 < board[0].size() && IsAvailableCell(t.first, t.second + 1))
            temp.push_back({t.first, t.second + 1});
        if (t.second - 1 >= 0 && IsAvailableCell(t.first, t.second - 1))
            temp.push_back({t.first, t.second - 1});
        for (int i = 0; i < temp.size(); ++i) {
            auto s = temp[i];
            if (d[s.first][s.second] > d[t.first][t.second] + 1) {
                d[s.first][s.second] = d[t.first][t.second] + 1;
                q.push(s);
            }
        }
    }
    return std::move(d);
}

bool CheckCrash(sf::RenderWindow &window) {
    auto pos_pink = pink->getPosition();
    auto pos_pacman = pacman->getPosition();
    auto pos_bot = bot->getPosition();
    if (pos_pacman == pos_pink) {
        pacman->exist = false;
        CheckEnd(window);
        return true;
    } else if (pos_bot == pos_pink) {
        bot->exist = false;
        CheckEnd(window);
        auto pos = bot->getPosition();
        dots.insert({pos.first, pos.second});
        board[pos.first][pos.second] = 7;
        bot->setPosition(-1, -1);
    }
    return false;
}

void EventButtons(sf::Event &event) {
    if (random_button.isClicked(event) && !play_button.isActive()){
        random_button.clicked();
        smart_button.notClicked();
        bot->setType(1);
    }
    if (smart_button.isClicked(event) && !play_button.isActive()){
        smart_button.clicked();
        random_button.notClicked();
        bot->setType(2);
    }
    if (play_button.isClicked(event)){
        if (smart_button.isActive() || random_button.isActive()){
            play_button.clicked();
        }
    }
    if (new_game_button.isClicked(event) && play_button.isActive()){
        play_button.notClicked();
        new_game_button.clicked();
    }
}