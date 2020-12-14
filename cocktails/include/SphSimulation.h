#ifndef PBS_SPH_SIMULATION_H
#define PBS_SPH_SIMULATION_H

#include "FluidSimulation.h"
#include "NeighborSearch.h"
#include "SimulationLoader.h"
#include "SPHKernels.h"

#include <string>


/// Mueller03 Basic SPH implementation with multi-threading
class SphSimulation : public FluidSimulation {
public:
    SphSimulation();

	virtual bool advance() override;
	virtual void resetMembers() override;

	/// SPH Kernel radius
	double m_kernelRadius;

	/// Width of a cell in the uniform grid, ideally equal to kernel Radius
	double m_gridWidth;

	/// Regulates the strength of the boundary repulsion
	double m_boundary_repulsion;

	/// If true, gravity force will act on all particles
    bool m_enableGravity = true;

    /// If true, surface tension forces will be added
    bool m_enableSurfaceTension = true;

protected:

    virtual void updateDensityAndPressure();
	virtual void updateForce();
	virtual void updateVelocityAndPosition();

    NeighborSearch* m_neighborSearch;
	void updateNeighbors();

	SPHKernels* m_kernels;

	friend class SimulationLoader;

};

#endif // PBS_SPH_SIMULATION_H