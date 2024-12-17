#include <SFML/Graphics.hpp>
#include "RoundedRectangleShape.cpp"
#include "Constants.hpp"
#include "Ball.cpp"

class PoolTable {
private:
    sf::RectangleShape tableShape;
    RoundedRectangleShape borderShape; // Use RoundedRectangleShape
    sf::RectangleShape cushionShape;
    std::vector<sf::CircleShape> pockets;
    float pocketRadius;

    void setupPockets() {
        std::vector<sf::Vector2f> pocketPositions = {
            {TableBorder - pocketRadius, TableBorder - pocketRadius},            
            {(WindowWidth / 2) - pocketRadius, TableBorder - pocketRadius}, 
            {WindowWidth - TableBorder - pocketRadius, TableBorder - pocketRadius}, 
            {TableBorder - pocketRadius, WindowHeight - TableBorder - pocketRadius}, 
            {(WindowWidth / 2) - pocketRadius, WindowHeight - TableBorder - pocketRadius},
            {WindowWidth - TableBorder - pocketRadius, WindowHeight - TableBorder - pocketRadius} 
        };
    for (const auto& position : pocketPositions) {
            sf::CircleShape pocket(pocketRadius);
            pocket.setFillColor(sf::Color::Black);
            pocket.setPosition(position + sf::Vector2f(OFFSET_X, OFFSET_Y)); // Apply offset
            pockets.push_back(pocket);
        }
    }

public:
    PoolTable() : pocketRadius(25.0f) {
        tableShape.setSize(sf::Vector2f(TableWidth, TableHeight));
        tableShape.setFillColor(sf::Color(0, 49, 0));
        tableShape.setPosition(TableBorder + OFFSET_X, TableBorder + OFFSET_Y); // Apply offset

        borderShape.setSize(sf::Vector2f(WindowWidth, WindowHeight));
        borderShape.setFillColor(sf::Color(40, 119, 61));
        borderShape.setPosition(OFFSET_X, OFFSET_Y); // Apply offset
        borderShape.setCornersRadius(20.0f); // Set corner radius

        cushionShape.setSize(sf::Vector2f(TableWidth + 20, TableHeight + 20));
        cushionShape.setFillColor(sf::Color(50, 205, 50));
        cushionShape.setPosition(TableBorder - 10 + OFFSET_X, TableBorder - 10 + OFFSET_Y); // Apply offset

        setupPockets();
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(borderShape);
        window.draw(cushionShape);
        window.draw(tableShape);
        for (const auto& pocket : pockets) {
            window.draw(pocket);
        }
    }

    bool isPocketed(Ball& ball) {
        for (const auto& pocket : pockets) {
            sf::FloatRect pocketBounds = pocket.getGlobalBounds();
            if (pocketBounds.contains(ball.getPosition())) {
                return true;
            }
        }
        return false;
    }
};