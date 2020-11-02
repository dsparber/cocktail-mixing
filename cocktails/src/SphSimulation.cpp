#include "../include/Kernels.h"
#include "../include/SphSimulation.h"

SphSimulation::SphSimulation() : ParticleSimulation(m_gridWidth) {
    m_stiffness = 1000.0;
    m_density_0 = 1000.0;
    m_viscosity = 1.0;
    m_dt = 0.1;
 }

void SphSimulation::init() {
	ParticleSimulation::init();

    // SphSimulation specific init

	reset();
}

void SphSimulation::resetMembers() {
    ParticleSimulation::resetMembers();

    // SphSimulation specific resetMembers
}

bool SphSimulation::advance() {
    ParticleSimulation::advance();

    updateDensityAndPressure();
    updateForce();
    updateVelocityAndPosition();

    // Needs to be called to update particle related data
    ParticleSimulation::postAdvance();
	return false;
}

void SphSimulation::updateDensityAndPressure() {
    for(auto& particle : m_particles) {

        // TODO: Problem if particle has no neighbors, than density is zero and later division by zero
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
        particle.m_pressure = m_stiffness * (particle.m_density - m_density_0);

    }
}


void SphSimulation::updateForce() {
    for(auto& particle : m_particles) {

        std::vector<Particle*> neighborhood = m_neighborSearch->getNeighbors(&particle, m_kernelRadius);

        // TODO: implement surface tension
        Eigen::Vector3d f_pressure, f_viscosity, f_external;
        f_pressure.setZero();
        f_viscosity.setZero();
        f_external.setZero();

        for(Particle* neighbor : neighborhood) {
            Eigen::Vector3d r = particle.m_pos - neighbor->m_pos;
            double pressure_coeff = neighbor->m_mass * (neighbor->m_pressure + particle.m_pressure) / (2.0*neighbor->m_density);
            double viscosity_coeff = neighbor->m_mass * kernels::lapWViscosity(r.squaredNorm(), m_kernelRadius) / neighbor->m_density;
            f_pressure += pressure_coeff * kernels::gradWspiky(r, m_kernelRadius);
            f_viscosity += viscosity_coeff * (particle.m_vel - neighbor->m_vel);
        }

        f_pressure *= - particle.m_mass / particle.m_density;
        f_viscosity *= - particle.m_mass * m_viscosity;
        f_external = Eigen::Vector3d(0.0, -1.0, 0.0) * particle.m_mass; // gravity
        particle.m_acc = (f_pressure + f_viscosity + f_external) / particle.m_density;

    }
    
}


void SphSimulation::updateVelocityAndPosition() {
    for(auto& particle : m_particles) {
        particle.m_vel += m_dt * particle.m_acc;
        Eigen::Vector3d nextPosition = particle.m_pos + m_dt * particle.m_vel;
        if(m_scene->outOfBoundary(particle.m_pos)){
            // move into box
            // reflect velocity
            particle.m_vel *= -1;
        }else {
            particle.m_pos = nextPosition;
        }
    }
}