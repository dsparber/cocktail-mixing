#ifndef PBS_PARTICLE_H
#define PBS_PARTICLE_H

#include <Eigen/Dense>

class Particle {

public:
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;
    Eigen::Vector3d color;
};


#endif //PBS_PARTICLE_H
