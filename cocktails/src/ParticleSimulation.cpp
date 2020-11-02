#include "../include/ParticleSimulation.h"
#include "../include/BruteForceNeighborSearch.h"

using namespace std;
/*
 * Example simulation that changes the colors of a cube.
 */
ParticleSimulation::ParticleSimulation(double gridWidth) : Simulation() {
    m_neighborSearch = new UniformGridNeighborSearch(gridWidth);
    //m_neighborSearch = new BruteForceNeighborSearch();
    m_scene = new Scene(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(4, 4, 4));

}

void ParticleSimulation::initParticles() {
    // Create particles
    int n = 6;
    Eigen::Vector3d start_pos(2,2,2);
    double spacing = 0.1;
    double V = pow(spacing, 3);
    m_neighborSearch->reset();
    m_particles = std::vector<Particle>();
    m_particles.reserve(n * n * n);
    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < n; ++y) {
            for (int z = 0; z < n; ++z) {
                Particle particle;
                particle.m_mass = 1000 * V;
                particle.m_pos << x, y, z;
                particle.m_pos *= spacing;
                particle.m_pos += start_pos;
                particle.m_color << 1. * x / n, 1. * y / n, 1. * z / n;
                m_particles.push_back(particle);
                m_neighborSearch->addParticle(&m_particles.back());
            }
        }
    }
}


void ParticleSimulation::init() {
	reset();
}

void ParticleSimulation::resetMembers() {
    initParticles();
}

bool ParticleSimulation::advance() {
	m_step++;
	return false;
}

void ParticleSimulation::postAdvance() {
    m_neighborSearch->reset();
    for (auto& particle : m_particles) {
        m_neighborSearch->addParticle(&particle);
    }
}

void ParticleSimulation::updateRenderGeometry() {
    V.resize(m_particles.size(), 3);
    C.resize(m_particles.size(), 3);

    for(int i = 0; i < m_particles.size(); i++) {
        V.row(i) << m_particles[i].m_pos.transpose();
        C.row(i) << m_particles[i].m_color.transpose();
    }
}

void ParticleSimulation::renderRenderGeometry(igl::opengl::glfw::Viewer &viewer) {
    //viewer.data().set_mesh(V, F);
    // viewer.data().set_colors(C);
    viewer.data().point_size = 10;
    viewer.data().set_points(V, C);
    m_scene->draw(viewer);
}


