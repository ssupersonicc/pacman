#include "global.h"

#include <SFML/Graphics.hpp>
#include <map>
#include <set>
#include <vector>

#include "characters.h"

std::set<std::pair<int, int>> dots;
std::vector<std::vector<int>> board;
const int kCell = 30;
sf::Color dots_color = sf::Color(247, 220, 111);
const double golden_ratio = 1.6180339887;
Pacman* pacman;
Bot* bot;
Ghost *pink;

sf::Time dispersion_elapsed_time;
sf::Clock dispersion_clock;
bool dispersion = false;

bool IsAvailableCell(int x, int y) {
    if (board[x][y] == 1 || board[x][y] == 3 || board[x][y] == 7 || board[x][y] == 8 ||
        board[x][y] == 4)
        return true;
    return false;
}

bool IsPortal(int x, int y) {
    if (board[x][y] == 8) return true;
    return false;
}

Button random_button(kCell * 19.5, 10 + kCell + 3 - kCell * 0.8, 110, 40);
Button smart_button(kCell * 19.5 + 20 + 110, 10 + kCell + 3 - kCell * 0.8, 110, 40);
Button play_button((840 - 330) / 2.0, 1080 - 60, 150, 50);
Button new_game_button((840 - 330) / 2.0 + 150 + 30, 1080 - 60, 150, 50);