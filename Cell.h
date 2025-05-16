#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <variant>
#include "Bonus.h"

enum Color { RED, GREEN, BLUE, YELLOW, PURPLE, EMPTY };

typedef struct Cell {
    Color color = EMPTY;
    std::shared_ptr<Bonus> bonus;
    

    sf::Vector2f bonusSource{ -1, -1 }; //координаты клетки, откуда выпал бонус.
    bool bonusActivated = false;
    bool toBeDestroyed = false;

    sf::Vector2f fallOffset{ 0, 0 }; // смещение при падении
    float alpha = 255; // прозрачность (для исчезновения)

}Cell;



