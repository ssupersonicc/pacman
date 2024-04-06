#pragma once
#include <SFML/Graphics.hpp>

void CreateCharacters();
void EventProcessing(sf::Event &event);
void UpdateCharactersPosition();
void DrawCharacters(sf::RenderWindow &window);

void DidPacmanEat();