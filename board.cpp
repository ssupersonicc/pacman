#include "board.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <map>

#include "global.h"
#include "gui.h"

void DrawBoard(sf::RenderWindow& window) {
    DrawObject(CreateText(kCell, 10, "PACMAN", kCell + 3, "CrackMan.TTF",
                          sf::Color::Yellow),
               window);
    DrawObject(CreateText(kCell * 6 + 12,
                          10 + kCell + 3 - 10, "by sonic", 10, "emulogic.ttf"),
               window);

    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board[i].size(); ++j) {
            if (board[i][j] == 0) continue;
            if (board[i][j] == 1) {
                DrawObject(CreateCircle(j * kCell + (kCell - 6) / 2,
                                        i * kCell + (kCell - 6) / 2, 3),
                           window);
                dots[{i, j}] = true;
            }
            if (board[i][j] == 7) {
                DrawObject(CreateCircle(j * kCell, i * kCell, 15), window);
            }
            if (board[i][j] == 2) {
                if (j > 0 && board[i][j - 1] == 2 && i + 1 < board.size() &&
                    board[i + 1][j] == 2 &&
                    (board[i + 1][j - 1] != 2 ||
                     (i > 0 && j < board[0].size() &&
                      board[i - 1][j + 1] != 2 && board[i - 1][j] != 2 &&
                      board[i][j + 1] != 2))) {
                    // right up
                    DrawObject(
                        CreateLine(j * kCell, i * kCell + kCell / 2, kCell / 2),
                        window);
                    DrawObject(
                        CreateLine(j * kCell + kCell / 2, i * kCell + kCell / 2,
                                   kCell / 2, 2, 90),
                        window);
                } else if (j + 1 < board[0].size() && board[i][j + 1] == 2 &&
                           i > 0 && board[i - 1][j] == 2 &&
                           (board[i - 1][j + 1] != 2 ||
                            (j > 0 && i + 1 < board.size() &&
                             board[i + 1][j] != 2 && board[i][j - 1] != 2))) {
                    // left down
                    DrawObject(CreateLine(j * kCell + kCell / 2,
                                          i * kCell + kCell / 2, 15),
                               window);
                    DrawObject(
                        CreateLine(j * kCell + kCell / 2, i * kCell, 15, 2, 90),
                        window);
                } else if (i > 0 && j > 0 && board[i - 1][j] == 2 &&
                           board[i][j - 1] == 2 &&
                           (board[i - 1][j - 1] != 2 ||
                            (i + 1 < board.size() && j + 1 < board[0].size() &&
                             board[i + 1][j] != 2 && board[i][j + 1] != 2))) {
                    // right down
                    DrawObject(CreateLine(j * kCell, i * kCell + kCell / 2, 15),
                               window);
                    DrawObject(
                        CreateLine(j * kCell + kCell / 2, i * kCell, 15, 2, 90),
                        window);
                } else if (i + 1 < board.size() && j + 1 < board[0].size() &&
                           board[i + 1][j] == 2 && board[i][j + 1] == 2 &&
                           (board[i + 1][j + 1] != 2 ||
                            (i > 0 && j > 0 && board[i - 1][j] != 2 &&
                             board[i][j - 1] != 2))) {
                    // left up
                    DrawObject(CreateLine(j * kCell + kCell / 2,
                                          i * kCell + kCell / 2, 15),
                               window);
                    DrawObject(CreateLine(j * kCell + kCell / 2,
                                          i * kCell + kCell / 2, 15, 2, 90),
                               window);
                } else if ((i > 0 && board[i - 1][j] == 2) &&
                           (i + 1 < board.size() && board[i + 1][j] == 2)) {
                    DrawObject(
                        CreateLine(j * kCell + kCell / 2, i * kCell, 30, 2, 90),
                        window);
                }
                // } else if ((j > 0 && board[i][j - 1] == 2) &&
                //            (j + 1 < board[0].size() && board[i][j + 1] == 2))
                //            {
                else {
                    DrawObject(CreateLine(j * kCell, i * kCell + kCell / 2, 30),
                               window);
                }
            }
            if (board[i][j] == 4) {
                DrawObject(CreateLine(j * kCell, i * kCell + kCell / 2, 30, 5,
                                      0, sf::Color::White),
                           window);
            }
        }
    }
}