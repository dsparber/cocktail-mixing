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
    void updateDensityandPressure();
	void updateForce();
	void updateVelocityandPosition();

	double m_kernelRadius = 0.2;
	constexpr static const double m_gridWidth = 4.0;

	double m_stiffness;
	double m_density_0;
	double m_viscosity;

};