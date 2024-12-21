#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Ball.cpp"

class Score {
private:
    sf::Font font;
    float posX, posY;
    std::vector<std::pair<sf::CircleShape, sf::Text>> scoreCircles; 
    int score; 

public:
    Score(const sf::Font& font, float x, float y) : font(font), posX(x), posY(y), score(0) {}

    void addScore(int ballID, sf::Color ballColor) {
        sf::CircleShape circle(15.0f); 
        circle.setFillColor(ballColor);   
        circle.setOutlineColor(sf::Color::Black);
        circle.setOutlineThickness(1);
        circle.setPosition(posX + scoreCircles.size() * (2 * 15.0f + 5), posY);

        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(ballID));
        text.setCharacterSize(14); 
        text.setFillColor(sf::Color::Black);
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.width / 1, textBounds.height / 1);
        text.setPosition(circle.getPosition().x + BallRadius, circle.getPosition().y + BallRadius);

        scoreCircles.push_back(std::make_pair(circle, text)); 
        score++; 
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& pair : scoreCircles) {
            window.draw(pair.first); 
            window.draw(pair.second); 
        }
    }

    int getScore() const {
        return score; 
    }
};