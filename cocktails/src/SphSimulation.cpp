#include "../include/Kernels.h"
#include "../include/SphSimulation.h"
#include "../include/Constants.h"
#include "../include/UniformGridNeighborSearch.h"

SphSimulation::SphSimulation() : FluidSimulation() {
    m_dt = 0.008;
    m_gridWidth = 0.2;
    m_kernelRadius = 0.2;
    m_neighborSearch = new UniformGridNeighborSearch(m_gridWidth);
}


bool SphSimulation::advance() {
    FluidSimulation::advance();

    updateDensityAndPressure();
    updateForce();
    updateVelocityAndPosition();

    // Needs to be called to update particle related data
    updateNeighbors();
	return false;
}

void SphSimulation::resetMembers() {
    FluidSimulation::resetMembers();
    delete m_neighborSearch;
    m_neighborSearch = new UniformGridNeighborSearch(m_gridWidth);
    updateNeighbors();
}

void SphSimulation::updateNeighbors() {
    m_neighborSearch->reset();
    for (auto& fluid : m_fluids) {
        for (auto& particle : fluid.m_particles) {
            m_neighborSearch->addParticle(&particle);
        }
    }
}

void SphSimulation::updateDensityAndPressure() {
    for (auto& fluid : m_fluids) {
        for(auto& particle : fluid.m_particles) {

            particle.m_density = 0.0;

            std::vector<Particle*> neighborhood = m_neighborSearch->getNeighbors(&particle, m_kernelRadius);

            // density
            for(Particle* neighbor : neighborhood) {
                double r2 = (particle.m_pos - neighbor->m_pos).squaredNorm();
                particle.m_density += neighbor->m_mass * kernels::wPoly6(r2, m_kernelRadius);
            }

            // pressure
            particle.m_pressure = std::max(0., fluid.m_stiffness * (particle.m_density - fluid.m_restDensity));
        }
    }

}


void SphSimulation::updateForce() {
    for (auto& fluid : m_fluids) {
        for (auto &particle : fluid.m_particles) {

            std::vector<Particle *> neighborhood = m_neighborSearch->getNeighbors(&particle, m_kernelRadius);

            // TODO: implement surface tension
            Eigen::Vector3d f_pressure = Eigen::Vector3d::Zero();
            Eigen::Vector3d f_viscosity = Eigen::Vector3d::Zero();
            Eigen::Vector3d f_external = Eigen::Vector3d::Zero();

            // Gravity
            f_external += constants::g * particle.m_mass;

            // Helper variables
            auto& p_i = particle.m_pressure;
            auto& r_i = particle.m_pos;
            auto& v_i = particle.m_vel;

            for (Particle *neighbor : neighborhood) {

                // Helper variables
                auto& m_j = neighbor->m_mass;
                auto& p_j = neighbor->m_pressure;
                auto& r_j = neighbor->m_pos;
                auto& v_j = neighbor->m_vel;
                auto& rho_j = neighbor->m_density;

                // Pressure
                f_pressure -= m_j * (p_i + p_j) / (2 * rho_j)
                        * kernels::gradWSpiky(r_i - r_j, m_kernelRadius);

                // Viscosity
                f_viscosity += fluid.m_viscosity * m_j * (v_j - v_i) / rho_j
                        * kernels::lapWViscosity((r_i - r_j).norm(), m_kernelRadius);
            }

            Eigen::Vector3d f =
                    f_external +
                    f_pressure +
                    f_viscosity;

            particle.m_acc = f / particle.m_density;
        }
    }
}


void SphSimulation::updateVelocityAndPosition() {
    for (auto& fluid : m_fluids) {
        for(auto& particle : fluid.m_particles) {
            particle.m_vel += m_dt * particle.m_acc;
            Eigen::Vector3d nextPosition = particle.m_pos + m_dt * particle.m_vel;
            if (m_scene->outOfBoundary(nextPosition)) {
                // move into box
                // reflect velocity
                particle.m_vel *= -1;
            } else {
                particle.m_pos = nextPosition;
            }
        }
    }

    return;
    // DEBUG Stuff
    double maxPressure = 0;
    for (auto& fluid : m_fluids) {
        for(auto& particle : fluid.m_particles) {
            maxPressure = std::max(maxPressure, particle.m_pressure);
        }
    }
    for (auto& fluid : m_fluids) {
        for(auto& particle : fluid.m_particles) {
            particle.m_color << abs(particle.m_pressure) / maxPressure, 0, 0;
        }
    }
}

void SphSimulation::setKernelRadius(double kernelRadius) {
    assert(kernelRadius >= 0);
    m_kernelRadius = kernelRadius;
}

void SphSimulation::setGridWidth(double gridWidth) {
    assert(gridWidth >= 0);
    m_gridWidth = gridWidth;
}