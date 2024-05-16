#include "characters.h"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <set>

#include "gameplay.h"
#include "global.h"

std::mt19937 mt(std::chrono::high_resolution_clock::now().time_since_epoch().count());

void Character::draw(sf::RenderWindow& window) {
    sf::Texture texture;
    if (texture.loadFromFile(texture_name_)) {
        sf::Sprite sprite;
        sprite.setTexture(texture);
        auto size = texture.getSize();
        sprite.setScale(sf::Vector2f((double)kCell / size.x, (double)kCell / size.y));
        int dx = 0, dy = 0;
        if (direction_ == Direction::UP) dy = -delta_;
        if (direction_ == Direction::DOWN) dy = delta_;
        if (direction_ == Direction::RIGHT) dx = delta_;
        if (direction_ == Direction::LEFT) dx = -delta_;
        sprite.setPosition(sf::Vector2f(j_ * kCell + dx, i_ * kCell + dy));
        window.draw(sprite);
    }
}

void Pacman::move() {
    if (next_direction_ != Direction::NULL_DIR && delta_ == 0) {
        if (next_direction_ == Direction::UP && IsAvailableCell(i_ - 1, j_)) {
            direction_ = next_direction_;
            next_direction_ = Direction::NULL_DIR;
            return;
        }
        if (next_direction_ == Direction::LEFT && IsAvailableCell(i_, j_ - 1)) {
            direction_ = next_direction_;
            next_direction_ = Direction::NULL_DIR;
            return;
        }
        if (next_direction_ == Direction::RIGHT && IsAvailableCell(i_, j_ + 1)) {
            direction_ = next_direction_;
            next_direction_ = Direction::NULL_DIR;
            return;
        }
        if (next_direction_ == Direction::DOWN && IsAvailableCell(i_ + 1, j_)) {
            direction_ = next_direction_;
            next_direction_ = Direction::NULL_DIR;
            return;
        }
    }

    if (direction_ == Direction::UP && i_ > 0 && IsAvailableCell(i_ - 1, j_)) {
        ++delta_;
    } else if (direction_ == Direction::RIGHT &&
               (j_ + 1 < board[0].size() && IsAvailableCell(i_, j_ + 1) || IsPortal(i_, j_))) {
        ++delta_;
    } else if (direction_ == Direction::DOWN &&
               i_ + 1 < board.size() & IsAvailableCell(i_ + 1, j_)) {
        ++delta_;
    } else if (direction_ == Direction::LEFT &&
               (j_ > 0 && IsAvailableCell(i_, j_ - 1) || IsPortal(i_, j_))) {
        ++delta_;
    } else {
        direction_ = Direction::NULL_DIR;
        texture_name_ = texture_default_;
    }
    if (delta_ % kCell == 0) {
        if (direction_ == Direction::UP) --i_;
        if (direction_ == Direction::RIGHT) ++j_;
        if (direction_ == Direction::DOWN) ++i_;
        if (direction_ == Direction::LEFT) --j_;
        if (j_ < 0) j_ = board[0].size() - 1;
        if (j_ > board[0].size() - 1) j_ = 0;
        delta_ = 0;
    }

    sf::Time delta_time = clock_.restart();
    elapsed_time_ += delta_time;
    if (elapsed_time_ > sf::milliseconds(100)) {
        elapsed_time_ = sf::milliseconds(0);
        if (direction_ == Direction::UP) {
            if (texture_name_ == texture_up_)
                texture_name_ = texture_up_open_;
            else
                texture_name_ = texture_up_;
        } else if (direction_ == Direction::RIGHT) {
            if (texture_name_ == texture_right_)
                texture_name_ = texture_right_open_;
            else
                texture_name_ = texture_right_;
        } else if (direction_ == Direction::DOWN) {
            if (texture_name_ == texture_down_)
                texture_name_ = texture_down_open_;
            else
                texture_name_ = texture_down_;
        } else if (direction_ == Direction::LEFT) {
            if (texture_name_ == texture_left_)
                texture_name_ = texture_left_open_;
            else
                texture_name_ = texture_left_;
        }
    }
}

