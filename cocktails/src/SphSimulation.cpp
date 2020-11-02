#include "../include/Kernels.h"
#include "../include/SphSimulation.h"
#include "../include/Constants.h"


SphSimulation::SphSimulation() : FluidSimulation(m_gridWidth) {
    m_dt = 0.1;
}


bool SphSimulation::advance() {
    FluidSimulation::advance();

    updateDensityAndPressure();
    updateForce();
    updateVelocityAndPosition();

    // Needs to be called to update particle related data
    FluidSimulation::updateNeighbors();
	return false;
}

void SphSimulation::updateDensityAndPressure() {
    for (auto& fluid : m_fluids) {
        for(auto& particle : fluid.m_particles) {

            particle.m_density = 0.0;

            std::vector<Particle*> neighborhood = m_neighborSearch->getNeighbors(&particle, m_kernelRadius);

            // density
            for(Particle* neighbor : neighborhood) {
                Eigen::Vector3d diff = particle.m_pos - neighbor->m_pos;
                double r2 = diff.dot(diff);
                double delta = neighbor->m_mass * kernels::Wpoly6(r2, m_kernelRadius);
                particle.m_density += delta;
            }

            // pressure
            particle.m_pressure = fluid.m_stiffness * (particle.m_density - fluid.m_initialDensity);
        }
    }

}


void SphSimulation::updateForce() {
    for (auto& fluid : m_fluids) {
        for (auto &particle : fluid.m_particles) {

            std::vector<Particle *> neighborhood = m_neighborSearch->getNeighbors(&particle, m_kernelRadius);

            // TODO: implement surface tension
            Eigen::Vector3d f_pressure, f_viscosity, f_external;
            f_pressure.setZero();
            f_viscosity.setZero();
            f_external.setZero();

            for (Particle *neighbor : neighborhood) {
                Eigen::Vector3d r = particle.m_pos - neighbor->m_pos;
                double pressure_coeff =
                        neighbor->m_mass * (neighbor->m_pressure + particle.m_pressure) / (2.0 * neighbor->m_density);
                double viscosity_coeff = neighbor->m_mass * kernels::lapWViscosity(r.squaredNorm(), m_kernelRadius) /
                                         neighbor->m_density;
                f_pressure += pressure_coeff * kernels::gradWspiky(r, m_kernelRadius);
                f_viscosity += viscosity_coeff * (particle.m_vel - neighbor->m_vel);
            }

            f_pressure *= -particle.m_mass / particle.m_density;
            f_viscosity *= -particle.m_mass * fluid.m_viscosity;
            f_external = constants::g * particle.m_mass; // gravity
            particle.m_acc = (f_pressure + f_viscosity + f_external) / particle.m_density;
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
}