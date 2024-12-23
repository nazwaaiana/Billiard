#include <SFML/Graphics.hpp>
#include "Ball.cpp"
#include <cmath>
#include <vector>

class Stick {
private:
    sf::VertexArray stickShape; 
    sf::VertexArray shadowShape; 
    bool isMoving;
    sf::Vector2f startPos;
    float maxForce;
    float offsetDistance;
    const float stickLength;
    bool isReleased;
    std::vector<sf::VertexArray> dashedLine;
    sf::RectangleShape powerBar; // Bar kekuatan
    sf::RectangleShape powerBarBackground; // Latar belakang bar kekuatan

public:
    Stick()
        : stickShape(sf::TriangleStrip, 4), 
          shadowShape(sf::TriangleStrip, 4), 
          isMoving(false), maxForce(2000.0f), offsetDistance(350.0f), stickLength(550.0f), isReleased(false) {
        stickShape[0].color = sf::Color(245, 222, 179); 
        stickShape[1].color = sf::Color(245, 222, 179); 
        stickShape[2].color = sf::Color(160, 82, 45);   
        stickShape[3].color = sf::Color(160, 82, 45);  

        for (int i = 0; i < 4; ++i) {
            shadowShape[i].color = sf::Color(0, 0, 0, 100); 
        }

        // Inisialisasi bar kekuatan
        powerBarBackground.setSize(sf::Vector2f(200.0f, 20.0f));
        powerBarBackground.setFillColor(sf::Color(50, 50, 50));
        powerBarBackground.setPosition(10.0f, 10.0f);

        powerBar.setSize(sf::Vector2f(0.0f, 20.0f));
        powerBar.setFillColor(sf::Color(0, 255, 0));
        powerBar.setPosition(10.0f, 10.0f);
    }

    void startMove(sf::Vector2f ballPosition) {
        isMoving = true;
        isReleased = false;
        startPos = ballPosition;
    }

    void endMove(Ball& ball, const sf::Vector2f& mousePosition) {
        if (!isMoving) return;

        sf::Vector2f force = startPos - mousePosition;
        float distance = std::sqrt(force.x * force.x + force.y * force.y);

        float forceMagnitude = std::min(distance, maxForce);

        sf::Vector2f normalizedForce = force / std::sqrt(force.x * force.x + force.y * force.y);
        ball.applyForce(normalizedForce * forceMagnitude * 2.0f);

        isReleased = true;
        isMoving = false;
    }

    void update(sf::Vector2f ballPosition, sf::Vector2f mousePosition) {
        if (isMoving) {
            sf::Vector2f direction = mousePosition - ballPosition;
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            float angle = std::atan2(direction.y, direction.x);

            sf::Vector2f normalizedDirection = direction / distance;

            offsetDistance = std::min(distance, 200.0f);

            sf::Vector2f stickPosition = ballPosition + normalizedDirection * offsetDistance;

            // Define the tapered stick
            sf::Vector2f tipPosition = stickPosition + normalizedDirection * stickLength;

            sf::Vector2f perpendicular(-normalizedDirection.y, normalizedDirection.x);

            float buttWidth = 15.0f; // Increased width of the butt end
            float tipWidth = 5.0f;   // Increased width of the tip

            // Set the vertices for the stick
            stickShape[0].position = stickPosition + perpendicular * tipWidth / 2.0f; // Tip top
            stickShape[1].position = stickPosition - perpendicular * tipWidth / 2.0f; // Tip bottom
            stickShape[2].position = tipPosition + perpendicular * buttWidth / 2.0f;  // Butt top
            stickShape[3].position = tipPosition - perpendicular * buttWidth / 2.0f;  // Butt bottom

            // Set the vertices for the shadow (slightly offset)
            sf::Vector2f shadowOffset(5.0f, 5.0f); // Offset for the shadow
            shadowShape[0].position = stickShape[0].position + shadowOffset;
            shadowShape[1].position = stickShape[1].position + shadowOffset;
            shadowShape[2].position = stickShape[2].position + shadowOffset;
            shadowShape[3].position = stickShape[3].position + shadowOffset;

            // Update dashed line
            dashedLine.clear();
            const float dashLength = 5.0f;
            const float gapLength = 5.0f;
            const float totalLength = 500.0f; // Length of the dashed line
            sf::Vector2f currentPoint = ballPosition;

            float remainingLength = totalLength;

            while (remainingLength > 0) {
                float segmentLength = std::min(dashLength, remainingLength);
                sf::Vector2f nextPoint = currentPoint - normalizedDirection * segmentLength;

                sf::VertexArray segment(sf::Lines, 2);
                segment[0].position = currentPoint;
                segment[1].position = nextPoint;
                segment[0].color = sf::Color::White;
                segment[1].color = sf::Color::White;

                dashedLine.push_back(segment);

                currentPoint = nextPoint - normalizedDirection * gapLength; // Skip the gap
                remainingLength -= (dashLength + gapLength);
            }

            // Update power bar
            float forceMagnitude = std::min(distance, maxForce);
            float powerBarWidth = (forceMagnitude / maxForce) * 200.0f;
            powerBar.setSize(sf::Vector2f(powerBarWidth, 20.0f));
        } else if (isReleased) {
            dashedLine.clear();
            isReleased = false;
        }
    }

    void draw(sf::RenderWindow& window) {
        if (isMoving) {
            window.draw(shadowShape); 
            window.draw(stickShape);  
            for (auto& segment : dashedLine) {
                window.draw(segment);
            }
            window.draw(powerBarBackground); 
            window.draw(powerBar); 
        }
    }
};
