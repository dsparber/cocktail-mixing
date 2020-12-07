#ifndef PBS_NEIGHBOR_SEARCH_H
#define PBS_NEIGHBOR_SEARCH_H

#include <vector>
#include <map>
#include "Particle.h"

class NeighborSearch {

public:
    virtual std::vector<Particle*> getNeighbors(Particle* particle, double radius) const = 0;

    virtual std::vector<Particle*> getNeighbors(Eigen::Vector3d& pos, double radius) const = 0;

    virtual ~NeighborSearch() {}

    virtual void reset() = 0;

    virtual void addParticle(Particle* particle) = 0;
};


#endif //PBS_NEIGHBOR_SEARCH_H
