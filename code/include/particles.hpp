// Header file for particles

#include <vector>
#include <glm/glm.hpp>

struct Particle {
    glm::vec3 position;
    unsigned char r, g, b, a; // for the color
    float life; // Remaining life of the particle. if < 0 : dead and unused.
};

class Particles {
public:
    Particles(); // constructor
    ~Particles(); // destructor
    void update(float delta);
    void triggerParticles(const glm::vec3& blockPosition);

private:
    std::vector<Particle> particles;
};

