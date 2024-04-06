#pragma once
#include <SFML/Graphics.hpp>
#include <string>
enum class Direction {
    LEFT,
    RIGHT,
    DOWN,
    UP,
    NULL_DIR
};

class Character {
   public:
    Character(int i, int j, std::string name)
        : i_(i), j_(j), texture_name_(name) {}
    void setTexture(std::string name) { texture_name_ = name; }
    void setPosition(int i, int j) {
        i_ = i;
        j_ = j;
    }
    void setDirection(Direction direction) { direction_ = direction; }
    void setDelta(int delta) { delta_ = delta; }
    void setNextDirection(Direction next) { next_direction_ = next; }
    void move();
    void draw(sf::RenderWindow& window);

    std::pair<int, int> getPosition() { return {i_, j_}; }
    int getDelta() { return delta_; }
    Direction getDirection() { return direction_; }

   protected:
    int i_, j_;
    std::string texture_name_;
    Direction direction_ = Direction::NULL_DIR, next_direction_ = Direction::NULL_DIR;
    int delta_ = 0;
};