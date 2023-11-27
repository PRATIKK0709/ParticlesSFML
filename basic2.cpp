#include <SFML/Graphics.hpp>
#include <vector>

class Particle {
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    float size;

    Particle(float x, float y) : position(x, y), velocity(0, 0), size(5.0f) {
        color = sf::Color(rand() % 256, rand() % 256, rand() % 256);
    }

    void update() {
        velocity.y += 0.05f; // Smoother gravity
        position += velocity;

        // Velocity decay for a more natural motion
        velocity *= 0.99f;
    }

    void draw(sf::RenderWindow& window) const {
        sf::CircleShape circle(size);
        circle.setPosition(position);
        circle.setFillColor(color);
        window.draw(circle);
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    const int windowWidth = 800;
    const int windowHeight = 600;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML Particle System");
    window.setFramerateLimit(60);

    std::vector<Particle> particles;

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            particles.emplace_back(mousePosition.x, mousePosition.y);
        }

        for (auto& particle : particles) {
            particle.update();
        }

        window.clear();

        for (const auto& particle : particles) {
            particle.draw(window);
        }

        window.display();

        // Frame timing for smoother animation
        sf::Time elapsed = clock.restart();
        sf::sleep(sf::milliseconds(16) - elapsed); // Target frame time of ~60 FPS
    }

    return 0;
}
