#include "../include/UniformGridNeighborSearch.h"
#include <algorithm>

UniformGridNeighborSearch::UniformGridNeighborSearch(double gridWidth) : gridWidth(gridWidth) {}

std::vector<Particle*> UniformGridNeighborSearch::getNeighbors(Particle* particle, double radius) const {
    int x, y, z;
    std::tie(x, y, z) = getIndex(particle->m_pos);

    double epsilon = gridWidth / 10000;

    int delta = int(ceil(radius / gridWidth));
    std::vector<Particle*> neighbors;
    for (int dx = -delta; dx <= delta; ++dx) {
        auto gridX = uniformGrid.find(x + dx);
        if (gridX == uniformGrid.end()) {
            continue;
        }
        for (int dy = -delta; dy <= delta; ++dy) {
            auto gridXY = gridX->second.find(y + dy);
            if (gridXY == gridX->second.end()) {
                continue;
            }
            for (int dz = -delta; dz <= delta; ++dz) {
                auto gridXYZ = gridXY->second.find(z + dz);
                if (gridXYZ == gridXY->second.end()) {
                    continue;
                }
                for (auto neighbor : gridXYZ->second) {
                    auto distance = (neighbor->m_pos - particle->m_pos).norm();
                    if (distance <= radius + epsilon) {
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
}

std::tuple<int, int, int> UniformGridNeighborSearch::getIndex(Eigen::Vector3d& position) const {
    int x = int(floor(position(0) / gridWidth));
    int y = int(floor(position(1) / gridWidth));
    int z = int(floor(position(2) / gridWidth));
    return {x, y, z};
}

void UniformGridNeighborSearch::reset() {
    uniformGrid.clear();
}



