#ifndef PBS_FLUID_H
#define PBS_FLUID_H

#include <vector>
#include "Particle.h"

class Fluid {

public:
    std::vector<Particle> m_particles;

    std::string m_name;

    double m_V;
    double m_viscosity;
    double m_stiffness;
    double m_restDensity;
    double m_particleMass;

    Fluid(std::string name, double V, double viscosity, double stiffness, double restDensity, double particleMass);

    void reset();
};


#endif // PBS_FLUID_H