void Bot::move() {
    if (type_ == 1) {
        if (next_x_ == i_ && next_y_ == j_ && next_direction_ == Direction::NULL_DIR) {
            if (next_x_ == i_ && next_y_ == j_) {
                next_x_ = mt() % board.size();
                next_y_ = mt() % board[0].size();
                while (!IsAvailableCell(next_x_, next_y_)) {
                    next_x_ = mt() % board.size();
                    next_y_ = mt() % board[0].size();
                }
            }
        }
        std::vector<std::vector<int>> d = dist({next_x_, next_y_});
        std::vector<std::pair<int, int>> dir;
        if (i_ - 1 >= 0 && IsAvailableCell(i_ - 1, j_)) dir.push_back({d[i_ - 1][j_], 0});
        if (j_ + 1 < board[0].size() && IsAvailableCell(i_, j_ + 1))
            dir.push_back({d[i_][j_ + 1], 1});
        if (i_ + 1 < board.size() && IsAvailableCell(i_ + 1, j_)) dir.push_back({d[i_ + 1][j_], 2});
        if (j_ - 1 >= 0 && IsAvailableCell(i_, j_ - 1)) dir.push_back({d[i_][j_ - 1], 3});

        std::sort(dir.begin(), dir.end());
        if (dir[0].second == 0)
            direction_ = Direction::UP;
        else if (dir[0].second == 1)
            direction_ = Direction::RIGHT;
        else if (dir[0].second == 2)
            direction_ = Direction::DOWN;
        else if (dir[0].second == 3)
            direction_ = Direction::LEFT;
    } else if (type_ == 2) {
        auto ghost_position = pink->getPosition();
        auto ghost_direction = pink->getDirection();
        if (i_ == ghost_position.first) {
            if (ghost_direction == Direction::LEFT && IsAvailableCell(i_, j_ - 1))
                next_direction_ = Direction::LEFT;
            else if (ghost_direction == Direction::RIGHT && IsAvailableCell(i_, j_ + 1))
                next_direction_ = Direction::RIGHT;
            else
                next_direction_ = Direction::NULL_DIR;
            next_ = 1;
        } else if (j_ == ghost_position.second) {
            if (ghost_direction == Direction::UP && IsAvailableCell(i_ - 1, j_))
                next_direction_ = Direction::UP;
            else if (ghost_direction == Direction::DOWN && IsAvailableCell(i_ + 1, j_))
                next_direction_ = Direction::DOWN;
            else
                next_direction_ = Direction::NULL_DIR;
            next_ = 2;
        } else {
            next_direction_ = Direction::NULL_DIR;
            next_ = 0;
        }
        if (next_x_ == i_ && next_y_ == j_) {
            std::vector<std::vector<int>> d = dist({i_, j_});
            int mn = INT32_MAX;
            for (int i = 0; i < board.size(); ++i) {
                for (int j = 0; j < board[0].size(); ++j) {
                    if (dots.count({i, j})) {
                        if (mn > d[i][j]) {
                            mn = d[i][j];
                            next_x_ = i;
                            next_y_ = j;
                        }
                    }
                }
            }
        }
        std::vector<std::vector<int>> d = dist({next_x_, next_y_});
        std::vector<std::pair<int, int>> dir;
        if (i_ - 1 >= 0 && IsAvailableCell(i_ - 1, j_)) dir.push_back({d[i_ - 1][j_], 0});
        if (j_ + 1 < board[0].size() && IsAvailableCell(i_, j_ + 1))
            dir.push_back({d[i_][j_ + 1], 1});
        if (i_ + 1 < board.size() && IsAvailableCell(i_ + 1, j_)) dir.push_back({d[i_ + 1][j_], 2});
        if (j_ - 1 >= 0 && IsAvailableCell(i_, j_ - 1)) dir.push_back({d[i_][j_ - 1], 3});

        std::sort(dir.begin(), dir.end());
        Direction tmp_direction = Direction::NULL_DIR;
        if (dir.size() > 0) {
            if (dir[0].second == 0)
                tmp_direction = Direction::UP;
            else if (dir[0].second == 1)
                tmp_direction = Direction::RIGHT;
            else if (dir[0].second == 2)
                tmp_direction = Direction::DOWN;
            else if (dir[0].second == 3)
                tmp_direction = Direction::LEFT;
        }
        if (next_direction_ == Direction::NULL_DIR && tmp_direction != Direction::NULL_DIR)
            UpdateDirection(tmp_direction, bot);
    }
    if (next_direction_ != Direction::NULL_DIR) {
        UpdateDirection(next_direction_, bot);
        next_direction_ = Direction::NULL_DIR;
    }
    // if (delta_ == 0 && next_ != 0) {
    //     if (next_ == 1) {
    //         if (i_ > 0 && IsAvailableCell(i_ - 1, j_)) {
    //             direction_ = Direction::UP;
    //             next_ = 0;
    //         } else if (i_ + 1 < board.size() && IsAvailableCell(i_ + 1, j_)) {
    //             direction_ = Direction::DOWN;
    //             next_ = 0;
    //         }
    //     } else if (next_ == 2) {
    //         if (j_ > 0 && IsAvailableCell(i_, j_ - 1)) {
    //             direction_ = Direction::LEFT;
    //             next_ = 0;
    //         } else if (j_ + 1 < board[0].size() && IsAvailableCell(i_, j_ + 1)) {
    //             direction_ = Direction::RIGHT;
    //             next_ = 0;
    //         }
    //     }
    // }

    if (direction_ == Direction::UP) {
        texture_name_ = texture_up_;
    } else if (direction_ == Direction::RIGHT) {
        texture_name_ = texture_right_;
    } else if (direction_ == Direction::DOWN) {
        texture_name_ = texture_down_;
    } else if (direction_ == Direction::LEFT) {
        texture_name_ = texture_left_;
    }

    if (direction_ == Direction::UP && i_ > 0 && IsAvailableCell(i_ - 1, j_)) {
        ++delta_;
    } else if (direction_ == Direction::RIGHT && j_ + 1 < board[0].size() &&
               IsAvailableCell(i_, j_ + 1)) {
        ++delta_;
    } else if (direction_ == Direction::DOWN &&
               i_ + 1 < board.size() & IsAvailableCell(i_ + 1, j_)) {
        ++delta_;
    } else if (direction_ == Direction::LEFT && j_ > 0 && IsAvailableCell(i_, j_ - 1)) {
        ++delta_;
    } else {
        direction_ = Direction::NULL_DIR;
    }
    if (delta_ % kCell == 0) {
        if (direction_ == Direction::UP) --i_;
        if (direction_ == Direction::RIGHT) ++j_;
        if (direction_ == Direction::DOWN) ++i_;
        if (direction_ == Direction::LEFT) --j_;
        delta_ = 0;
    }
}

