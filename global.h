#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <set>
#include <vector>

#include "characters.h"
#include "gui.h"

extern std::set<std::pair<int, int>> dots;
extern std::vector<std::vector<int>> board;
extern const int kCell;
extern sf::Color dots_color;
extern const double golden_ratio;
extern Pacman* pacman;
extern Bot* bot;
extern Ghost *pink, blue;

extern sf::Time dispersion_elapsed_time;
extern sf::Clock dispersion_clock;
extern bool dispersion;

bool IsAvailableCell(int x, int y);
bool IsPortal(int x, int y);

extern Button random_button;
extern Button smart_button;
extern Button play_button;
extern Button new_game_button;