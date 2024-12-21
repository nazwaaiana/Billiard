#include <SFML/Graphics.hpp>
#include <string>

class Player {
private:
    std::string name;
    sf::Text text;
    int score;
    bool isActive;

public:
    Player(const std::string& name, const sf::Font& font, const sf::Vector2f& position)
        : name(name), score(0), isActive(false) {
        text.setFont(font);
        text.setString(name);
        text.setCharacterSize(30);
        text.setPosition(position);
    }

    void setActive(bool active) {
        isActive = active;
        text.setFillColor(active ? sf::Color::White : sf::Color(100, 100, 100));
    }

    bool getActive() const {
        return isActive;
    }

    void addScore(int points) {
        score += points;
    }

    int getScore() const {
        return score;
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(text);
    }
};