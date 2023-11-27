#include <SFML/Graphics.hpp>
#include <vector>

// Particle class
class Particle {
public:
    sf::Vector2f position;
    sf::Vector2f velocity;

    Particle(float x, float y) : position(x, y), velocity(0, 0) {}

    void update() {
        position += velocity;
    }
};

int main() {
    // Window settings
    const int windowWidth = 800;
    const int windowHeight = 600;

    // SFML window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML Particle System");

    // Particle container
    std::vector<Particle> particles;

    // Main loop
    while (window.isOpen()) {
        // Event handling
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Mouse interaction
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            particles.emplace_back(mousePosition.x, mousePosition.y);
        }

        // Update particles
        for (auto& particle : particles) {
            // Simulate simple gravity
            particle.velocity.y += 0.1f;

            // Update particle position
            particle.update();
        }

        // Clear the window
        window.clear();

        // Draw particles
        for (const auto& particle : particles) {
            sf::CircleShape circle(5.0f);
            circle.setPosition(particle.position);
            window.draw(circle);
        }

        // Display the window
        window.display();
    }

    return 0;
}
