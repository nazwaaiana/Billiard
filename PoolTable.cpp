#include <SFML/Graphics.hpp>
#include "RoundedRectangleShape.cpp"
#include "Constants.hpp"
#include "Ball.cpp"

class PoolTable {
private:
    sf::RectangleShape tableShape;
    RoundedRectangleShape borderShape; 
    sf::RectangleShape cushionShape;
    std::vector<sf::CircleShape> pockets;
    float pocketRadius;

    sf::Texture borderTexture;
    sf::Texture cushionTexture;
    sf::Texture tableTexture;

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
        if (!borderTexture.loadFromFile("D:/sfmll/bg/border_texture.jpg")) {
        }
        if (!cushionTexture.loadFromFile("D:/sfmll/bg/border_texture.jpg")) {
        }
        if (!tableTexture.loadFromFile("D:/sfmll/bg/green_texture.jpg")) {
        }

        tableShape.setSize(sf::Vector2f(TableWidth, TableHeight));
        tableShape.setTexture(&tableTexture); 
        tableShape.setPosition(TableBorder + OFFSET_X, TableBorder + OFFSET_Y); 

        borderShape.setSize(sf::Vector2f(WindowWidth, WindowHeight));
        borderShape.setTexture(&borderTexture); 
        borderShape.setPosition(OFFSET_X, OFFSET_Y); 
        borderShape.setCornersRadius(20.0f); 

        cushionShape.setSize(sf::Vector2f(TableWidth + 20, TableHeight + 20));
        cushionShape.setTexture(&cushionTexture); 
        cushionShape.setPosition(TableBorder - 10 + OFFSET_X, TableBorder - 10 + OFFSET_Y);

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
