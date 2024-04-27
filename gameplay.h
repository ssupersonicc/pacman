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
void CheckEnd(sf::RenderWindow &window);