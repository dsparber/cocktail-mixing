#ifndef PBS_DCSPH_SIMULATION_H
#define PBS_DCSPH_SIMULATION_H

#include "SphSimulation.h"
#include "NeighborSearch.h"

///
/// Sol08 Density Contrast SPH
///
class DCSPHSimulation : public SphSimulation {
public:
    DCSPHSimulation();

protected:
    virtual void updateDensityAndPressure() override;
	virtual void updateForce() override;

};

#endif // PBS_DCSPH_SIMULATION_H