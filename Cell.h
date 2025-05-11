#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

enum Color { RED, GREEN, BLUE, YELLOW, PURPLE, EMPTY };
enum BonusType { NONE, RECOLOR, BOMB };


typedef struct Cell {
    Color color = EMPTY;
    BonusType bonus = NONE;
    sf::Vector2f bonusSource{ -1, -1 }; //координаты клетки, откуда выпал бонус.
    bool bonusActivated = false;
    bool toBeDestroyed = false;

    sf::Vector2f fallOffset{ 0, 0 }; // смещение при падении
    float alpha = 255; // прозрачность (для исчезновения)
}Cell;



