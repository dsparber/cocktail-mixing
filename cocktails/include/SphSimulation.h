#ifndef PBS_SPH_SIMULATION_H
#define PBS_SPH_SIMULATION_H

#include "ParticleSimulation.h"

/*
 * Example simulation that changes the colors of a cube.
 */
class SphSimulation : public ParticleSimulation {
public:
    SphSimulation();

	void init() override;
	void resetMembers() override;
	bool advance() override;

private:
    void updateDensityAndPressure();
	void updateForce();
	void updateVelocityAndPosition();

	double m_kernelRadius = 0.2;
	constexpr static const double m_gridWidth = 0.2; // Width of a cell in the uniform grid, ideally equal to kernel Radius

	double m_stiffness;
	double m_density_0;
	double m_viscosity;

};

#endif