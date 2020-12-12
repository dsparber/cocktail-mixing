#include "../include/SphSimulation.h"
#include "../include/Constants.h"
#include "../include/UniformGridNeighborSearch.h"
#include "../include/FluidDefinitons.h"

SphSimulation::SphSimulation() : FluidSimulation() {
    m_dt = 0.01;
    m_gridWidth = 0.2;
    m_kernelRadius = 0.2;
    m_neighborSearch = new UniformGridNeighborSearch(m_gridWidth);
    m_boundary_repulsion = 2000.;
    m_kernels = new SPHKernels(m_kernelRadius);
}


bool SphSimulation::advance() {
    FluidSimulation::advance();
    // Needs to be called to update particle related data
    updateNeighbors();

    updateDensityAndPressure();
    updateForce();
    updateVelocityAndPosition();

	return false;
}

void SphSimulation::resetMembers() {
    FluidSimulation::resetMembers();
    if (m_neighborSearch != NULL) delete m_neighborSearch;
    m_neighborSearch = NULL;
    m_neighborSearch = new UniformGridNeighborSearch(m_gridWidth);
    updateNeighbors();
}

void SphSimulation::updateNeighbors() {
    m_neighborSearch->reset();
    for (auto& fluid : m_fluids) {
        for (auto& particle : fluid->m_particles) {
            m_neighborSearch->addParticle(&particle);
        }
    }
}


void SphSimulation::updateDensityAndPressure() {
    for (auto& fluid : m_fluids) {

        // Define function for parallelism
        auto f = [fluid, this](int start, int end) {

            // Actual computation
            for(int i = start; i < end; ++i) {
                auto& particle = fluid->m_particles.at(i);

                m_neighborSearch->getNeighbors(&particle, m_kernelRadius);

                particle.m_density = 0.0;

                // density
                for(Particle* neighbor : particle.m_neighbors) {
                    double r2 = (particle.m_pos - neighbor->m_pos).squaredNorm();
                    particle.m_density += neighbor->m_mass * m_kernels->wPoly6(r2);
                }

                // pressure
                particle.m_pressure = std::max(0., fluid->m_stiffness * (particle.m_density - fluid->m_restDensity));

            }
        };

        runParallel(fluid->m_particles.size(), f);
    }
}

void SphSimulation::updateForce() {
    for (auto& fluid : m_fluids) {

        // Define function for parallelism
        auto f = [fluid, this](int start, int end) {

            // Actual computation
            for (int i = start; i < end; ++i) {
                auto &particle = fluid->m_particles.at(i);

                Eigen::Vector3d f_pressure = Eigen::Vector3d::Zero();
                Eigen::Vector3d f_viscosity = Eigen::Vector3d::Zero();
                Eigen::Vector3d f_external = Eigen::Vector3d::Zero();
                Eigen::Vector3d f_surface = Eigen::Vector3d::Zero();
                Eigen::Vector3d f_interface = Eigen::Vector3d::Zero();
                Eigen::Vector3d f_boundary = Eigen::Vector3d::Zero();

                double lapColor = 0.0;
                Eigen::Vector3d normalColor = Eigen::Vector3d::Zero();

                // Gravity
                f_external += constants::g * particle.m_density;

                // Helper variables
                auto &m_i = particle.m_mass;
                auto &p_i = particle.m_pressure;
                auto &r_i = particle.m_pos;
                auto &v_i = particle.m_vel;
                auto &rho_i = particle.m_density;

                for (Particle *neighbor : particle.m_neighbors) {

                    // Helper variables
                    auto &m_j = neighbor->m_mass;
                    auto &p_j = neighbor->m_pressure;
                    auto &r_j = neighbor->m_pos;
                    auto &v_j = neighbor->m_vel;
                    auto &rho_j = neighbor->m_density;
                    Eigen::Vector3d r = r_i - r_j;

                    // Boundary repulsion
                    if (neighbor->m_fluid->m_isBoundary) {
                        f_boundary +=
                                m_boundary_repulsion *
                                r.normalized() *
                                m_kernels->wPoly6(r.squaredNorm());
                        continue;
                    }

                    // Pressure
                    f_pressure -= m_j * (p_i + p_j) / (2 * rho_j)
                                  * m_kernels->gwSpiky(r);

                    // Viscosity
                    f_viscosity += m_j * (v_j - v_i) / rho_j
                                   * m_kernels->lwVisc(r.norm());
                    
                    // Surface tension
                    lapColor += m_j * m_kernels->lwPoly6(r.squaredNorm()) / rho_j;
                    normalColor += m_j * m_kernels->gwPoly6(r) / rho_j;
                }

                f_viscosity *= fluid->m_viscosity;

                if(normalColor.norm() >= fluid->m_tension_thres)
                    f_interface = - fluid->m_tension * lapColor * normalColor.normalized();

                // std::cout << f_external.norm() << " " << f_pressure.norm() << " " << f_viscosity.norm() << " " << f_interface.norm() << std::endl;

                Eigen::Vector3d f =
                        f_external +
                        f_pressure +
                        f_viscosity +
                        f_interface;

                particle.m_acc = f / particle.m_density;
            }
        };

        runParallel(fluid->m_particles.size(), f);
    }
}


void SphSimulation::updateVelocityAndPosition() {
    for (auto& fluid : m_fluids) {

        if (fluid->m_isBoundary) {
            continue;
        }

        // Define function for parallelism
        auto f = [fluid, this](int start, int end) {
            for (int i = start; i < end; ++i) {
                auto &particle = fluid->m_particles.at(i);
                particle.m_vel += m_dt * particle.m_acc;

                // enforce CFL condition
                double v_max = 0.4*m_kernelRadius/m_dt;
                particle.m_vel = (particle.m_vel.norm() > v_max) ? particle.m_vel.normalized() * v_max : particle.m_vel;

                Eigen::Vector3d nextPosition = particle.m_pos + m_dt * particle.m_vel;
                if (m_scene != nullptr && m_scene->outOfBoundary(nextPosition)) {
                    m_scene->updateOnBoundaryCollision(particle, m_dt);
                } else {
                    particle.m_pos = nextPosition;
                }
            }
        };
        runParallel(fluid->m_particles.size(), f);
    }
}