#include "../include/SimulationLoader.h"
#include "../include/BoxScene.h"
#include "../include/DCSPHSimulation.h"
#include "../include/SphSimulation.h"
#include "../include/Fluid.h"
#include "../include/FluidDefinitons.h"
#include "../include/StateSource.h"

#include <fstream>

using namespace std;
// static
void SimulationLoader::saveSimulation(std::string exportPath, SphSimulation* sim) {
    ofstream exportFile;
    exportFile.open(exportPath, ios::out | ios::trunc);
    // save SPH parameter
    auto dcsph_cast = dynamic_cast<DCSPHSimulation*>(sim);
    exportFile << ((dcsph_cast == nullptr) ? "sph\n" : "dcsph\n");
    exportFile << std::to_string(sim->m_dt) << " " << std::to_string(sim->m_gridWidth) << " " << std::to_string(sim->m_kernelRadius) << "\n";

    // save scene information
    auto box_cast = dynamic_cast<BoxScene*>(sim->m_scene);
    if(box_cast != nullptr) {
        Eigen::Vector3d m, M;
        sim->m_scene->getMinMax(m, M);
        exportFile << "box " << m.transpose() << " " << M.transpose() << "\n";
    } else {
        exportFile << "no_scene\n";
    }

    exportFile << sim->m_fluids.size() << "\n";
    // TODO: save particles
    for(auto &fluid : sim->m_fluids) {
        // save fluid information
        exportFile << fluid->m_name << " " << fluid->m_particles.size() << "\n";
        // save particle pos and vel
        for(auto &particle : fluid->m_particles) {
            exportFile << particle.m_pos.transpose() << " " << particle.m_vel.transpose() << "\n";
        }
    }

    exportFile.close();
}

// static
SphSimulation* SimulationLoader::loadSimulation(std::string loadPath) {
    SphSimulation* temp = nullptr;
    ifstream loadFile(loadPath);
    // set SPH parameter
    std::string reader;
    loadFile >> reader;

    if(reader == "sph") {
        temp = new SphSimulation();
    } else {
        temp = new DCSPHSimulation();
    }

    // clear all members
    temp->resetMembers();

    loadFile >> temp->m_dt;
    loadFile >> temp->m_gridWidth;
    loadFile >> temp->m_kernelRadius;


    // set scene information
    loadFile >> reader;

    if(reader == "box") {
        Eigen::Vector3d m, M;
        double x,y,z;

        loadFile >> x >> y >> z;
        m = Eigen::Vector3d(x,y,z);

        loadFile >> x >> y >> z;
        M = Eigen::Vector3d(x,y,z);

        temp->m_scene = new BoxScene(m, M);
    }

    // restore particles
    int num_fluids; loadFile >> num_fluids;
    string fluid_name;
    int num_particles;
    for(int i = 0; i < num_fluids; i++) {
        loadFile >> fluid_name >> num_particles;
        Fluid* fluid = nullptr;

        for(auto temp_fluid : temp->m_fluids) {
            if(fluid_name == temp_fluid->m_name) {
                fluid = temp_fluid;
                break;
            }
        }

        if(fluid == nullptr) {
            cout << "Unknown fluid type\n";
            break;
        }

        std::vector<Particle> particles;
        for(int j = 0; j < num_particles; j++) {
            double x,y,z;
            Particle particle;
            particle.m_mass = fluid->m_particleMass;
            loadFile >> x >> y >> z;
            particle.m_pos = Eigen::Vector3d(x,y,z);
            loadFile >> x >> y >> z;
            particle.m_vel = Eigen::Vector3d(x,y,z);
            particle.m_fluid = fluid;
            particles.push_back(particle);
        }

        temp->m_sources.push_back(new StateSource(fluid, particles));
    }

    loadFile.close();

    return temp;
}