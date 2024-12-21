#include <SFML/Graphics.hpp>
#include <iostream>
#include "Ball.cpp"
#include "Stick.cpp"
#include "PoolTable.cpp"

bool isBallStopped(const sf::Vector2f& velocity) {
    return std::abs(velocity.x) < VELOCITY_THRESHOLD && std::abs(velocity.y) < VELOCITY_THRESHOLD;
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

    sf::Text player1Text("Player 1", font, 30);
    sf::Text player2Text("Player 2", font, 30);

    player1Text.setPosition(50, 50);
    player2Text.setPosition(BackWidth - 150, 50);

    // Variabel pemain
    int currentPlayer = 1; // 1 untuk Player 1, 2 untuk Player 2
    bool ballPocketed = false;

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
                if (it == balls.begin()) {
                    // Jika bola putih masuk
                    std::cout << "Bola putih masuk ke lubang!" << std::endl;
                    it->respawn(); 
                    ++it;
                } else {
                    // Jika bola warna masuk
                    std::cout << "Bola warna masuk" << std::endl;
                    it = balls.erase(it); 
                    ballPocketed = true; // Tandai bahwa bola masuk
                }
            } else {
                ++it;
            }
        }

        static bool turnEnded = false; // Variabel untuk mengecek akhir giliran
        if (isBallStopped(balls[0].getVelocity()) && !turnEnded) {
            if (ballPocketed) {
                // Jika bola berhasil masuk, pemain tetap melanjutkan giliran
                std::cout << "Bola masuk! Pemain tetap melanjutkan giliran." << std::endl;
                turnEnded = true;
                ballPocketed = false;
            } else {
                // Jika tidak ada bola masuk, ganti giliran pemain
                currentPlayer = (currentPlayer == 1) ? 2 : 1;
                std::cout << "Tidak ada bola masuk. Ganti giliran ke pemain " << currentPlayer << "." << std::endl;
                turnEnded = true;
            }
        }

        // Jika bola mulai bergerak lagi, reset kondisi akhir giliran
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
        for (const auto& ball : balls) {
            ball.draw(window, font);
        }

        cue.draw(window);
        window.display();
    }

    return 0;
}
