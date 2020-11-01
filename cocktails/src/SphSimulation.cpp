#include "../include/SphSimulation.h"

SphSimulation::SphSimulation() : ParticleSimulation(kernelRadius) { }

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

    // TODO @vyang: implement SphSimulation here
    // You can use the following:
    // - particles
    // - neighborSearch->getNeighbors(Eigen::Vector3d& position, double radius)

    // Dummy code
    auto dt = 1. / 10;
    for (auto& particle : particles) {
        particle.position += particle.velocity * dt;
    }


    // Needs to be called to update particle related data
    ParticleSimulation::postAdvance();
	return false;
}