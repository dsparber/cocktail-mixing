#include "../include/DCSPHSimulation.h"
#include "../include/Constants.h"
#include "../include/UniformGridNeighborSearch.h"
#include <iostream>

DCSPHSimulation::DCSPHSimulation() : SphSimulation() {}

void DCSPHSimulation::updateDensityAndPressure() {
    for (auto& fluid : m_fluids) {
        if(fluid->m_isBoundary) continue;
        // Define function for parallelism
        auto f = [fluid, this](int start, int end) {

            // Actual computation
            for(int i = start; i < end; ++i) {
                auto& particle = fluid->m_particles.at(i);

                m_neighborSearch->getNeighbors(&particle, m_kernelRadius);

                particle.m_density = 0.0;

                // Particle density!
                for(Particle* neighbor : particle.m_neighbors) {
                    if(neighbor->m_fluid->m_isBoundary) continue;
                    double r2 = (particle.m_pos - neighbor->m_pos).squaredNorm();
                    particle.m_density += m_kernels->wPoly6(r2); // particle density
                }

                // pressure
                // particle.m_pressure = std::max(0., fluid->m_stiffness * (particle.m_mass * particle.m_density - fluid->m_restDensity));

                // Pressure using Tait equation (11)
                particle.m_pressure = std::max(0., constants::taitEq(fluid->m_stiffness, fluid->m_restDensity, particle.m_mass * particle.m_density));

            }
        };

        runParallel(fluid->m_particles.size(), f);
    }
}

void DCSPHSimulation::updateForce() {

    // update colorfield

    for (auto& fluid : m_fluids) {

        // No force update necessairy for boundary particles since no advection
        if(fluid->m_isBoundary) continue;
        
        // Define function for parallelism
        auto f = [fluid, this](int start, int end) {

            // Actual computation
            for (int i = start; i < end; ++i) {

                auto &particle = fluid->m_particles.at(i);

                Eigen::Vector3d f_external = Eigen::Vector3d::Zero();
                Eigen::Vector3d f_pressure = Eigen::Vector3d::Zero();
                Eigen::Vector3d f_viscosity = Eigen::Vector3d::Zero();
                Eigen::Vector3d f_boundary = Eigen::Vector3d::Zero();
                Eigen::Vector3d f_interface = Eigen::Vector3d::Zero();

                double lapColor = 0;
                Eigen::Vector3d normalColor = Eigen::Vector3d::Zero();

                // Helper variables
                auto &m_i = particle.m_mass;
                auto &p_i = particle.m_pressure;
                auto &r_i = particle.m_pos;
                auto &v_i = particle.m_vel;
                auto &d_i = particle.m_density;
                auto &mu_i = fluid->m_viscosity;

                // Gravitational force
                f_external = constants::g * m_i;

                for (Particle *neighbor : particle.m_neighbors) {

                    // Helper variables
                    auto &m_j = neighbor->m_mass;
                    auto &p_j = neighbor->m_pressure;
                    auto &r_j = neighbor->m_pos;
                    auto &v_j = neighbor->m_vel;
                    auto &d_j = neighbor->m_density;
                    auto &mu_j = neighbor->m_fluid->m_viscosity;
                    Eigen::Vector3d r_ij = r_i - r_j;
                    double r = r_ij.norm();

                    // Boundary repulsion
                    if (neighbor->m_fluid->m_isBoundary) {
                        f_boundary +=
                                m_boundary_repulsion *
                                r_ij.normalized() *
                                m_kernels->wPoly6(r*r);
                        continue;
                    }

                    // Pressure
                    //f_pressure -= (p_i + p_j) / (2 * d_j)
                    //                * m_kernels->gwSpiky(r_ij);

                    f_pressure -= ((p_i/(d_i*d_i)) + (p_j/(d_j * d_j))) * m_kernels->gwSpiky(r_ij);
                 
                    // Viscosity
                    f_viscosity += (mu_i + mu_j) * (v_j - v_i) / (2 * d_j)
                                   * m_kernels->lwVisc(r);

                    // Interface Tension Forces as in Mueller 03
                    lapColor += m_j * m_kernels->lwPoly6(r*r) / d_j;
                    normalColor += m_j * m_kernels->gwPoly6(r_ij) / d_j;
                }

                f_viscosity /= d_i;
                
                f_boundary /= d_i;

                if(normalColor.norm() > fluid->m_tension_thres)
                    f_interface -= fluid->m_tension * lapColor * normalColor.normalized() / d_i;

                // DEBUGGING
                // if(f_boundary.norm() > 1e-6)
                //     std::cout << f_external.norm() << " " << f_pressure.norm() << " " << f_viscosity.norm() << " " << f_interface.norm() << " " << f_boundary.norm() << std::endl;

                Eigen::Vector3d f = f_external
                                    + f_pressure
                                    + f_viscosity
                                    + f_boundary
                                    + f_interface;

                particle.m_acc = f / m_i;
            }
        };

        runParallel(fluid->m_particles.size(), f);
    }
}