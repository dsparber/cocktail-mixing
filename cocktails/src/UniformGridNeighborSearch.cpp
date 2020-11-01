#include "../include/UniformGridNeighborSearch.h"
#include <algorithm>
#include <iostream>

UniformGridNeighborSearch::UniformGridNeighborSearch(double gridWidth) : gridWidth(gridWidth) {}

std::vector<Particle*> UniformGridNeighborSearch::getNeighbors(Eigen::Vector3d& position, double radius) const {
    int x, y, z;
    std::tie(x, y, z) = getIndex(position);

    int delta = int(ceil(radius / gridWidth));
    std::vector<Particle*> neighbors;
    for (int dx = -delta; dx <= delta; ++dx) {
        auto gridX = uniformGrid.find(x);
        if (gridX == uniformGrid.end()) {
            continue;
        }
        for (int dy = -delta; dy <= delta; ++dy) {
            auto gridXY = gridX->second.find(y + dy);
            if (gridXY == gridX->second.end()) {
                continue;
            }
            for (int dz = -delta; dz <= delta; ++dz) {
                auto gridXYZ = gridXY->second.find(y + dy);
                if (gridXYZ == gridXY->second.end()) {
                    continue;
                }
                auto particles = gridXYZ->second;
                for (auto particle : particles) {
                    if ((position - particle->m_pos).norm() <= radius) {
                        neighbors.push_back(particle);
                    }
                }
            }
        }
    }
    return neighbors;
}

void UniformGridNeighborSearch::addParticle(Particle *particle) {
    int x, y, z;
    std::tie(x, y, z) = getIndex(particle->m_pos);

    uniformGrid[x][y][z].push_back(particle);
    particleIndex[particle] = {x, y, z};
}

void UniformGridNeighborSearch::updateParticle(Particle *particle) {

    // Remove old
    int x, y, z;
    std::tie(x, y, z) = getIndex(particle->m_pos);
    auto particles = uniformGrid[x][y][z];
    particles.erase(std::remove(particles.begin(), particles.end(), particle), particles.end());

    // Add new
    addParticle(particle);
}

std::tuple<int, int, int> UniformGridNeighborSearch::getIndex(Eigen::Vector3d& position) const {
    int x = int(floor(position(0) / gridWidth));
    int y = int(floor(position(1) / gridWidth));
    int z = int(floor(position(2) / gridWidth));
    return {x, y, z};
}



