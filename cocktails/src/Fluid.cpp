#include "../include/Fluid.h"

#include <utility>

void Fluid::reset() {
    m_particles = std::vector<Particle>();
}

Fluid::Fluid(std::string name, double viscosity, double stiffness, double restDensity, double particleMass, Eigen::Vector3d color) :
        m_name(std::move(name)),
        m_viscosity(viscosity),
        m_stiffness(stiffness),
        m_restDensity(restDensity),
        m_particleMass(particleMass),
        m_color(color) {}
