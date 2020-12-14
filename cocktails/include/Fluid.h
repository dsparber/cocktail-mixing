#ifndef PBS_FLUID_H
#define PBS_FLUID_H

#include <vector>
#include "Particle.h"

/// Holds all fluid properties and contains all particles for this fluid
class Fluid {

public:
    std::vector<Particle> m_particles;

    std::string m_name;

    double m_viscosity;
    double m_stiffness;
    double m_restDensity;
    double m_particleMass;
    bool m_isBoundary;
    double m_tension;
    double m_tension_thres;

    Eigen::Vector3d m_color;

    Fluid(std::string name, double viscosity, double stiffness, double restDensity,
          double particleMass, Eigen::Vector3d color, bool isBoundary, double tension = 0.1, double m_tension_thres = 1e-6);

    void reset();
};


#endif // PBS_FLUID_H
