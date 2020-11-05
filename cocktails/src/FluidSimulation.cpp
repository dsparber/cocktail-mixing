#include "../include/FluidSimulation.h"
#include "../include/UniformGridNeighborSearch.h"
#include "../include/FluidDefinitons.h"

using namespace std;

FluidSimulation::FluidSimulation() : Simulation() {
    m_scene = new Scene(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(4, 4, 4));
    m_fluids.push_back(fluids::waterBlock());
}

void FluidSimulation::initFluids() {
    for (auto& fluid : m_fluids) {
        fluid.init();
    }
}


void FluidSimulation::init() {
	reset();
}

void FluidSimulation::resetMembers() {
    initFluids();
}

bool FluidSimulation::advance() {
	m_step++;
	return false;
}

void FluidSimulation::updateRenderGeometry() {
    int numParticles = 0;
    for (auto& fluid : m_fluids) {
        numParticles += fluid.m_particles.size();
    }

    V.resize(numParticles, 3);
    C.resize(numParticles, 3);

    int i = 0;
    for (auto& fluid : m_fluids) {
        for (auto& particle : fluid.m_particles) {
            V.row(i) << particle.m_pos.transpose();
            C.row(i) << particle.m_color.transpose();
            ++i;
        }
    }
}

void FluidSimulation::renderRenderGeometry(igl::opengl::glfw::Viewer &viewer) {
    viewer.data().point_size = 10;
    viewer.data().set_points(V, C);
    m_scene->draw(viewer);
}

void FluidSimulation::updateFluids(const std::vector<Fluid>& fluids) {
    m_fluids.clear();
    for(auto& fluid : fluids) {
        m_fluids.push_back(fluid);
    }
}