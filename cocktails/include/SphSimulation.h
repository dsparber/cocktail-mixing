#ifndef PBS_SPH_SIMULATION_H
#define PBS_SPH_SIMULATION_H

#include "FluidSimulation.h"
#include "NeighborSearch.h"

/*
 * Example simulation that changes the colors of a cube.
 */
class SphSimulation : public FluidSimulation {
public:
    SphSimulation();

	virtual bool advance() override;
	virtual void resetMembers() override;

	void setKernelRadius(double kernelRadius);
	void setGridWidth(double gridwidth);
	
private:
    void updateDensityAndPressure();
	void updateForce();
	void updateVelocityAndPosition();

    NeighborSearch* m_neighborSearch;
	void updateNeighbors();

	double m_kernelRadius;
	double m_gridWidth; // Width of a cell in the uniform grid, ideally equal to kernel Radius
};

#endif // PBS_SPH_SIMULATION_H