#include "characters.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "global.h"

void Character::draw(sf::RenderWindow& window) {
    sf::Texture texture;
    if (texture.loadFromFile(texture_name_)) {
        sf::Sprite sprite;
        sprite.setTexture(texture);
        auto size = texture.getSize();
        sprite.setScale(
            sf::Vector2f((double)kCell / size.x, (double)kCell / size.y));
        int dx = 0, dy = 0;
        if (direction_ == Direction::UP) dy = -delta_;
        if (direction_ == Direction::DOWN) dy = delta_;
        if (direction_ == Direction::RIGHT) dx = delta_;
        if (direction_ == Direction::LEFT) dx = -delta_;
        sprite.setPosition(sf::Vector2f(j_ * kCell + dx, i_ * kCell + dy));
        window.draw(sprite);
    }
}

void Character::move() {
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
        return;
    }
    if (delta_ % kCell == 0) {
        if (direction_ == Direction::UP) --i_;
        if (direction_ == Direction::RIGHT) ++j_;
        if (direction_ == Direction::DOWN) ++i_;
        if (direction_ == Direction::LEFT) --j_;
        delta_ = 0;
    }
}