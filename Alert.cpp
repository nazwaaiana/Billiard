#include <SFML/Graphics.hpp>
#include <string>

class Alert {
private:
    sf::RectangleShape popup;
    sf::Text winnerText;
    sf::RectangleShape closeButton;
    sf::Text closeText;
    bool showPopup;

public:
    Alert(const sf::Font& font, const sf::Vector2f& windowSize)
        : showPopup(false) {
        popup.setSize(sf::Vector2f(400.0f, 200.0f));
        popup.setFillColor(sf::Color(50, 50, 50));
        popup.setOutlineColor(sf::Color::White);
        popup.setOutlineThickness(5);
        popup.setPosition((windowSize.x - 400) / 2, (windowSize.y - 200) / 2);

        winnerText.setFont(font);
        winnerText.setCharacterSize(25);
        winnerText.setFillColor(sf::Color::White);

        closeButton.setSize(sf::Vector2f(100.0f, 50.0f));
        closeButton.setFillColor(sf::Color(100, 100, 100));
        closeButton.setPosition((windowSize.x - 100) / 2, (windowSize.y - 200) / 2 + 120);

        closeText.setFont(font);
        closeText.setString("Close");
        closeText.setCharacterSize(20);
        closeText.setFillColor(sf::Color::White);
        sf::FloatRect closeTextBounds = closeText.getGlobalBounds();
        closeText.setPosition((windowSize.x - closeTextBounds.width) / 2, (windowSize.y - 200) / 2 + 135);
    }

    void show(int winner) {
        winnerText.setString("The Winner Is Player " + std::to_string(winner) + "!");
        sf::FloatRect textBounds = winnerText.getGlobalBounds();
        winnerText.setPosition((popup.getPosition().x + (popup.getSize().x - textBounds.width) / 2), popup.getPosition().y + 40);
        showPopup = true;
    }

    void hide() {
        showPopup = false;
    }

    bool isVisible() const {
        return showPopup;
    }

    void draw(sf::RenderWindow& window) {
        if (showPopup) {
            window.draw(popup);
            window.draw(winnerText);
            window.draw(closeButton);
            window.draw(closeText);
        }
    }

    bool isCloseButtonPressed(const sf::Vector2f& mousePos) const {
        return closeButton.getGlobalBounds().contains(mousePos);
    }
};