#ifndef PBS_BRUTE_FORCE_NEIGHBOR_SEARCH_H
#define PBS_BRUTE_FORCE_NEIGHBOR_SEARCH_H

#include <map>
#include <tuple>
#include "NeighborSearch.h"

/// Iterates over all particles to find neighbors
/// Should only be used to verify a more efficient implementation
class BruteForceNeighborSearch : public NeighborSearch {

public:
    BruteForceNeighborSearch();

    void reset() override;

    std::vector<Particle*> getNeighbors(Particle* particle, double radius) const override;

    void addParticle(Particle *particle) override;

private:
    std::vector<Particle*> particles;

};


#endif //PBS_UNIFORM_GRID_NEIGHBOR_SEARCH_H
