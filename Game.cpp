#include"Game.h"

//конструктор (создание окна)
Game::Game(int rows, int cols) : board(rows, cols), window(sf::VideoMode(cols * cellSize, rows * cellSize), "GEMS") 
{
    board.fillEmptySpaces();
}

void Game::run()
{
    sf::Clock clock;
    
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                handleMouseClick(event.mouseButton.x, event.mouseButton.y);
            }
        }
    }
}

void Game::render()
{
    window.clear(sf::Color::White);

    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
    for (int i = 0; i < board.getRows(); i++)
    {
        for (int j = 0; j < board.getCols(); j++)
        {
            Color color = board.getCellColor(i, j);
            if (color == EMPTY || board.getAlpha(i, j) <= 0)
                continue;
            
            cell.setPosition(j * cellSize, i * cellSize + board.getFallOffset(i, j).y);
            switch (board.getCellColor(i, j))
            {
            case RED: cell.setFillColor(sf::Color::Red); break;
            case GREEN: cell.setFillColor(sf::Color::Green); break;
            case BLUE: cell.setFillColor(sf::Color::Blue); break;
            case YELLOW: cell.setFillColor(sf::Color::Yellow); break;
            case PURPLE: cell.setFillColor(sf::Color::Magenta); break;
            }

            sf::Color c = cell.getFillColor();
            c.a = static_cast<sf::Uint8>(board.getAlpha(i, j));
            cell.setFillColor(c);

            window.draw(cell);

            if (board.hasBonus(i, j))
            {
                drawBonus(i, j, cell.getPosition());
            }
        }
    }
    drawSelection();
    window.display();
}

void Game::drawBonus(int row, int col, const sf::Vector2f& pos)
{
    if (!board.hasBonus(row, col)) return;

    sf::CircleShape indicator(cellSize / 4);
    indicator.setPosition(pos.x + cellSize / 4, pos.y + cellSize / 4);
    indicator.setFillColor(board.getBonusColor(row, col));

    window.draw(indicator);
}

//обработка клика мыши
void Game::handleMouseClick(int x, int y)
{
    if (board.isAnimating()) return;

    int col = x / cellSize;
    int row = y / cellSize;

    if (row >= 0 && row < board.getRows() && col >= 0 && col < board.getCols())
    {
        if (isFirstClick)
        {
            firstSelectedCell = sf::Vector2i(col, row);
            isFirstClick = false;
        }
        else
        {
            secondSelectedCell = sf::Vector2i(col, row);
            SwapCells();
            isFirstClick = true;
        }
    }
}

//обмен клеток
void Game::SwapCells()
{
    int dx = abs(firstSelectedCell.x - secondSelectedCell.x);
    int dy = abs(firstSelectedCell.y - secondSelectedCell.y);

    if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1))
    {
        board.handlePlayerMove(firstSelectedCell.y, firstSelectedCell.x, secondSelectedCell.y, secondSelectedCell.x);
    }
}

//отрисовка выделения
void Game::drawSelection()
{
    if (!isFirstClick)
    {
        sf::RectangleShape selection(sf::Vector2f(cellSize - 2, cellSize - 2));
        selection.setPosition(firstSelectedCell.x * cellSize + 1, firstSelectedCell.y * cellSize + 1);
        selection.setFillColor(sf::Color::Transparent);
        selection.setOutlineThickness(2);
        selection.setOutlineColor(sf::Color::White);
        window.draw(selection);
    }
}

void Game::update(float deltaTime)
{
    board.update(deltaTime);
}





