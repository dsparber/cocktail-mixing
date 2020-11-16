#include "../include/BlockSource.h"

BlockSource::BlockSource(Fluid *fluid) : Source(fluid) {
    m_initialDimension << 10, 10, 10;
    m_initialSpacing = .1;
    m_initialOffset << 1.5, 2.5, 1.5;
}

void BlockSource::init() {
    auto& d = m_initialDimension;

    for (int x = 0; x < d.x(); ++x) {
        for (int y = 0; y < d.y(); ++y) {
            for (int z = 0; z < d.z(); ++z) {
                Particle particle;
                particle.m_mass = m_fluid->m_particleMass;
                particle.m_pos << x, y, z;
                particle.m_pos *= m_initialSpacing;
                particle.m_pos += m_initialOffset;
                particle.m_color << 1. * x / d.x(), 1. * y / d.y(), 1. * z / d.z();
                particle.m_fluid = m_fluid;
                m_fluid->m_particles.push_back(particle);
            }
        }
    }
}

std::string BlockSource::toString() {
    return "Block Source (" + m_fluid->m_name +")";
}
