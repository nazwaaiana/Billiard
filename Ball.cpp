#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Constants.hpp"

class Ball {
private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    sf::Vector2f initialPosition; 
    bool isStriped;
    sf::Text text;
    int id;

public:
    Ball(float radius, sf::Vector2f position, sf::Color color, int id, const sf::Font& font)
    : velocity(0.0f, 0.0f), initialPosition(position), isStriped(false), id(id) {

        if (id >= 9 && id <= 15) {
            isStriped = true; 
        }

        shape.setRadius(radius);
        shape.setFillColor(color);
        shape.setOrigin(radius, radius);
        shape.setPosition(position + sf::Vector2f(OFFSET_X, OFFSET_Y)); 
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::Black);

        text.setFont(font);
        text.setString(std::to_string(id)); 
        text.setCharacterSize(radius);      
        text.setFillColor(sf::Color::Black);
        text.setStyle(sf::Text::Bold);
        
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
        text.setPosition(shape.getPosition());
    }


    void update(float deltaTime) {
        sf::Vector2f newPosition = shape.getPosition() + velocity * deltaTime;

        if (newPosition.x - BallRadius < TableBorder + OFFSET_X || newPosition.x + BallRadius > WindowWidth - TableBorder + OFFSET_X) {
            velocity.x = -velocity.x;
        }
        if (newPosition.y - BallRadius < TableBorder + OFFSET_Y || newPosition.y + BallRadius > WindowHeight - TableBorder + OFFSET_Y) {
            velocity.y = -velocity.y;
        }

        float friction = 0.99f; 
        velocity *= std::pow(friction, deltaTime * 120); 

        shape.setPosition(shape.getPosition() + velocity * deltaTime);

        if (std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y) < MinVelocity) {
            velocity = sf::Vector2f(0.0f, 0.0f);
        }

        shape.setPosition(shape.getPosition() + velocity * deltaTime);
        text.setPosition(shape.getPosition()); // Update posisi teks
    }

    void applyForce(sf::Vector2f force) {
        velocity += force; 
    }

    void draw(sf::RenderWindow& window, sf::Font& font) const {
        window.draw(shape);

        if (isStriped) {
            sf::CircleShape centerCircle(BallRadius / 1.5);
            centerCircle.setFillColor(sf::Color::White);
            centerCircle.setPosition(shape.getPosition());
            centerCircle.setOrigin(centerCircle.getRadius(), centerCircle.getRadius());
            window.draw(centerCircle);
        }

        if (id > 0) {
            sf::Text number;
            number.setFont(font);
            number.setString(std::to_string(id));
            number.setCharacterSize(18);
            number.setStyle(sf::Text::Bold);
            
            sf::Vector2f ballPosition = shape.getPosition();
            number.setPosition(ballPosition.x - BallRadius / 2.5f, ballPosition.y - BallRadius / 1.5f);

            if (id == 8) {
                number.setFillColor(sf::Color::White);
            } else {
                number.setFillColor(sf::Color::Black);
            }

            window.draw(number);
        }
    }

    sf::Vector2f getPosition() const {
        return shape.getPosition();
    }

    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds();
    }

    sf::Vector2f getVelocity() const {
        return velocity;
    }

    void setVelocity(const sf::Vector2f& newVelocity) {
        velocity = newVelocity;
    }

    void respawn() {
        shape.setPosition(initialPosition + sf::Vector2f(OFFSET_X, OFFSET_Y)); // Apply offset
        velocity = sf::Vector2f(0.0f, 0.0f); 
    }
};