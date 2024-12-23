#include <SFML/Graphics.hpp>
#include "Constants.hpp"
#include <iostream>

class StartMenu {
public:
    StartMenu() {
        if (!backgroundTexture.loadFromFile("D:/sfmll/bg/menu-bg.jpg")) {
            std::cerr << "Failed to load background image!" << std::endl;
        } else {
            backgroundSprite.setTexture(backgroundTexture);
            sf::Vector2u windowSize = backgroundTexture.getSize();
            sf::Vector2f scaleFactor(
                static_cast<float>(BackWidth) / windowSize.x,
                static_cast<float>(BackHeight) / windowSize.y
            );
            backgroundSprite.setScale(scaleFactor);
        }
    }
    ~StartMenu() {}

    void show(sf::RenderWindow& window, sf::Font& font) {
        window.clear();
        window.draw(backgroundSprite);
        drawText(window, "Billiard Game", font, 50, window.getSize().x / 2, 100);
        drawText(window, "Press ENTER to Start", font, 30, window.getSize().x / 2, 250);
        window.display();
    }

    bool handleInput(sf::RenderWindow& window) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            } 
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
                return true;
            }
        }
        return false;
    }

private:
    void drawText(sf::RenderWindow& window, const std::string& text, sf::Font& font, unsigned int size, float x, float y) {
        sf::Text drawText(text, font, size);
        drawText.setPosition(x - drawText.getLocalBounds().width / 2, y);
        window.draw(drawText);
    }
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
};