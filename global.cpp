#include "global.h"

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

#include "characters.h"

std::map<std::pair<int, int>, bool> dots;
std::vector<std::vector<int>> board;
const int kCell = 30;
sf::Color dots_color = sf::Color(247, 220, 111);
const double golden_ratio = 1.6180339887;
Pacman* pacman;
Bot* bot;


bool IsAvailableCell(int x, int y) {
    if (board[x][y] == 1 || board[x][y] == 3 || board[x][y] == 7) return true;
    return false;
}