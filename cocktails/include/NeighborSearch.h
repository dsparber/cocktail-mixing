#ifndef PBS_NEIGHBORSEARCH_H
#define PBS_NEIGHBORSEARCH_H

#include <vector>
#include <map>
#include "Particle.h"

class NeighborSearch {

public:
    virtual std::vector<Particle*> getNeighbors(Eigen::Vector3d& position, double radius) const = 0;

    virtual void addParticle(Particle* particle) = 0;

    virtual void updateParticle(Particle* particle) = 0;

    void addParticles(std::vector<Particle*> particles) {
        for (auto particle : particles) {
            addParticle(particle);
        }
    }

    void updateParticles(std::vector<Particle*> particles) {
        for (auto particle : particles) {
            updateParticle(particle);
        }
    }
};


#endif //PBS_NEIGHBORSEARCH_H
