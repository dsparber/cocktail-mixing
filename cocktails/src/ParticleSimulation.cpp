#include "../include/ParticleSimulation.h"

using namespace std;
/*
 * Example simulation that changes the colors of a cube.
 */
ParticleSimulation::ParticleSimulation(double gridWidth) : Simulation() {
	m_neighborSearch = new UniformGridNeighborSearch(gridWidth);
    m_box = new Box(Eigen::Vector3d(0,0,0), Eigen::Vector3d(4,4,4));

}

void ParticleSimulation::init() {

	// Create particles
	int n = 4;
    Eigen::Vector3d start_pos(2,2,2);
    double spacing = 0.1;
    double V = pow(spacing, 3);
	m_particles = std::vector<Particle>();
	m_particles.reserve(n * n * n);
	for (int x = 0; x < n; ++x) {
        for (int y = 0; y < n; ++y) {
            for (int z = 0; z < n; ++z) {
                Particle particle;
                particle.m_mass = 1000.0 * V;
                particle.m_pos << x, y, z;
                particle.m_pos *= spacing;
                particle.m_pos += start_pos;
                particle.m_color << 1. * x / n, 1. * y / n, 1. * z / n;
                m_particles.push_back(particle);
                m_neighborSearch->addParticle(&m_particles.back());
            }
        }
	}
	reset();
}

void ParticleSimulation::resetMembers() {

}

bool ParticleSimulation::advance() {
	m_step++;
	return false;
}

void ParticleSimulation::postAdvance() {
    for (auto particle : m_particles) {
        m_neighborSearch->updateParticle(&particle);
    }
}

void ParticleSimulation::updateRenderGeometry() {
    // creates small spheres to visualize particles

/*     using namespace Eigen;

    auto res = 8; // the resolution of the sphere discretization
    auto radius = .2;
    int numParticles = m_particles.size();

    V.resize(res * res * numParticles, 3);
    F.resize(2 * (res - 1) * res * numParticles, 3);
    C.resize(F.rows(), 3);

    for (int i = 0; i < numParticles; i++) {
        RowVector3d center = m_particles[i].m_pos.transpose();

        //creating vertices
        for (int j = 0; j < res; j++) {
            double z = center(2) + radius * cos(M_PI * (double) j / (double(res - 1)));
            for (int k = 0; k < res; k++) {
                double x = center(0) + radius * sin(M_PI * (double) j / (double(res - 1))) * cos(2 * M_PI * (double) k / (double(res - 1)));
                double y = center(1) + radius * sin(M_PI * (double) j / (double(res - 1))) * sin(2 * M_PI * (double) k / (double(res - 1)));
                V.row((res * res) * i + j * res + k) << x, y, z;
            }
        }

        //creating faces
        for (int j = 0; j < res - 1; j++) {
            for (int k = 0; k < res; k++) {
                int v1 = (res * res) * i + j * res + k;
                int v2 = (res * res) * i + (j + 1) * res + k;
                int v3 = (res * res) * i + (j + 1) * res + (k + 1) % res;
                int v4 = (res * res) * i + j * res + (k + 1) % res;
                F.row(2 * (((res - 1) * res) * i + res * j + k)) << v1, v2, v3;
                F.row(2 * (((res - 1) * res) * i + res * j + k) + 1) << v4, v1, v3;
                C.row(2 * (((res - 1) * res) * i + res * j + k)) = m_particles[i].m_color.transpose();
                C.row(2 * (((res - 1) * res) * i + res * j + k) + 1) = m_particles[i].m_color.transpose();
            }
        }
    } */

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
    m_box->draw(viewer);
}
