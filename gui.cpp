#include "gui.h"

#include <SFML/Graphics.hpp>

#include "global.h"

Line CreateLine(double x, double y, double length, double width, double angle,
                sf::Color color) {
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

Text CreateText(double x, double y, std::string content, double size,
                std::string font_name, sf::Color color) {
    Text text(x, y, content, size);
    text.setFont(font_name);
    text.setColor(color);
    return text;
}

void DrawObject(Object&& obj, sf::RenderWindow& window) { obj.draw(window); }