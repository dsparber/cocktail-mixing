#include "../include/Fluid.h"

#include <utility>

void Fluid::reset() {
    m_particles = std::vector<Particle>();
}

Fluid::Fluid(std::string name, double V, double viscosity, double stiffness, double restDensity, double particleMass) :
        m_name(std::move(name)),
        m_V(V),
        m_viscosity(viscosity),
        m_stiffness(stiffness),
        m_restDensity(restDensity),
        m_particleMass(particleMass) {}
