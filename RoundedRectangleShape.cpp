#include <SFML/Graphics.hpp>
#include <cmath>

class RoundedRectangleShape : public sf::Shape {
public:
    explicit RoundedRectangleShape(const sf::Vector2f& size = sf::Vector2f(0, 0), float radius = 0, std::size_t pointCount = 30) :
        m_size(size), m_radius(radius), m_pointCount(pointCount) {
        update();
    }

    void setSize(const sf::Vector2f& size) {
        m_size = size;
        update();
    }

    const sf::Vector2f& getSize() const {
        return m_size;
    }

    void setCornersRadius(float radius) {
        m_radius = radius;
        update();
    }

    float getCornersRadius() const {
        return m_radius;
    }

    void setPointCount(std::size_t count) {
        m_pointCount = count;
        update();
    }

    std::size_t getPointCount() const override {
        return m_pointCount * 4;
    }

    sf::Vector2f getPoint(std::size_t index) const override {
        if (index >= m_pointCount * 4)
            return sf::Vector2f(0, 0);

        float deltaAngle = 90.0f / (m_pointCount - 1);
        sf::Vector2f center;
        unsigned int centerIndex = index / m_pointCount;
        static const float pi = 3.141592654f;

        switch (centerIndex) {
        case 0: center.x = m_size.x - m_radius; center.y = m_radius; break;
        case 1: center.x = m_radius; center.y = m_radius; break;
        case 2: center.x = m_radius; center.y = m_size.y - m_radius; break;
        case 3: center.x = m_size.x - m_radius; center.y = m_size.y - m_radius; break;
        }

        return sf::Vector2f(m_radius * cos(deltaAngle * (index - centerIndex) * pi / 180) + center.x,
                            -m_radius * sin(deltaAngle * (index - centerIndex) * pi / 180) + center.y);
    }

private:
    sf::Vector2f m_size;
    float m_radius;
    std::size_t m_pointCount;
};