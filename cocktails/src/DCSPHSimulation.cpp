#include "../include/Kernels.h"
#include "../include/DCSPHSimulation.h"
#include "../include/Constants.h"
#include "../include/UniformGridNeighborSearch.h"
#include <iostream>

DCSPHSimulation::DCSPHSimulation() : SphSimulation() {}

void DCSPHSimulation::updateDensityAndPressure() {
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
                    particle.m_density += kernels::wPoly6(r2, m_kernelRadius); // particle density
                }

                // pressure
                particle.m_pressure = std::max(0., fluid->m_stiffness * (particle.m_mass * particle.m_density - fluid->m_restDensity));
            }
        };

        runParallel(fluid->m_particles.size(), f);
    }
}

void DCSPHSimulation::updateForce() {
    for (auto& fluid : m_fluids) {

        // Define function for parallelism
        auto f = [fluid, this](int start, int end) {

            // Actual computation
            for (int i = start; i < end; ++i) {
                auto &particle = fluid->m_particles.at(i);

                Eigen::Vector3d f_pressure = Eigen::Vector3d::Zero();
                Eigen::Vector3d f_viscosity = Eigen::Vector3d::Zero();
                Eigen::Vector3d f_external = Eigen::Vector3d::Zero();
                Eigen::Vector3d f_boundary = Eigen::Vector3d::Zero();
                Eigen::Vector3d f_interface = Eigen::Vector3d::Zero();

                // Gravity
                f_external += constants::g * particle.m_mass;

                // Helper variables
                auto &p_i = particle.m_pressure;
                auto &r_i = particle.m_pos;
                auto &v_i = particle.m_vel;
                auto &rho_i = particle.m_density;
                auto &mu_i = fluid->m_viscosity;

                for (Particle *neighbor : particle.m_neighbors) {

                    // Helper variables
                    auto &p_j = neighbor->m_pressure;
                    auto &r_j = neighbor->m_pos;
                    auto &v_j = neighbor->m_vel;
                    auto &rho_j = neighbor->m_density;
                    auto &mu_j = neighbor->m_fluid->m_viscosity;

                    // Pressure
                    f_pressure -= (p_i + p_j) / (2 * rho_j)
                                  * kernels::gradWSpiky(r_i - r_j, m_kernelRadius);

                    // Viscosity
                    f_viscosity += (mu_i + mu_j) * (v_j - v_i) / (2 * rho_j)
                                   * kernels::lapWViscosity((r_i - r_j).norm(), m_kernelRadius);

                    // Boundary repulsion
                    double K = .4;
                    if (neighbor->m_fluid->m_isBoundary) {
                        f_boundary +=
                                K *
                                (r_j - r_i).normalized() *
                                kernels::wPoly6((r_i - r_j).norm(), m_kernelRadius);
                    }
                }

                f_pressure /= rho_i;

                Eigen::Vector3d f =
                        f_external +
                        f_pressure +
                        f_viscosity +
                        f_boundary;

                particle.m_acc = f / (particle.m_mass * particle.m_density);
            }
        };

        runParallel(fluid->m_particles.size(), f);
    }
}