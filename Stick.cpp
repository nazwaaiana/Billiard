#include <SFML/Graphics.hpp>
#include "Ball.cpp"

class Stick {
private:
    sf::RectangleShape stickShape;
    bool isMoving;
    sf::Vector2f startPos;

public:
    Stick() : isMoving(false) {
        stickShape.setFillColor(sf::Color(255, 255, 255)); 
    }

    void startMove(sf::Vector2f ballPosition) {
        isMoving = true;
        startPos = ballPosition;
    }

    void endMove(Ball& ball, const sf::Vector2f& mousePosition) {
        if (!isMoving) return;

        sf::Vector2f force = startPos - mousePosition;
        float forceMagnitude = std::sqrt(force.x * force.x + force.y * force.y) * 4.0f; 
        sf::Vector2f normalizedForce = force / std::sqrt(force.x * force.x + force.y * force.y);
        ball.applyForce(normalizedForce * forceMagnitude);

        isMoving = false;
    }

    void update(sf::Vector2f ballPosition, sf::Vector2f mousePosition) {
        if (isMoving) {
            sf::Vector2f direction = mousePosition - ballPosition;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (length != 0) {
                direction /= length;
            }
           
            stickShape.setSize(sf::Vector2f(length, 10.0f)); 
            stickShape.setOrigin(0.0f, stickShape.getSize().y / 2); 
            stickShape.setPosition(ballPosition); 

            float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159f; 
            stickShape.setRotation(angle);
        }
    }

    void draw(sf::RenderWindow& window) const {
        if (isMoving) {
            window.draw(stickShape);
        }
    }
};