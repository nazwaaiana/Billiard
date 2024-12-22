#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "Ball.cpp"
#include "Stick.cpp"
#include "PoolTable.cpp"
#include "Player.cpp"
#include "Score.cpp"

bool isBallStopped(const sf::Vector2f& velocity) {
    return std::abs(velocity.x) < MinVelocity && std::abs(velocity.y) < MinVelocity;
}

void checkCollision(Ball& ball1, Ball& ball2) {
    sf::Vector2f direction = ball2.getPosition() - ball1.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 2 * BallRadius) {
        direction /= distance;

        sf::Vector2f relativeVelocity = ball2.getVelocity() - ball1.getVelocity();
        float speed = relativeVelocity.x * direction.x + relativeVelocity.y * direction.y;

        if (speed < 0) { 
            float impulse = 2 * speed / (1 / BallRadius + 1 / BallRadius);
            sf::Vector2f impulseVector = direction * impulse;

            ball1.setVelocity(ball1.getVelocity() + impulseVector / BallRadius);
            ball2.setVelocity(ball2.getVelocity() - impulseVector / BallRadius);
        }
    }
}

void drawBackground(sf::RenderWindow& window) {
    sf::RectangleShape background(sf::Vector2f(BackWidth, BackHeight));
    background.setFillColor(sf::Color(150, 0, 0));
    window.draw(background);
}

