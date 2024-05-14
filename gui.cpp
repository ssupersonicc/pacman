#include "gui.h"

#include <SFML/Graphics.hpp>

#include "global.h"

Line CreateLine(double x, double y, double length, double width, double angle, sf::Color color) {
    Line line(x, y);
    line.setColor(color);
    line.setSize(length, width);
    line.setRotation(angle);
    return line;
}

Circle CreateCircle(double x, double y, double rad, sf::Color color) {
    Circle circle(x, y);
    circle.setColor(color);
    circle.setRadius(rad);
    return circle;
}

Text CreateText(double x, double y, std::string content, double size, std::string font_name,
                sf::Color color) {
    Text text(x, y, content, size);
    text.setFont(font_name);
    text.setColor(color);
    return text;
}

void DrawObject(Object&& obj, sf::RenderWindow& window) { obj.draw(window); }

void Button::draw(sf::RenderWindow& window) {
    if (!this->is_active_)
        color_ = color_clicked_;
    else
        color_ = color_not_clicked_;
    sf::RectangleShape field;
    field.move(x_, y_);
    field.setFillColor(color_);
    field.setSize(sf::Vector2f(width_, height_));
    field.setOutlineThickness(2);
    field.setOutlineColor(sf::Color::Black);
    window.draw(field);
    text_.draw(window);
}


bool Button::isClicked(sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            double mouse_x = event.mouseButton.x;
            double mouse_y = event.mouseButton.y;
            if (mouse_x >= x_ && mouse_x <= x_ + width_ && mouse_y >= y_ &&
                mouse_y <= y_ + height_) {
                return true;
            }
        }
    }
    return false;
}
