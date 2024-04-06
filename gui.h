#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "global.h"

class Object {
   public:
    Object() = default;
    Object(double x, double y) : x_(x), y_(y) {}
    void setPosition(double x, double y) {
        x_ = x;
        y_ = y;
    }
    void setColor(sf::Color color) { color_ = color; }
    virtual void draw(sf::RenderWindow &window) = 0;

   protected:
    double x_, y_;
    sf::Color color_;
};

class Pixel : public Object {
   public:
    void draw(sf::RenderWindow &window) {
        sf::RectangleShape shape;
        shape.setPosition(x_, y_);
        shape.setFillColor(color_);
        shape.setSize(sf::Vector2f(1.0, 1.0));
        window.draw(shape);
    }
};

class Line : public Object {
   public:
    Line() = default;
    Line(double x, double y) {
        x_ = x;
        y_ = y;
    }
    void setRotation(double angle) { angle_ = angle; }
    void setSize(double size_x, double size_y) {
        size_x_ = size_x;
        size_y_ = size_y;
    }
    void draw(sf::RenderWindow &window) {
        sf::RectangleShape shape;
        shape.setPosition(x_, y_);
        shape.setFillColor(color_);
        shape.setSize(sf::Vector2f(size_x_, size_y_));
        shape.setRotation(angle_);
        window.draw(shape);
    }

   private:
    double angle_ = 0;
    double size_x_, size_y_;
};

class Circle : public Object {
   public:
    Circle(double x, double y) {
        x_ = x;
        y_ = y;
    }
    void setRadius(double rad) { rad_ = rad; }
    void draw(sf::RenderWindow &window) {
        sf::CircleShape shape;
        shape.setPosition(x_, y_);
        shape.setFillColor(color_);
        shape.setRadius(rad_);
        window.draw(shape);
    }

   private:
    double rad_;
};

class Text : public Object {
   public:
    Text(double x, double y, std::string text, double size) {
        x_ = x;
        y_ = y;
        text_ = text;
        size_ = size;
    }
    void setText(std::string text) { text_ = text; }
    void setFont(std::string font_name) { font_name_ = font_name; }
    void setSize(double size) { size_ = size; }
    void draw(sf::RenderWindow &window) {
        if (text_.length() != 0) {
            sf::Font font;
            if (font.loadFromFile(font_name_)) {
                sf::Text text;
                text.setFont(font);
                text.setString(text_);
                text.setCharacterSize(size_);
                text.setFillColor(color_);
                text.setPosition(x_, y_);
                window.draw(text);
            } else {
                std::cout << "ERROR!\n";
            }
        }
    }

   private:
    double x_, y_, size_;
    std::string text_, font_name_;
};

Line CreateLine(double x, double y, double length, double width = 2.0,
                double angle = 0.0, sf::Color color = sf::Color::Blue);

Circle CreateCircle(double x, double y, double rad,
                    sf::Color color = dots_color);

Text CreateText(double x, double y, std::string content, double size,
                std::string font_name, sf::Color color = sf::Color::White);
void DrawObject(Object &&, sf::RenderWindow &);
