#include "../include/BruteForceNeighborSearch.h"

BruteForceNeighborSearch::BruteForceNeighborSearch() {

}

void BruteForceNeighborSearch::reset() {
    particles.clear();
}

std::vector<Particle*> BruteForceNeighborSearch::getNeighbors(Particle* particle, double radius) const {
    double epsilon = 1e-4;

    std::vector<Particle*> neighbors;
    for (auto& neighbor : particles) {
        auto distance = (neighbor->m_pos - particle->m_pos).norm();
        if (distance <= radius + epsilon) {
            neighbors.push_back(neighbor);
        }
    }
    return neighbors;
}

void BruteForceNeighborSearch::addParticle(Particle *particle) {
    particles.push_back(particle);
}