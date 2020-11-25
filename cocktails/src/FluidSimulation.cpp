#include "../include/FluidSimulation.h"
#include "../include/FluidDefinitons.h"
#include <thread>

using namespace std;

FluidSimulation::FluidSimulation() : Simulation() {
    m_fluids = fluids::all;
    m_sources = vector<Source*>();
    m_use_particle_color = false;
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

    V.setZero(numParticles, 3);
    C.setZero(numParticles, 3);

    int i = 0;
    if(m_use_particle_color){
        for (auto &fluid : m_fluids) {
            for (auto &particle : fluid->m_particles) {
                V.row(i) = particle.m_pos.transpose();
                C.row(i) = particle.m_color.transpose();
                ++i;
            }
        }
    }else{
        // use fluid color
        for (auto &fluid : m_fluids) {
            for (auto &particle : fluid->m_particles) {
                V.row(i) = particle.m_pos.transpose();
                C.row(i) = fluid->m_color.transpose();
                ++i;
            }
        }
    }
}

void FluidSimulation::renderRenderGeometry(igl::opengl::glfw::Viewer &viewer) {
    viewer.data().point_size = 10;
    viewer.data().set_points(V, C);
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


void FluidSimulation::getMinMaxParticlePosition(Eigen::Vector3d& minPosition, Eigen::Vector3d& maxPosition) {
    minPosition = Eigen::Vector3d(INFINITY, INFINITY, INFINITY);
    maxPosition = Eigen::Vector3d(-INFINITY, -INFINITY, -INFINITY);
    for (auto &fluid : m_fluids) {
        for (auto &particle : fluid->m_particles) {
            for(int i = 0; i < 3; i++) {
                minPosition[i] = std::min(minPosition[i], particle.m_pos[i]);
                maxPosition[i] = std::max(maxPosition[i], particle.m_pos[i]);
            }
        }
    }
}
