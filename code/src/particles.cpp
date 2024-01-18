// C++ Standard Template Library (STL)
#include <iostream>
#include <cstdlib> // to use random

// Importing header files
#include "../include/particles.hpp"

Particles::Particles() {
    //initialize
}

Particles::~Particles() {
    // delete memories here
}

void Particles::update(float delta) {
    for (auto it = particles.begin(); it != particles.end(); it++) {
        Particle& p = *it;
        if (p.life > 0.0f) {
            //if it's active
            p.life -= delta;
        }
    }
}

void Particles::triggerParticles(const glm::vec3& blockPosition) {
    for (auto& particle : particles) {
        if (particle.life <= 0.0f) {

            //gets activated based on the block position
            particle.position = blockPosition;

            //randomizing the colors for now
            particle.r = rand() % 256;
            particle.g = rand() % 256;
            particle.b = rand() % 256;
            particle.a = 100;

            particle.life = 2.0f;
        }
    }
}
