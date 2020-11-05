#include "../include/Fluid.h"

void Fluid::init() {

    auto& d = m_initialDimension;
    m_particles = std::vector<Particle>();
    m_particles.reserve(d.x() * d.y() * d.z());

    for (int x = 0; x < d.x(); ++x) {
        for (int y = 0; y < d.y(); ++y) {
            for (int z = 0; z < d.z(); ++z) {
                Particle particle;
                particle.m_mass = m_particleMass;
                particle.m_pos << x, y, z;
                particle.m_pos *= m_initialSpacing;
                particle.m_pos += m_initialOffset;
                particle.m_color << 1. * x / d.x(), 1. * y / d.y(), 1. * z / d.z();
                m_particles.push_back(particle);
            }
        }
    }
}
