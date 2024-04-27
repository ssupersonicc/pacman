#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

#include "characters.h"

extern std::map<std::pair<int, int>, bool> dots;
extern std::vector<std::vector<int>> board;
extern const int kCell;
extern sf::Color dots_color;
extern const double golden_ratio;
extern Pacman* pacman;
extern Bot* bot;


bool IsAvailableCell(int x, int y);