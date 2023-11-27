#include <SFML/Graphics.hpp>
#include <vector>

class Particle {
public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    float size;
    float lifespan;

    Particle(float x, float y) : position(x, y), velocity(0, 0), size(5.0f), lifespan(255.f) {
        color = sf::Color(rand() % 256, rand() % 256, rand() % 256);
    }

    void update() {
        velocity.y += 0.05f; // Smoother gravity
        position += velocity;

        // Velocity decay for a more natural motion
        velocity *= 0.99f;

        // Decrease lifespan
        lifespan -= 1.0f;
        if (lifespan < 0) lifespan = 0;
    }

    void draw(sf::RenderWindow& window) const {
        sf::CircleShape circle(size);
        circle.setPosition(position);
        circle.setFillColor(sf::Color(color.r, color.g, color.b, static_cast<sf::Uint8>(lifespan)));
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

    const float BURST_PARTICLES = 20.0f;
    const float INTERACTION_RADIUS = 50.0f;
    const float PARTICLE_SIZE_MULTIPLIER = 2.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    particles.emplace_back(mousePosition.x, mousePosition.y);
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    for (int i = 0; i < BURST_PARTICLES; ++i) {
                        Particle particle(mousePosition.x, mousePosition.y);
                        particle.velocity = sf::Vector2f(rand() % 200 - 100, rand() % 200 - 100);
                        particles.push_back(particle);
                    }
                }
            }
        }

        for (auto& particle : particles) {
            particle.update();

            // Particle interaction (repulsion)
            for (const auto& other : particles) {
                if (&particle != &other) {
                    sf::Vector2f diff = particle.position - other.position;
                    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

                    if (distance < INTERACTION_RADIUS) {
                        float repulsionStrength = 1.0f / distance;
                        particle.velocity += repulsionStrength * diff;
                    }
                }
            }
        }

        // Gradually fade out particles for smooth trail effect
        sf::RectangleShape fadeOutRect(sf::Vector2f(windowWidth, windowHeight));
        fadeOutRect.setFillColor(sf::Color(0, 0, 0, 15)); // Semi-transparent black
        window.draw(fadeOutRect);

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