void Ghost::move() {
    if (type_ == 1 && delta_ == 0) {
        if (pacman->exist && bot->exist) {
            if (next_x_ == i_ && next_y_ == j_) {
                auto position_pacman = pacman->getPosition();
                auto position_bot = bot->getPosition();
                std::vector<std::vector<int>> pac_dist = dist(position_pacman);
                std::vector<std::vector<int>> bot_dist = dist(position_bot);
                if (cnt_ < 3) {
                    for (int i = 0; i < board.size(); ++i) {
                        for (int j = 0; j < board[0].size(); ++j) {
                            if (IsAvailableCell(i, j)) {
                                int dt1 = pac_dist[i][j] + bot_dist[i][j];
                                int dt2 = pac_dist[next_x_][next_y_] + bot_dist[next_x_][next_y_];
                                if ((dt2 > dt1) || pac_dist[next_x_][next_y_] == INT32_MAX) {
                                    next_x_ = i;
                                    next_y_ = j;
                                }
                            }
                        }
                    }
                    if (pac_dist[next_x_][next_y_] < 10) {
                        next_x_ = position_pacman.first;
                        next_y_ = position_pacman.second;
                    } else if (bot_dist[next_x_][next_y_] < 10) {
                        next_x_ = position_bot.first;
                        next_y_ = position_bot.second;
                    }
                    cnt_++;
                    std::cout << cnt_ << '\n';
                } else {
                    cnt_ = 0;
                    if (pac_dist[next_x_][next_y_] < bot_dist[next_x_][next_y_]) {
                        next_x_ = position_bot.first;
                        next_y_ = position_bot.second;
                    } else {
                        next_x_ = position_pacman.first;
                        next_y_ = position_pacman.second;
                    }
                }
            }
        } else {
            if (next_x_ == i_ && next_y_ == j_) {
                next_x_ = mt() % board.size();
                next_y_ = mt() % board[0].size();
                while (!IsAvailableCell(next_x_, next_y_)) {
                    next_x_ = mt() % board.size();
                    next_y_ = mt() % board[0].size();
                }
            }
        }
        std::vector<std::vector<int>> d = dist({next_x_, next_y_});
        std::vector<std::pair<int, int>> dir;
        if (i_ - 1 >= 0 && IsAvailableCell(i_ - 1, j_)) dir.push_back({d[i_ - 1][j_], 0});
        if (j_ + 1 < board[0].size() && IsAvailableCell(i_, j_ + 1))
            dir.push_back({d[i_][j_ + 1], 1});
        if (i_ + 1 < board.size() && IsAvailableCell(i_ + 1, j_)) dir.push_back({d[i_ + 1][j_], 2});
        if (j_ - 1 >= 0 && IsAvailableCell(i_, j_ - 1)) dir.push_back({d[i_][j_ - 1], 3});

        std::sort(dir.begin(), dir.end());
        if (dir[0].second == 0)
            direction_ = Direction::UP;
        else if (dir[0].second == 1)
            direction_ = Direction::RIGHT;
        else if (dir[0].second == 2)
            direction_ = Direction::DOWN;
        else if (dir[0].second == 3)
            direction_ = Direction::LEFT;
        if (direction_ == Direction::UP) {
            texture_name_ = texture_up_;
        } else if (direction_ == Direction::RIGHT) {
            texture_name_ = texture_right_;
        } else if (direction_ == Direction::DOWN) {
            texture_name_ = texture_down_;
        } else if (direction_ == Direction::LEFT) {
            texture_name_ = texture_left_;
        }
    }

    if (direction_ == Direction::UP && i_ > 0 && IsAvailableCell(i_ - 1, j_)) {
        ++delta_;
    } else if (direction_ == Direction::RIGHT && j_ + 1 < board[0].size() &&
               IsAvailableCell(i_, j_ + 1)) {
        ++delta_;
    } else if (direction_ == Direction::DOWN &&
               i_ + 1 < board.size() & IsAvailableCell(i_ + 1, j_)) {
        ++delta_;
    } else if (direction_ == Direction::LEFT && j_ > 0 && IsAvailableCell(i_, j_ - 1)) {
        ++delta_;
    }
    if (delta_ % kCell == 0) {
        if (direction_ == Direction::UP) --i_;
        if (direction_ == Direction::RIGHT) ++j_;
        if (direction_ == Direction::DOWN) ++i_;
        if (direction_ == Direction::LEFT) --j_;
        delta_ = 0;
    }
}