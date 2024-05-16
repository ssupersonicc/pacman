#pragma once
#include <SFML/Graphics.hpp>
#include <string>
enum class Direction { LEFT, RIGHT, DOWN, UP, NULL_DIR };

class Character {
   public:
    bool exist = true;
    void setPosition(int i, int j) {
        i_ = i;
        j_ = j;
    }
    void setDirection(Direction direction) { direction_ = direction; }
    void setDelta(int delta) { delta_ = delta; }
    void setNextDirection(Direction next) { next_direction_ = next; }
    virtual void move() = 0;
    void draw(sf::RenderWindow& window);

    std::pair<int, int> getPosition() { return {i_, j_}; }
    int getDelta() { return delta_; }
    Direction getDirection() { return direction_; }
    void set_textures(std::string def, std::string up, std::string down, std::string right,
                      std::string left) {
        texture_default_ = "img/" + def + ".png";
        texture_up_ = "img/" + up + ".png";
        texture_down_ = "img/" + down + ".png";
        texture_left_ = "img/" + left + ".png";
        texture_right_ = "img/" + right + ".png";
        texture_name_ = texture_default_;
    }

   protected:
    int i_, j_;
    std::string texture_name_;
    Direction direction_ = Direction::NULL_DIR, next_direction_ = Direction::NULL_DIR;
    int delta_ = 0;
    std::string texture_default_, texture_down_, texture_up_, texture_right_, texture_left_;
};

class Pacman : public Character {
   public:
    Pacman(int i, int j) {
        i_ = i;
        j_ = j;
    }
    void move();
    void set_speed(double s) { speed_ = s; }
    double get_speed() { return speed_; }
    void eat_dot(int i) { score_ += 10 * i; }
    int get_score() { return score_; }
    void set_texture_open(std::string up, std::string down, std::string left, std::string right) {
        texture_up_open_ = "img/" + up + ".png";
        texture_down_open_ = "img/" + down + ".png";
        texture_left_open_ = "img/" + left + ".png";
        texture_right_open_ = "img/" + right + ".png";
    }

   private:
    std::string texture_up_open_, texture_down_open_, texture_left_open_, texture_right_open_;
    sf::Time elapsed_time_;
    sf::Clock clock_;
    int score_ = 0;
    double speed_ = 1.0;
};

class Bot : public Character {
   public:
    Bot(int i, int j) {
        i_ = i;
        j_ = j;
    }
    void move();
    void eat_dot(int i) { score_ += 10 * i; }
    int get_score() { return score_; }
    void setType(int type) { type_ = type; }
    void set_next(int x, int y) {
        next_x_ = x;
        next_y_ = y;
    }

   private:
    int type_ = -1;
    int score_ = 0;
    int next_x_, next_y_;
    int next_ = 0;
};

class Ghost : public Character {
   public:
    Ghost() = default;
    Ghost(int i, int j) {
        i_ = i;
        j_ = j;
    }
    void move();
    void set_next(int x, int y) {
        next_x_ = x;
        next_y_ = y;
    }

   private:
    int type_ = 1;
    int next_x_ = -1, next_y_ = -1;
    int cnt_ = 0;
};