bool checkFoul(const std::vector<Ball>& balls, int currentPlayer, int player1Type, int player2Type) {
    bool foul = false;
    bool cueBallHit = false;
    bool targetBallHit = false;

    for (const auto& ball : balls) {
        if (ball.getID() == 0 && ball.isMoving()) {
            cueBallHit = true;
        } else if ((currentPlayer == 1 && player1Type != -1 && ball.getID() == player1Type) || 
                   (currentPlayer == 2 && player2Type != -1 && ball.getID() == player2Type)) {
            if (ball.isMoving()) {
                targetBallHit = true;
            }
        }
    }

    if (!cueBallHit) {
        std::cout << "Foul: Cue ball tidak menyentuh bola lain." << std::endl;
        foul = true;
    } else if ((currentPlayer == 1 && player1Type != -1 && !targetBallHit) || 
               (currentPlayer == 2 && player2Type != -1 && !targetBallHit)) {
        std::cout << "Foul: Tidak mengenai bola target terlebih dahulu." << std::endl;
        foul = true;
    }

    return foul;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(BackWidth, BackHeight), "Billiard Simulation");

    sf::Font font;
    if (!font.loadFromFile("D:/sfmll/font/Roboto-Black.ttf")) {
        std::cerr << "Error loading font\n";
        return -1;
    }

    std::vector<Ball> balls;
    balls.push_back(Ball(BallRadius, sf::Vector2f(200.0f, 330.0f), sf::Color::White, 0, font)); 

    balls.push_back(Ball(BallRadius, sf::Vector2f(650.0f, 330.0f), sf::Color(255, 255, 0), 1, font));
    balls.push_back(Ball(BallRadius, sf::Vector2f(685.0f, 310.0f), sf::Color(0, 0, 255), 2, font));
    balls.push_back(Ball(BallRadius, sf::Vector2f(685.0f, 350.0f), sf::Color(255, 0, 0), 3, font));
    balls.push_back(Ball(BallRadius, sf::Vector2f(720.0f, 290.0f), sf::Color(128, 0, 128), 4, font));
    balls.push_back(Ball(BallRadius, sf::Vector2f(720.0f, 330.0f), sf::Color(255, 165, 0), 5, font));
    balls.push_back(Ball(BallRadius, sf::Vector2f(720.0f, 370.0f), sf::Color(0, 255, 0), 6, font));
    balls.push_back(Ball(BallRadius, sf::Vector2f(755.0f, 270.0f), sf::Color(128, 0, 0), 7, font));
    balls.push_back(Ball(BallRadius, sf::Vector2f(755.0f, 310.0f), sf::Color(0, 0, 0), 8, font));

    balls.push_back(Ball(BallRadius, sf::Vector2f(755.0f, 350.0f), sf::Color(255, 255, 0), 9, font)); 
    balls.push_back(Ball(BallRadius, sf::Vector2f(755.0f, 390.0f), sf::Color(0, 0, 255), 10, font)); 
    balls.push_back(Ball(BallRadius, sf::Vector2f(790.0f, 250.0f), sf::Color(255, 0, 0), 11, font)); 
    balls.push_back(Ball(BallRadius, sf::Vector2f(790.0f, 290.0f), sf::Color(128, 0, 128), 12, font)); 
    balls.push_back(Ball(BallRadius, sf::Vector2f(790.0f, 330.0f), sf::Color(255, 165, 0), 13, font)); 
    balls.push_back(Ball(BallRadius, sf::Vector2f(790.0f, 370.0f), sf::Color(0, 255, 0), 14, font)); 
    balls.push_back(Ball(BallRadius, sf::Vector2f(790.0f, 410.0f), sf::Color(128, 0, 0), 15, font)); 

    PoolTable table;
    Stick cue;

    sf::Clock clock;
    sf::Text player1Text("Player 1", font, 25);
    sf::Text player2Text("Player 2", font, 25);

    sf::FloatRect player1Bounds = player1Text.getGlobalBounds();
    sf::FloatRect player2Bounds = player2Text.getGlobalBounds();

    player1Text.setPosition((BackWidth / 4) - (player1Bounds.width / 2), 10); 
    player2Text.setPosition((BackWidth * 3 / 4) - (player2Bounds.width / 2), 10); 

    Score player1Score(font, (BackWidth / 4) - 100, 50);
    Score player2Score(font, (BackWidth * 3 / 4) - 100, 50);

    int player1Type = -1; 
    int player2Type = -1; 

    int currentPlayer = 2; 
    bool ballPocketed = false;

    bool showPopup = false;
    int winner = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                cue.startMove(balls[0].getPosition());
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                cue.endMove(balls[0], sf::Vector2f(sf::Mouse::getPosition(window)));
            }
        }

        float deltaTime = clock.restart().asSeconds();

        for (auto& ball : balls) {
            ball.update(deltaTime);
        }
        for (size_t i = 0; i < balls.size(); ++i) {
            for (size_t j = i + 1; j < balls.size(); ++j) {
                checkCollision(balls[i], balls[j]);
            }
        }

        for (auto it = balls.begin(); it != balls.end();) {
            if (table.isPocketed(*it)) {
                int ballID = it->getID();

                if (ballID == 0) { 
                    // Bola putih masuk
                    std::cout << "Bola putih masuk ke lubang. Ganti giliran!" << std::endl;
                    it->respawn();
                    currentPlayer = (currentPlayer == 1) ? 2 : 1;
                    ++it;
                } else if (ballID == 8) { 
                    // Bola hitam masuk
                    std::cout << "Bola hitam masuk ke lubang. Permainan selesai!" << std::endl;
                    winner = (currentPlayer == 1) ? 2 : 1;
                    std::cout << "Pemenangnya adalah Player " << winner << "!" << std::endl;
                    showPopup = true;
                    break;
                } else { 
                    // Bola lain masuk
                    int ballType = (ballID >= 1 && ballID <= 7) ? 1 : 2;

                    if (currentPlayer == 1) {
                        if (player1Type == -1) {
                            player1Type = ballType;
                            player2Type = (ballType == 1) ? 2 : 1;
                            std::cout << "Player 1 memilih bola " << ((player1Type == 1) ? "solid" : "striped") << "." << std::endl;
                        }

                        if (player1Type == ballType) {
                            player1Score.addScore(ballID, it->getColor());
                        } else {
                            std::cout << "Player 1 memasukkan bola lawan. Ganti giliran!" << std::endl;
                            currentPlayer = 2;
                            player2Score.addScore(ballID, it->getColor());
                        }
                    } else {
                        if (player2Type == -1) {
                            player2Type = ballType;
                            player1Type = (ballType == 1) ? 2 : 1;
                            std::cout << "Player 2 memilih bola " << ((player2Type == 1) ? "solid" : "striped") << "." << std::endl;
                        }

                        if (player2Type == ballType) {
                            player2Score.addScore(ballID, it->getColor());
                        } else {
                            std::cout << "Player 2 memasukkan bola lawan. Ganti giliran!" << std::endl;
                            currentPlayer = 1;
                            player1Score.addScore(ballID, it->getColor());
                        }
                    }
                    ballPocketed = true;
                    it = balls.erase(it);
                }
            } else {
                ++it;
            }
        }
        static bool turnEnded = false;
        if (isBallStopped(balls[0].getVelocity()) && !turnEnded) {
            if (ballPocketed) {
                std::cout << "Bola masuk! Pemain tetap melanjutkan giliran." << std::endl;
                turnEnded = true;
                ballPocketed = false;

            } else {
                if (checkFoul(balls, currentPlayer, player1Type, player2Type)) {
                    currentPlayer = (currentPlayer == 1) ? 2 : 1;
                    std::cout << "Foul terjadi. Ganti giliran ke pemain " << currentPlayer << "." << std::endl;

                } else {
                    currentPlayer = (currentPlayer == 1) ? 2 : 1;
                    std::cout << "Tidak ada bola masuk. Ganti giliran ke pemain " << currentPlayer << "." << std::endl;
                }
                turnEnded = true;
            }
        }

        if (!isBallStopped(balls[0].getVelocity())) {
            turnEnded = false;
        }

        for (size_t i = 0; i < balls.size(); ) { 
            if (table.isPocketed(balls[i])) {
                if (i == 0) { 
                    balls[i].respawn(); 
                    ++i;
                } else { 
                    balls.erase(balls.begin() + i); 
                }
            } else {
                ++i;
            }
        }

        cue.update(balls[0].getPosition(), sf::Vector2f(sf::Mouse::getPosition(window)));

        player1Text.setFillColor((currentPlayer == 1) ? sf::Color::White : sf::Color(100, 100, 100));
        player2Text.setFillColor((currentPlayer == 2) ? sf::Color::White : sf::Color(100, 100, 100));

        window.clear();
        drawBackground(window);
        table.draw(window);
        window.draw(player1Text);
        window.draw(player2Text);

        player1Score.draw(window);
        player2Score.draw(window);

        for (const auto& ball : balls) {
            ball.draw(window, font);
        }

        cue.draw(window);

        if (showPopup) {
            sf::RectangleShape popup(sf::Vector2f(400.0f, 200.0f));
            popup.setFillColor(sf::Color(50, 50, 50));
            popup.setOutlineColor(sf::Color::White);
            popup.setOutlineThickness(5);
            popup.setPosition((BackWidth - 400) / 2, (BackHeight - 200) / 2);

            sf::Text winnerText("The Winner Is " + std::to_string(winner) + "!", font, 25);
            winnerText.setFillColor(sf::Color::White);
            sf::FloatRect textBounds = winnerText.getGlobalBounds();
            winnerText.setPosition((BackWidth - textBounds.width) / 2, (BackHeight - 200) / 2 + 40);

            sf::RectangleShape closeButton(sf::Vector2f(100.0f, 50.0f));
            closeButton.setFillColor(sf::Color(100, 100, 100));
            closeButton.setPosition((BackWidth - 100) / 2, (BackHeight - 200) / 2 + 120);

            sf::Text closeText("Close", font, 20);
            closeText.setFillColor(sf::Color::White);
            sf::FloatRect closeTextBounds = closeText.getGlobalBounds();
            closeText.setPosition((BackWidth - closeTextBounds.width) / 2, (BackHeight - 200) / 2 + 135);

            window.draw(popup);
            window.draw(winnerText);
            window.draw(closeButton);
            window.draw(closeText);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                if (closeButton.getGlobalBounds().contains(mousePosF)) {
                    window.close();
                }
            }
        }
        window.display();
    }

    return 0;
}
