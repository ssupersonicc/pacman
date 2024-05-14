#pragma once
#include <SFML/Graphics.hpp>

#include "characters.h"

void CreateCharacters();
void EventProcessing(sf::Event &event);
void UpdateCharactersPosition();
void DrawCharacters(sf::RenderWindow &window);

void DidPacmanEat();
void DidBotEat();
void UpdateDirection(Direction direction, Character *ch);
bool CheckEnd(sf::RenderWindow &window);
bool CheckCrash(sf::RenderWindow &window);
void PrintLose(sf::RenderWindow &window);
void PrintWin(sf::RenderWindow &window);
void EventButtons(sf::Event &event);
std::vector<std::vector<int>> dist(std::pair<int, int> f);