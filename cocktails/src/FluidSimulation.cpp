#include "../include/FluidSimulation.h"
#include "../include/UniformGridNeighborSearch.h"
#include "../include/BoxScene.h"
#include "../include/FluidDefinitons.h"
#include <thread>

using namespace std;

FluidSimulation::FluidSimulation() : Simulation() {
    m_fluids = fluids::all;
    m_scene = new BoxScene(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(4, 4, 4));
}

void FluidSimulation::init() {
    reset();
}

void FluidSimulation::resetMembers() {
    for (auto &fluid : m_fluids) {
        fluid->reset();
    }
    for (auto &source : m_sources) {
        source->init();
    }
}

bool FluidSimulation::advance() {
    m_time += m_dt;
    for (auto &source : m_sources) {
        source->advance(m_time);
    }
    m_step++;
    return false;
}

void FluidSimulation::updateRenderGeometry() {
    int numParticles = 0;
    for (auto &fluid : m_fluids) {
        numParticles += fluid->m_particles.size();
    }

    V.resize(numParticles, 3);
    C.resize(numParticles, 3);

    int i = 0;
    if(m_use_particle_color){
        for (auto &fluid : m_fluids) {
            for (auto &particle : fluid->m_particles) {
                V.row(i) << particle.m_pos.transpose();
                C.row(i) << particle.m_color.transpose();
                ++i;
            }
        }
    }else{
        // use fluid color
        for (auto &fluid : m_fluids) {
            for (auto &particle : fluid->m_particles) {
                V.row(i) << particle.m_pos.transpose();
                C.row(i) << fluid->m_color.transpose();
                ++i;
            }
        }
    }
}

void FluidSimulation::renderRenderGeometry(igl::opengl::glfw::Viewer &viewer) {
    viewer.data().point_size = 10;
    viewer.data().set_points(V, C);
    m_scene->draw(viewer);
}

void FluidSimulation::runParallel(int elementCount, const std::function<void(int, int)> &f) {
    int numThreads = std::max(4, (int) std::thread::hardware_concurrency());
    int elementsPerThread = ceil(1. * elementCount / numThreads);
    std::vector<std::thread> threads(numThreads);

    if (elementCount == 0) {
        return;
    }

    for (int i = 0; i < numThreads; ++i) {
        int start = std::min(elementCount - 1, i * elementsPerThread);
        int end = std::min(elementCount, (i + 1) * elementsPerThread);
        threads[i] = std::thread(f, start, end);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}
