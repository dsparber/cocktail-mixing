#ifndef PBS_SPH_SIMULATION_H
#define PBS_SPH_SIMULATION_H

#include "FluidSimulation.h"

/*
 * Example simulation that changes the colors of a cube.
 */
class SphSimulation : public FluidSimulation {
public:
    SphSimulation();
	bool advance() override;

private:
    void updateDensityAndPressure();
	void updateForce();
	void updateVelocityAndPosition();

	constexpr static const double m_kernelRadius = 0.2;
	constexpr static const double m_gridWidth = 0.2; // Width of a cell in the uniform grid, ideally equal to kernel Radius
};

#endif