#include "characters.h"

#include <SFML/Graphics.hpp>
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
        texture_name_ = texture_default_;
    }
    if (delta_ % kCell == 0) {
        if (direction_ == Direction::UP) --i_;
        if (direction_ == Direction::RIGHT) ++j_;
        if (direction_ == Direction::DOWN) ++i_;
        if (direction_ == Direction::LEFT) --j_;
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
        if (delta_ == 0) {
            int cur = 0;
            if (direction_ == Direction::LEFT)
                cur = 3;
            else if (direction_ == Direction::RIGHT)
                cur = 1;
            else if (direction_ == Direction::DOWN)
                cur = 2;

            std::set<int> s;
            if (IsAvailableCell(i_ - 1, j_) && cur % 2 != 0) {
                s.insert(0);
            }
            if (IsAvailableCell(i_ + 1, j_) && cur % 2 != 0) {
                s.insert(2);
            }
            if (IsAvailableCell(i_, j_ + 1) && cur % 2 == 0) {
                s.insert(1);
            }
            if (IsAvailableCell(i_, j_ - 1) && cur % 2 == 0) {
                s.insert(3);
            }
            if (s.size() != 0) {
                int num = mt() % s.size();
                for (auto i : s){
                    if (num == 0) {
                        num = i;
                        break;
                    }
                    --num;
                }
                Direction dir;
                if (num == 0)
                    dir = Direction::UP;
                else if (num == 1)
                    dir = Direction::RIGHT;
                else if (num == 2)
                    dir = Direction::DOWN;
                else
                    dir = Direction::LEFT;
                UpdateDirection(dir, bot);
            }
        }
    }

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
        texture_name_ = texture_default_;
    }
    if (delta_ % kCell == 0) {
        if (direction_ == Direction::UP) --i_;
        if (direction_ == Direction::RIGHT) ++j_;
        if (direction_ == Direction::DOWN) ++i_;
        if (direction_ == Direction::LEFT) --j_;
        delta_ = 0;
    }

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