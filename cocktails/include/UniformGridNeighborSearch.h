#ifndef PBS_UNIFORMGRIDNEIGHBORSEARCH_H
#define PBS_UNIFORMGRIDNEIGHBORSEARCH_H


#include <map>
#include <tuple>
#include "NeighborSearch.h"

class UniformGridNeighborSearch : public NeighborSearch {

public:
    UniformGridNeighborSearch(double gridWidth);

    std::vector<Particle*> getNeighbors(Eigen::Vector3d& position, double radius) const override;

    void addParticle(Particle *particle) override;

    void updateParticle(Particle *particle) override;

private:
    double gridWidth;

    std::map<int, std::map<int, std::map<int, std::vector<Particle*>>>> uniformGrid;
    std::map<Particle*, std::tuple<int, int, int>> particleIndex;

    std::tuple<int, int, int> getIndex(Eigen::Vector3d& position) const;
};


#endif //PBS_UNIFORMGRIDNEIGHBORSEARCH_H
