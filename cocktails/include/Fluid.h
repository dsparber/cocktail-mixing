#ifndef PBS_FLUID_H
#define PBS_FLUID_H

#include <vector>
#include "Particle.h"

class Fluid {

public:
    std::vector<Particle> m_particles;

    double m_V;
    double m_viscosity;
    double m_stiffness;
    double m_particleMass;
    double m_initialDensity;

    double m_initialSpacing;
    Eigen::Vector3i m_initialDimension;
    Eigen::Vector3d m_initialOffset;

    void init();
};


#endif // PBS_FLUID_H
